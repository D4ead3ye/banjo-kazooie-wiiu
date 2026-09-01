#include <libultraship/libultraship.h>
#include <libultraship/bridge/consolevariablebridge.h>

#include "port/Engine.h"
#include "port/UI/cvar_prefixes.h"
#include "port/Enhancements/Events/Hooks/Events.h"
#include "port/ShipInit.hpp"
#include "port/ShipUtils.h"

#include "enums.h"
#include "functions.h"
#include "variables.h"

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

extern "C" {
extern float sViewportFOVy;
extern float sViewportAspect;
}

#define CVAR_AR_ENABLED CVAR_PREFIX_ADVANCED_RESOLUTION ".Enabled"
#define CVAR_AR_COMBO CVAR_PREFIX_ADVANCED_RESOLUTION ".UIComboItem.AspectRatio"
#define CVAR_AR_X CVAR_PREFIX_ADVANCED_RESOLUTION ".AspectRatioX"
#define CVAR_AR_Y CVAR_PREFIX_ADVANCED_RESOLUTION ".AspectRatioY"
#define CVAR_CUTSCENE_ASPECT CVAR_ENHANCEMENT("Graphics.CutsceneAspect")
// Keeps vanilla 4:3 actor culling during cutscenes so off-camera actors stay
// hidden at widescreen. Only affects the culling frustum, not the projection.
#define CVAR_CUTSCENE_CULL CVAR_ENHANCEMENT("Graphics.CutsceneActorCull")
#ifdef __WIIU__
static constexpr int kCutsceneCullDefault = 1;
#else
static constexpr int kCutsceneCullDefault = 0;
#endif

// [port] Off on console. This does not clamp the projection - it forces
// libultraship's advanced-resolution override to 4:3, which the GX2 backend does
// not render: turning it on gave a black screen from the first cutscene onward.
// Fixing the intro's widescreen framing needs a projection-side change instead.
static constexpr int kCutsceneAspectDefault = 0;
#define CVAR_CSA_BAK_ACTIVE CVAR_ENHANCEMENT("Graphics.CutsceneAspectBackup.Active")
#define CVAR_CSA_BAK_ENABLED CVAR_ENHANCEMENT("Graphics.CutsceneAspectBackup.Enabled")
#define CVAR_CSA_BAK_COMBO CVAR_ENHANCEMENT("Graphics.CutsceneAspectBackup.Combo")
#define CVAR_CSA_BAK_X CVAR_ENHANCEMENT("Graphics.CutsceneAspectBackup.X")
#define CVAR_CSA_BAK_Y CVAR_ENHANCEMENT("Graphics.CutsceneAspectBackup.Y")
#define CVAR_WS_CAMERA_FIX CVAR_ENHANCEMENT("Fix.WidescreenCamera")
#define CVAR_NO_SCREEN_SHAKE CVAR_SETTING("A11yDisableScreenShake")

static int32_t sCutsceneAspectActive = 0;
static constexpr float kWsPositionTolerance = 2.0f;
static constexpr float kWsRotationTolerance = 1.0f;
static constexpr float kWsReferenceAspect = 16.0f / 9.0f;

static void restoreCutsceneAspectBackup() {
    CVarSetInteger(CVAR_AR_ENABLED, CVarGetInteger(CVAR_CSA_BAK_ENABLED, 0));
    CVarSetInteger(CVAR_AR_COMBO, CVarGetInteger(CVAR_CSA_BAK_COMBO, 3));
    CVarSetFloat(CVAR_AR_X, CVarGetFloat(CVAR_CSA_BAK_X, 16.0f));
    CVarSetFloat(CVAR_AR_Y, CVarGetFloat(CVAR_CSA_BAK_Y, 9.0f));
    CVarClear(CVAR_CSA_BAK_ACTIVE);
    sCutsceneAspectActive = 0;
}

// [port] True while the loaded map is a cutscene. Range-checked because
// gsworld_getMap() is not meaningful before the first map load and map_getLevel
// indexes a table with it.
static bool currentMapIsCutscene() {
    int32_t m = (int32_t)gsworld_getMap();
    if (m < 0 || m >= MAP_NUM_MAPS) {
        return false;
    }
    return map_getLevel((enum map_e)m) == LEVEL_D_CUTSCENE;
}

static void updateCutsceneAspect(int32_t mapId) {
    bool isCutscene = (map_getLevel((enum map_e)mapId) == LEVEL_D_CUTSCENE);

    if (isCutscene && !sCutsceneAspectActive) {
        bool enabled = CVarGetInteger(CVAR_AR_ENABLED, 0);
        float arX = CVarGetFloat(CVAR_AR_X, 0.0f);
        float arY = CVarGetFloat(CVAR_AR_Y, 0.0f);
        auto window = Ship::Context::GetRawInstance()->GetWindow();
        uint32_t winW = window->GetWidth();
        uint32_t winH = window->GetHeight();
        float actual = (winH > 0) ? (float)winW / (float)winH : GameEngine_GetAspectRatio();
        if ((arY > 0.0f && (arX / arY) > (4.0f / 3.0f + 0.01f)) || (!enabled && actual > (4.0f / 3.0f + 0.01f))) {
            CVarSetInteger(CVAR_CSA_BAK_ENABLED, CVarGetInteger(CVAR_AR_ENABLED, 0));
            CVarSetInteger(CVAR_CSA_BAK_COMBO, CVarGetInteger(CVAR_AR_COMBO, 3));
            CVarSetFloat(CVAR_CSA_BAK_X, CVarGetFloat(CVAR_AR_X, 16.0f));
            CVarSetFloat(CVAR_CSA_BAK_Y, CVarGetFloat(CVAR_AR_Y, 9.0f));
            CVarSetInteger(CVAR_CSA_BAK_ACTIVE, 1);
            CVarSetInteger(CVAR_AR_ENABLED, 1);
            CVarSetInteger(CVAR_AR_COMBO, 2);
            CVarSetFloat(CVAR_AR_X, 4.0f);
            CVarSetFloat(CVAR_AR_Y, 3.0f);
            sCutsceneAspectActive = 1;
        }
    } else if (!isCutscene && sCutsceneAspectActive) {
        restoreCutsceneAspectBackup();
    }
}

struct WsCameraFix {
    int32_t map;
    int32_t source;
    int32_t id = -1;
    bool matchTransform = false;
    float position[3] = { 0.0f, 0.0f, 0.0f }; // x, y, z
    float rotation[3] = { 0.0f, 0.0f, 0.0f }; // pitch, yaw, roll
    float adjust[3] = { 0.0f, 0.0f, 0.0f };   // pitch, yaw, roll
};

// If adding a camera adjustment here, you need the following:
//   - map: the map the camera is in
//   - source: the camera type
//   - matchTransform: true if the camera moves and you need conditional adjustment
//   - position, rotation: keys for matchTransform
//   - adjust: self explanatory
static const WsCameraFix sWsCameraFixes[] = {
    // Concert, when Banjo looks at Tooty
    { .map = MAP_1E_CS_START_NINTENDO,
      .source = CAMERA_TYPE_1_UNKNOWN,
      .matchTransform = true,
      .position = { 464.32f, 382.75f, 258.68f },
      .rotation = { 0.0f, 83.0f, 0.0f },
      .adjust = { 0.0f, -5.0f, 0.0f } },
    // Eggs molehill
    { .map = MAP_2_MM_MUMBOS_MOUNTAIN, .source = CAMERA_TYPE_3_STATIC, .id = 0x16, .adjust = { 5.9f, -6.0f, 0.0f } },
    // Beak Buster molehill
    { .map = MAP_2_MM_MUMBOS_MOUNTAIN, .source = CAMERA_TYPE_3_STATIC, .id = 0x17, .adjust = { 0.0f, -6.0f, 0.0f } },
};

static constexpr int WS_CAMERA_FIX_COUNT = sizeof(sWsCameraFixes) / sizeof(sWsCameraFixes[0]);

struct WsAuthoredCamera {
    bool valid;
    int32_t map;
    int32_t source;
    int32_t id;
    float position[3];
    float rotation[3];
};

static WsAuthoredCamera sWsAuthored;
static float sWsLastAspect;

extern "C" float port_wsCameraYawScale(void) {
    float fovY = sViewportFOVy > 0.0f ? sViewportFOVy : 40.0f;
    float tanHalfFovY = std::tan((fovY * 0.5f) * (float)(M_PI / 180.0));
    float base = std::atan(tanHalfFovY * sViewportAspect);
    float reference = std::atan(tanHalfFovY * kWsReferenceAspect) - base;
    if (reference <= 0.0f) {
        return 1.0f;
    }
    float extra = std::atan(tanHalfFovY * GameEngine_GetAspectRatio()) - base;
    return extra > 0.0f ? extra / reference : 0.0f;
}

extern "C" float port_wsCameraPitchScale(void) {
    return std::sqrt(port_wsCameraYawScale());
}

static void wsCameraScales(float scale[3]) {
    scale[1] = port_wsCameraYawScale();
    scale[0] = std::sqrt(scale[1]);
    scale[2] = 1.0f;
}

static float wsAngleDifference(float a, float b) {
    float delta = a - b;
    while (delta > 180.0f) {
        delta -= 360.0f;
    }
    while (delta < -180.0f) {
        delta += 360.0f;
    }
    return delta;
}

static bool wsCameraFixMatches(const WsCameraFix& fix, const CameraRotationAuthored* ev, int32_t curMap) {
    if (curMap != fix.map || ev->source != fix.source) {
        return false;
    }
    if (fix.id != -1 && ev->id != fix.id) {
        return false;
    }
    if (!fix.matchTransform) {
        return true;
    }
    if (ev->position == nullptr) {
        return false;
    }
    for (int i = 0; i < 3; i++) {
        if (std::fabs(ev->position[i] - fix.position[i]) > kWsPositionTolerance) {
            return false;
        }
        if (std::fabs(wsAngleDifference(ev->rotation[i], fix.rotation[i])) > kWsRotationTolerance) {
            return false;
        }
    }
    return true;
}

static void wsReauthorStaticCamera() {
    float rotation[3];
    for (int i = 0; i < 3; i++) {
        rotation[i] = sWsAuthored.rotation[i];
    }
    CALL_EVENT(CameraRotationAuthored, sWsAuthored.source, sWsAuthored.id, sWsAuthored.position, rotation);

    float position[3];
    ncStaticCamera_getPosition(position);
    ncStaticCamera_setPositionAndRotation(position, rotation);
}

// Event listeners

void RegisterCutsceneAspect() {
    if (CVarGetInteger(CVAR_CSA_BAK_ACTIVE, 0)) {
        restoreCutsceneAspectBackup();
    }
    COND_HOOK(OnMapLoad, EVENT_PRIORITY_NORMAL, CVarGetInteger(CVAR_CUTSCENE_ASPECT, kCutsceneAspectDefault), [](IEvent* event) {
        OnMapLoad* ev = (OnMapLoad*)event;
        updateCutsceneAspect(ev->nextMap);
    });

    // The hook above only ever sees the *next* map, so a cutscene that is
    // already on screen when this runs never gets the 4:3 clamp - and this
    // runs both at boot and every time the CVar changes. That left the
    // setting doing nothing until the following map load: turn it on during
    // the intro and the out-of-frame scenery stays visible for the rest of
    // it, then behaves correctly the next time round. Apply it to the map
    // that is already loaded.
    // [port] Skip this on the very first call. It runs from GameEngine::Create,
    // before any map exists: gsworld_getMap() returns an uninitialised value that
    // can pass a range check and still fault inside map_getLevel(), whose table is
    // not built yet. Every later call comes from the CVar changing at runtime,
    // when a map really is loaded - which is the case this block is for. At boot
    // the OnMapLoad hook above covers the first map anyway.
    static bool sPastBootCall = false;
    if (!sPastBootCall) {
        sPastBootCall = true;
        return;
    }

    if (CVarGetInteger(CVAR_CUTSCENE_ASPECT, kCutsceneAspectDefault)) {
        int32_t curMap = (int32_t)gsworld_getMap();
        if (curMap >= 0 && curMap < MAP_NUM_MAPS) {
            updateCutsceneAspect(curMap);
        }
    }
}

void RegisterWidescreenCamera() {
    COND_HOOK(CameraRotationAuthored, EVENT_PRIORITY_NORMAL, CVarGetInteger(CVAR_WS_CAMERA_FIX, 1), [](IEvent* event) {
        float scale[3];
        wsCameraScales(scale);
        if (scale[1] <= 0.0f) {
            return;
        }
        auto* ev = (CameraRotationAuthored*)event;
        int32_t curMap = (int32_t)gsworld_getMap();
        for (int i = 0; i < WS_CAMERA_FIX_COUNT; i++) {
            if (wsCameraFixMatches(sWsCameraFixes[i], ev, curMap)) {
                for (int axis = 0; axis < 3; axis++) {
                    ev->rotation[axis] =
                        mlNormalizeAngle(ev->rotation[axis] + sWsCameraFixes[i].adjust[axis] * scale[axis]);
                }
                break;
            }
        }
    });

    COND_HOOK(CameraRotationAuthored, EVENT_PRIORITY_LOW, CVarGetInteger(CVAR_WS_CAMERA_FIX, 1), [](IEvent* event) {
        auto* ev = (CameraRotationAuthored*)event;
        sWsAuthored.valid = true;
        sWsAuthored.map = (int32_t)gsworld_getMap();
        sWsAuthored.source = ev->source;
        sWsAuthored.id = ev->id;
        for (int i = 0; i < 3; i++) {
            sWsAuthored.rotation[i] = ev->rotation[i];
            sWsAuthored.position[i] = ev->position != nullptr ? ev->position[i] : 0.0f;
        }
    });

    COND_HOOK(GameFrameUpdate, EVENT_PRIORITY_NORMAL, CVarGetInteger(CVAR_WS_CAMERA_FIX, 1), [](IEvent* event) {
        float aspect = GameEngine_GetAspectRatio();
        if (std::fabs(aspect - sWsLastAspect) < 0.0005f) {
            return;
        }
        sWsLastAspect = aspect;
        if (!sWsAuthored.valid || sWsAuthored.source != CAMERA_TYPE_3_STATIC) {
            return;
        }
        if (sWsAuthored.map != (int32_t)gsworld_getMap() || ncCamera_getType() != CAMERA_TYPE_3_STATIC) {
            return;
        }
        wsReauthorStaticCamera();
    });
}

void RegisterCameraPatches_Init() {

    COND_VB_SHOULD(VB_CAMERA_LIVE_ASPECT, EVENT_PRIORITY_NORMAL, true, { *should = false; });

    // Bigger frustum for wider aspect ratios, no-op for deterministic scenes
    REGISTER_LISTENER(ViewportFrustumUpdate, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        // [port] Cutscenes are framed for 4:3, and actors wait off to the sides
        // until their cue. Widening the frustum for 16:9 stops culling them, so
        // they can be seen loitering - the insect at the start of the intro is
        // the clearest case. IsDemoMode() alone did not cover this: the intro
        // runs as a scripted cutscene in GAME_MODE_3_NORMAL on a cold boot, and
        // only takes the attract-demo path afterwards, which is exactly why it
        // was visible once per launch and never again.
        if (IsDemoMode() || (CVarGetInteger(CVAR_CUTSCENE_CULL, kCutsceneCullDefault) && currentMapIsCutscene())) {
            return;
        }
        auto* ev = (ViewportFrustumUpdate*)event;
        const float kFrustumZ = 45.168514251708984f;
        const float kMargin = 1.10f;
        float aspect = GameEngine_GetAspectRatio();
        if (aspect < sViewportAspect) {
            aspect = sViewportAspect;
        }
        float halfFovYRad = (sViewportFOVy * 0.5f) * (float)(M_PI / 180.0);
        float halfFovXRad = std::atan(std::tan(halfFovYRad) * aspect * kMargin);
        *ev->frustumX = kFrustumZ / std::tan(halfFovXRad);
        *ev->frustumY = 93.9692611694336f * 1.15f;
    });
}

void RegisterScreenShake_Init() {
    COND_VB_SHOULD(VB_CAMERA_APPLY_SHAKE, EVENT_PRIORITY_NORMAL, CVarGetInteger(CVAR_NO_SCREEN_SHAKE, 0),
                   { *should = false; });
}

static RegisterShipInitFunc cutsceneAspectInitFunc(RegisterCutsceneAspect, { CVAR_CUTSCENE_ASPECT });
static RegisterShipInitFunc widescreenCameraInitFunc(RegisterWidescreenCamera, { CVAR_WS_CAMERA_FIX });
static RegisterShipInitFunc staticCamInitFunc(RegisterCameraPatches_Init);
static RegisterShipInitFunc screenShakeInitFunc(RegisterScreenShake_Init, { CVAR_NO_SCREEN_SHAKE });
