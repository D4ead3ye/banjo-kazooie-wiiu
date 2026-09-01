// Free look camera

#include <cmath>

#include <libultraship/bridge/consolevariablebridge.h>

#include "port/UI/cvar_prefixes.h"
#include "port/Enhancements/Camera/FreeLookCamera.h"
#ifdef __WIIU__
#include <coreinit/time.h>
#include "port/WiiUDebug.h"
#endif

extern "C" {
// Math helpers
void func_80256E24(float dst[3], float pitch, float yaw, float x, float y, float z);
int getGameMode(void);
void ml_vec3f_add(float dst[3], float a[3], float b[3]);
void ml_vec3f_clear(float dst[3]);
float mlNormalizeAngle(float deg);

float time_getDelta(void);

void controller_getRightStick(int controller_index, float dst[2]);

int bainput_should_rotate_camera_left(void);
int bainput_should_rotate_camera_right(void);
int bainput_should_look_first_person_camera(void);

int ncDynamicCamera_getState(void);
void ncDynamicCamera_setState(int state);
void ncDynamicCamera_getPosition(float dst[3]);
void ncDynamicCamera_setRotation(float src[3]);

// Vanilla follow-camera machinery
extern float D_8037DB70;    // dynamicCamB.c: orbit yaw, degrees
extern float D_8037D9C8[3]; // dynamicCamera.c: rotation spring velocity

void func_802C0150(int mode);        // select the focus target
void func_802C0370(void);            // record the pre-move position (anti-flip guard)
void func_802C0394(float target[3]); // record the ideal target (anti-flip guard)
void func_802C03BC(void);            // undo a collision resolve that crossed over
void func_802C0490(float dst[3]);    // focus/orbit center
void func_802C04B0(void);            // re-derive D_8037DB70 from the live camera

float func_802BD8D4(void);                           // target orbit distance (zoom level)
void func_802BE190(float target[3]);                 // position spring toward target
void func_802BE230(float gain, float damp);          // position spring tuning
int func_802BE60C(void);                             // swept camera collision + slide
int func_802BC84C(int mode);                         // occluded-too-long recovery
void func_802BE6FC(float rotOut[3], float focus[3]); // look-at from the live position
}

namespace {
#define CVAR_FREELOOK_ENABLED CVAR_ENHANCEMENT("Camera.FreeLook.Enabled")
// Lets free look take over the fixed camera-node angles and the underwater camera.
#define CVAR_FREELOOK_MODERN CVAR_ENHANCEMENT("Camera.FreeLook.OverrideFixed")
// Spring-arm behaviour near walls: keep the player's steering through a
// collision, and pull the camera in rather than letting it jam.
#define CVAR_FREELOOK_WALLS CVAR_ENHANCEMENT("Camera.FreeLook.WallSmoothing")
#define CVAR_FREELOOK_YAW_SENS CVAR_ENHANCEMENT("Camera.FreeLook.YawSensitivity")
#define CVAR_FREELOOK_PITCH_SENS CVAR_ENHANCEMENT("Camera.FreeLook.PitchSensitivity")
#define CVAR_FREELOOK_INVERT_X CVAR_ENHANCEMENT("Camera.FreeLook.InvertX")
#define CVAR_FREELOOK_INVERT_Y CVAR_ENHANCEMENT("Camera.FreeLook.InvertY")
#define CVAR_FREELOOK_SMOOTH_RATE CVAR_ENHANCEMENT("Camera.FreeLook.SmoothRate")

constexpr float kDeadzone = 0.15f;
// GAME_MODE_3_NORMAL; enums.h is not reachable from this translation unit.
constexpr int kGameModeNormal = 3;
// Spring-arm limits. Give ground quickly so the camera never clips into
// geometry, but recover slowly and only after the way has been clear for a
// moment.
//
// The blocked/clear signal is binary, so feeding it straight back into the
// distance oscillates: pulling in clears the obstruction, which lets the arm
// extend, which blocks it again. Asymmetric rates alone only change how fast it
// pumps. The dwell breaks the loop - the arm cannot start extending until it has
// been unobstructed for kClearDwell, so one wall contact settles at a short
// length instead of buzzing against it.
constexpr float kMinDistScale = 0.45f;
constexpr float kDistPullInRate = 3.0f;
constexpr float kDistPushOutRate = 0.5f;
constexpr float kClearDwell = 0.4f;

// The collision state drives a *target* length; what the camera actually uses
// eases towards it. Applying the target directly meant every change of state
// showed up on screen the instant it happened, which is what made rotating along
// a wall feel like the camera was snapping at you. Shrinking eases faster than
// growing so the arm still gets out of geometry promptly.
constexpr float kDistShrinkSmooth = 12.0f;
constexpr float kDistGrowSmooth = 3.5f;
float sDistTarget = 1.0f;
float sDistScale = 1.0f;
float sClearTime = 0.0f;

// Stick response. A curve gives fine control near centre while keeping the full
// rate at the edge, and easing the angular velocity stops the camera starting
// and stopping dead with the stick.
constexpr float kStickCurve = 1.7f;
constexpr float kTurnSmooth = 14.0f;
float sYawVel = 0.0f;
float sPitchVel = 0.0f;

// Frame-rate independent exponential smoothing.
float ExpSmooth(float current, float target, float rate, float dt) {
    if (rate <= 0.0f || dt <= 0.0f) {
        return target;
    }
    return current + (target - current) * (1.0f - std::exp(-rate * dt));
}

constexpr float kEnterThreshold = 0.30f;
constexpr float kYawSpeed = 160.0f;
constexpr float kPitchSpeed = 100.0f;

constexpr float kMinPitch = -85.0f;
// [port] Free look places the camera analytically and so does not get the
// collision push-out the stock camera relies on. Positive pitch swings it below
// the player, where it used to sink through the floor; keep it shallow enough
// that it stays in the room.
constexpr float kMaxPitch = 22.0f;

constexpr float kAimHeightRate = 8.0f;
constexpr float kDefaultSmooth = 40.0f;
constexpr float kSmoothToGain = 0.5f;
constexpr float kDampRatio = 4.0f;

constexpr float kRadToDeg = 57.29577951308232f;

bool sActive = false;
bool sAimValid = false;
float sPitch = 0.0f;
float sAimY = 0.0f;

float clampf(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

float SpringGain() {
    return CVarGetFloat(CVAR_FREELOOK_SMOOTH_RATE, kDefaultSmooth) * kSmoothToGain;
}

float ReadStick(float out[2]) {
    controller_getRightStick(0, out);
    float mag = std::sqrt(out[0] * out[0] + out[1] * out[1]);
    if (mag < kDeadzone) {
        out[0] = out[1] = 0.0f;
        return 0.0f;
    }
    if (mag > 1.0f) {
        mag = 1.0f;
    }
    float scaled = (mag - kDeadzone) / (1.0f - kDeadzone);
    // Curve the magnitude: small deflections turn slowly for aiming, full
    // deflection still reaches the full rate.
    scaled = std::pow(scaled, kStickCurve);
    float k = scaled / mag;
    out[0] *= k;
    out[1] *= k;
    return scaled;
}

bool ManualCameraControl() {
    return bainput_should_rotate_camera_left() || bainput_should_rotate_camera_right() ||
           bainput_should_look_first_person_camera();
}

float CapturePitch() {
    float camPos[3];
    float focus[3];
    ncDynamicCamera_getPosition(camPos);
    func_802C0490(focus);

    float diff[3] = { camPos[0] - focus[0], camPos[1] - focus[1], camPos[2] - focus[2] };
    float dist = std::sqrt(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
    if (dist < 1.0f) {
        return 0.0f;
    }
    return clampf(std::asin(clampf(-diff[1] / dist, -1.0f, 1.0f)) * kRadToDeg, kMinPitch, kMaxPitch);
}

void EnterOrbit() {
    func_802C0150(2);
    func_802BE230(SpringGain(), SpringGain() * kDampRatio);
    func_802C04B0();
    sPitch = CapturePitch();

    ml_vec3f_clear(D_8037D9C8);

    // Start each orbit at full length; a value left over from the last wall
    // would otherwise open the camera up close for half a second.
    sDistScale = 1.0f;
    sDistTarget = 1.0f;
    sClearTime = 0.0f;
    sYawVel = 0.0f;
    sPitchVel = 0.0f;

    sAimValid = false;
    sActive = true;
    ncDynamicCamera_setState(FREELOOK_CAM_STATE);
}

void ExitOrbit() {
    sActive = false;
    if (ncDynamicCamera_getState() == FREELOOK_CAM_STATE) {
        ncDynamicCamera_setState(0xB);
    }
}

} // namespace

extern "C" int port_freeLook_isEnabled(void) {
    // [port] Dual analog is what a second stick is for, so it is on by default
    // on every target. The in-game menu is the normal way to change this; on
    // console it can also be set in config.yml next to the .wuhb.
    return CVarGetInteger(CVAR_FREELOOK_ENABLED, 1) != 0;
}

extern "C" int port_freeLook_overridesFixed(void) {
    return port_freeLook_isEnabled() && CVarGetInteger(CVAR_FREELOOK_MODERN, 1) != 0;
}

// True while free look is actually driving the camera. The game re-forces its
// fixed camera states every frame, so those sites have to leave the state alone
// while this holds or they would yank the camera back a frame after every nudge.
//
// sActive on its own is not enough to claim that. The fixed-camera sites are
// evaluated before the handler that clears sActive - they short-circuit it - so a
// flag left over from ordinary gameplay made them stand down in places where free
// look was not running at all. Nothing owned the camera then, and it drifted
// out of bounds; in the picture modes that ends up in the bonus picture. Require
// that free look really is the live camera, in a mode where it is allowed.
extern "C" int port_freeLook_ownsCamera(void) {
    const int state = ncDynamicCamera_getState();
    const int mode = getGameMode();
    const bool owns = sActive && state == FREELOOK_CAM_STATE && mode == kGameModeNormal &&
                      port_freeLook_overridesFixed();
#ifdef __WIIU__
    // [port] A report of the camera locking in one spot underwater, with no way
    // to steer until surfacing, could not be reproduced. The entry path does
    // cover state 3, so whatever declined it is one of the other terms. Say
    // which, once a second and only while a fixed state is actually being held,
    // so a recurrence names its own cause instead of needing a repro.
    if (!owns && port_freeLook_isEnabled() && (state == 0x3 || state == 0x8 || state == 0x11)) {
        static uint32_t sLastReport = 0;
        const uint32_t now = (uint32_t)(OSTicksToMilliseconds(OSGetSystemTime()));
        if (now - sLastReport > 1000) {
            sLastReport = now;
            WIIU_TRACE("[cam] declined fixed state=%d mode=%d active=%d overrides=%d", state, mode,
                       (int)sActive, port_freeLook_overridesFixed());
        }
    }
#endif
    return owns;
}

extern "C" int port_freeLook_handle(void) {
    // [port] The camera states free look hooks are also used by the attract demo
    // and the intro, so gating on state alone let the right stick drive the
    // camera on the title screen. Only take over during actual gameplay.
    if (getGameMode() != kGameModeNormal) {
        if (sActive) {
            ExitOrbit();
        }
        return 0;
    }

    if (!port_freeLook_isEnabled()) {
        if (sActive) {
            ExitOrbit();
        }
        return 0;
    }

    int state = ncDynamicCamera_getState();

    if (sActive) {
        if (state != FREELOOK_CAM_STATE) {
            sActive = false;
            return 0;
        }
        if (ManualCameraControl()) {
            ExitOrbit();
            return 0;
        }
        return 1;
    }

    if (ManualCameraControl()) {
        return 0;
    }

    // 0xB/0x1/0xA are the ordinary follow states. With the modern option on we
    // also take over the states the level forces on the player: 0x3 underwater,
    // 0x8 from a pivot camera node and 0x11 from a zoom node - the ones that
    // otherwise lock the view to a fixed angle.
    const bool modernCam = port_freeLook_overridesFixed();
    const bool ordinary = (state == 0xB || state == 0x1 || state == 0xA);
    const bool fixed = (state == 0x3 || state == 0x8 || state == 0x11);
    if (!ordinary && !(modernCam && fixed)) {
        return 0;
    }

    float stick[2];
    if (ReadStick(stick) >= kEnterThreshold) {
        EnterOrbit();
        return 1;
    }

    return 0;
}

extern "C" void port_freeLookCamera_update(void) {
    float dt = time_getDelta();

    float stick[2];
    ReadStick(stick);

#ifdef __WIIU__
    // Deliberately NOT mirrored to other targets. Both axes came out reversed on
    // the GamePad specifically - stick left panned right, stick up looked down -
    // so this corrects that pad, not a convention the game shares. An SDL pad on
    // desktop already reports the standard sense.
    const int kInvertXDefault = 1;
    const int kInvertYDefault = 0;
#else
    const int kInvertXDefault = 0;
    const int kInvertYDefault = 0;
#endif
    float yawSens = CVarGetFloat(CVAR_FREELOOK_YAW_SENS, 1.0f);
    float pitchSens = CVarGetFloat(CVAR_FREELOOK_PITCH_SENS, 1.0f);
    bool invertX = CVarGetInteger(CVAR_FREELOOK_INVERT_X, kInvertXDefault) != 0;
    bool invertY = CVarGetInteger(CVAR_FREELOOK_INVERT_Y, kInvertYDefault) != 0;

    // [port] Ease the turn rate rather than driving the angle straight from the
    // stick. Modern third-person cameras ramp in and out over a few frames; the
    // raw stick made every flick start and stop dead.
    const float yawWanted = (invertX ? -stick[0] : stick[0]) * kYawSpeed * yawSens;
    const float pitchWanted = (invertY ? -stick[1] : stick[1]) * kPitchSpeed * pitchSens;
    sYawVel = ExpSmooth(sYawVel, yawWanted, kTurnSmooth, dt);
    sPitchVel = ExpSmooth(sPitchVel, pitchWanted, kTurnSmooth, dt);

    D_8037DB70 = mlNormalizeAngle(D_8037DB70 + sYawVel * dt);
    // [port] Default is now stick-up looks up. InvertY still flips it for anyone
    // who wants the other convention.
    sPitch = clampf(sPitch + sPitchVel * dt, kMinPitch, kMaxPitch);

    float focus[3];
    float offset[3];
    float target[3];

    const bool smoothWalls = CVarGetInteger(CVAR_FREELOOK_WALLS, 1) != 0;

    func_802C0370();
    func_802C0490(focus);
    // [port] Spring arm: shorten the orbit while something is in the way so the
    // camera slides in close instead of grinding along the wall at full length.
    const float dist = func_802BD8D4() * (smoothWalls ? sDistScale : 1.0f);
    func_80256E24(offset, sPitch, D_8037DB70, 0.0f, 0.0f, dist);
    ml_vec3f_add(target, focus, offset);

    // What the player asked for this frame. A collision resolve rewrites this,
    // which is what made the camera fight the stick near walls.
    const float desiredYaw = D_8037DB70;

    func_802C0394(target);
    func_802BE190(target);

    const bool blocked = func_802BE60C() != 0;
    if (blocked) {
        if (!func_802BC84C(1)) {
            func_802C03BC();
        }
        func_802C04B0();
        // [port] func_802C04B0 re-derives the orbit yaw from where the collision
        // left the camera, discarding the angle the player was steering towards -
        // so a wall would capture the camera and only enough extra rotation could
        // pull it free. Put the intended yaw back and let the spring keep working
        // towards it; collision still owns the position, just not the aim.
        if (smoothWalls) {
            D_8037DB70 = desiredYaw;
        }
    }

    if (smoothWalls) {
        if (blocked) {
            sClearTime = 0.0f;
            sDistTarget -= kDistPullInRate * dt;
        } else {
            sClearTime += dt;
            if (sClearTime >= kClearDwell) {
                sDistTarget += kDistPushOutRate * dt;
            }
        }
        sDistTarget = clampf(sDistTarget, kMinDistScale, 1.0f);
        // Ease towards the target instead of using it directly.
        sDistScale = ExpSmooth(sDistScale, sDistTarget,
                               (sDistTarget < sDistScale) ? kDistShrinkSmooth : kDistGrowSmooth, dt);
    } else {
        sDistTarget = 1.0f;
        sDistScale = 1.0f;
        sClearTime = 0.0f;
    }

    func_802C0490(focus);
    if (!sAimValid) {
        sAimY = focus[1];
        sAimValid = true;
    } else {
        sAimY += (focus[1] - sAimY) * clampf(kAimHeightRate * dt, 0.0f, 1.0f);
    }
    focus[1] = sAimY;

    float rot[3];
    func_802BE6FC(rot, focus);
    ncDynamicCamera_setRotation(rot);
}
