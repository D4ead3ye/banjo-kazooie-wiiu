#pragma once

/*  Startup tracing for the Wii U build.

    The console gives no console output, so progress is broadcast over UDP
    (port 4405) and read on a PC. Enabled unconditionally while the port is
    being brought up; it costs one UDP packet per checkpoint.
*/
#ifdef __WIIU__

#include <whb/log.h>
#include <whb/log_udp.h>
#include <whb/crash.h>

#define WIIU_TRACE_INIT()          \
    do {                           \
        WHBLogUdpInit();           \
        WHBInitCrashHandler();     \
    } while (0)

#define WIIU_TRACE(...) WHBLogPrintf(__VA_ARGS__)

#else

#define WIIU_TRACE_INIT() ((void)0)
#define WIIU_TRACE(...) ((void)0)

#endif
