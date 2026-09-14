#pragma once

/*  Startup tracing for the Wii U build.

    Logs over UDP (port 4405), read on a PC. An OSScreen sink was used while
    UDP was still unproven; it is gone because its buffers have to be released
    before GX2Init and drawing to it afterwards is a crash waiting to happen.

    Also installs a terminate handler: "Abort called" on the console is
    usually std::terminate, and the exception message is the useful part.

    Enabled unconditionally while the port is being brought up.
*/
#ifdef __WIIU__

#include <whb/log.h>
#include <whb/log_udp.h>
#include <whb/crash.h>

#include <cstdlib>
#include <exception>

inline void WiiUTerminateHandler() {
    WHBLogPrintf("[lh] !!! std::terminate");
    if (std::exception_ptr active = std::current_exception()) {
        try {
            std::rethrow_exception(active);
        } catch (const std::exception& e) {
            WHBLogPrintf("[lh] uncaught exception: %s", e.what());
        } catch (...) {
            WHBLogPrintf("[lh] uncaught exception of non-standard type");
        }
    } else {
        WHBLogPrintf("[lh] terminate with no active exception");
    }
    // [port] abort() unwinds without going through the crash handler, so we got
    // the exception text and no call stack. Fault deliberately instead: that
    // hands control to WHBInitCrashHandler, which dumps registers and the back
    // chain - which is what actually locates the throw site.
    WHBLogPrintf("[lh] faulting deliberately to capture a backtrace");
    *(volatile uint32_t*)0 = 0;
    std::abort();
}

#define WIIU_TRACE_INIT()                        \
    do {                                         \
        WHBLogUdpInit();                         \
        WHBInitCrashHandler();                   \
        std::set_terminate(WiiUTerminateHandler); \
    } while (0)

#define WIIU_TRACE(...) WHBLogPrintf(__VA_ARGS__)

// [port] The high-frequency tier, compiled out unless asked for.
//
// A crash report from a ninety-minute session faulted inside newlib's locale
// lookup, reached from _svfprintf_r - which is where snprintf and friends end
// up. Whatever the root cause turns out to be, a release build has no business
// running printf every second forever: the once-a-second idle counter, the
// periodic scene stats, and the controller enumeration (which has been seen
// firing eight hundred times in three seconds) between them keep that path
// permanently busy.
//
// Build with -DLH_WIIU_HOT_TRACE=1 to get them back when actually chasing
// something. Everything that happens once a session - startup, foreground
// handovers, exit, the breadcrumb replay - stays on WIIU_TRACE and is
// unaffected.
#ifdef LH_WIIU_HOT_TRACE
#define WIIU_TRACE_HOT(...) WHBLogPrintf(__VA_ARGS__)
#else
#define WIIU_TRACE_HOT(...) ((void)0)
#endif

#else

#define WIIU_TRACE_INIT() ((void)0)
#define WIIU_TRACE(...) ((void)0)
#define WIIU_TRACE_HOT(...) ((void)0)

// Bring-up logging in the port layer calls WHBLogPrintf directly in a number of
// places. Those lines are Wii U diagnostics, not behaviour, so give the name a
// no-op off-console instead of wrapping every call site in a guard - which is
// what kept the tree from compiling for any other target.
#define WHBLogPrintf(...) ((void)0)

#endif
