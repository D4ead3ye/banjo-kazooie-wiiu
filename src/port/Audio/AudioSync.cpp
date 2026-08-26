// [port] Audio engine synchronization: a single recursive lock serializing the free-running
// audio worker (GameEngine::HandleAudioThread, advancing the synth in n_alAudioFrame) against the
// game thread's SFX/music writes.
//
// osSetIntMask lives here, not with the libultra OS stubs, because in this port it's no longer an
// interrupt primitive — its only callers are the N64 audio files, which bracket every
// thread-shared section with osSetIntMask(OS_IM_NONE)/restore. Routing those brackets to this
// lock protects all of them with no edits to the audio code.

#ifdef __WIIU__
#include "port/WiiUThreadLocal.h"
#include <whb/log.h>
#endif
#include <atomic>
#include <chrono>
#include <cstdint>
#include <condition_variable>
#include <mutex>
#include <thread>

extern "C" {
#include <libultra/exception.h> // OSIntMask, OS_IM_NONE
}

namespace {
// osSetIntMask is a *global* interrupt mask on the N64, so the depth that
// mirrors it has to be global too. Keeping it per-thread meant that a section
// opened on one thread and restored on another - which loading a level's sound
// banks does - left the lock held forever and hung every thread that touched
// audio afterwards. A recursive_mutex cannot express that either, since only
// the owning thread may unlock one, so this is a hand-rolled recursive lock
// whose depth any thread can release.
std::mutex gAudioMutex;
std::condition_variable gAudioCv;
int gAudioMaskDepth = 0;
bool gAudioMaskHeld = false;
std::thread::id gAudioMaskOwner;
} // namespace

// Window-freeze audio hold
namespace {
constexpr int64_t kCushionMs = 100;
std::atomic<int64_t> sAllowedUntilMs{ 0 };

int64_t NowMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch())
        .count();
}
} // namespace

extern "C" void port_noteMainLoopAlive(void) {
    sAllowedUntilMs.store(NowMs() + kCushionMs, std::memory_order_relaxed);
}

extern "C" int port_audioStallHold(void) {
    const int64_t allowedUntil = sAllowedUntilMs.load(std::memory_order_relaxed);
    return allowedUntil != 0 && NowMs() > allowedUntil;
}

extern "C" void port_audioIntMaskEnter(void);
extern "C" void port_audioIntMaskExit(void);

extern "C" void port_lockAudio(void) {
    port_audioIntMaskEnter();
}

extern "C" void port_unlockAudio(void) {
    port_audioIntMaskExit();
}

extern "C" void port_audioIntMaskEnter(void) {
    const auto self = std::this_thread::get_id();
    std::unique_lock<std::mutex> lk(gAudioMutex);
    while (gAudioMaskHeld && gAudioMaskOwner != self) {
        if (gAudioCv.wait_for(lk, std::chrono::seconds(2)) == std::cv_status::timeout) {
#ifdef __WIIU__
            // Never hang silently again: say so and keep waiting.
            WHBLogPrintf("[audio] STUCK waiting on the interrupt mask, depth %d", gAudioMaskDepth);
#endif
        }
    }
    gAudioMaskHeld = true;
    gAudioMaskOwner = self;
    gAudioMaskDepth++;
}

extern "C" void port_audioIntMaskExit(void) {
    const auto self = std::this_thread::get_id();
    std::unique_lock<std::mutex> lk(gAudioMutex);
    if (gAudioMaskDepth == 0) {
        return; // restore without a matching mask - nothing to give back
    }
#ifdef __WIIU__
    if (gAudioMaskOwner != self) {
        WHBLogPrintf("[audio] cross-thread restore of the interrupt mask (depth %d)", gAudioMaskDepth);
    }
#endif
    if (--gAudioMaskDepth == 0) {
        gAudioMaskHeld = false;
        gAudioMaskOwner = std::thread::id();
        gAudioCv.notify_all();
    }
}

// OS_IM_NONE enters a critical section, a restored mask exits it. Returns 0 so the saved value
// the caller passes to the paired restore is non-OS_IM_NONE.
extern "C" OSIntMask osSetIntMask(OSIntMask mask) {
    if (mask == OS_IM_NONE) {
        port_audioIntMaskEnter();
    } else {
        port_audioIntMaskExit();
    }
    return 0;
}
