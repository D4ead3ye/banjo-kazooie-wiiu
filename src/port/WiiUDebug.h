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
    std::abort();
}

#define WIIU_TRACE_INIT()                        \
    do {                                         \
        WHBLogUdpInit();                         \
        WHBInitCrashHandler();                   \
        std::set_terminate(WiiUTerminateHandler); \
    } while (0)

#define WIIU_TRACE(...) WHBLogPrintf(__VA_ARGS__)

#else

#define WIIU_TRACE_INIT() ((void)0)
#define WIIU_TRACE(...) ((void)0)

#endif
