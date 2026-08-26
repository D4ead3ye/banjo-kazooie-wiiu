#pragma once

/*  Startup tracing for the Wii U build.

    Two sinks, because either can fail on its own:
      - UDP (port 4405), read on a PC, which needs the network to cooperate
      - the console's own screen via OSScreen, which needs nothing

    The on-screen sink has to be torn down before GX2 initialises, since both
    want the scan buffers. gfx_wiiu.cpp calls WIIU_TRACE_SCREEN_OFF() for that.

    Enabled unconditionally while the port is being brought up.
*/
#ifdef __WIIU__

#include <whb/log.h>
#include <whb/log_udp.h>
#include <whb/log_console.h>
#include <whb/crash.h>

#define WIIU_TRACE_INIT()                   do {                                        WHBLogUdpInit();                        WHBLogConsoleInit();                    WHBLogConsoleSetColor(0x0A3D1400);         WHBInitCrashHandler();              } while (0)

#define WIIU_TRACE(...)          do {                             WHBLogPrintf(__VA_ARGS__);         WHBLogConsoleDraw();     } while (0)

#else

#define WIIU_TRACE_INIT() ((void)0)
#define WIIU_TRACE(...) ((void)0)

#endif
