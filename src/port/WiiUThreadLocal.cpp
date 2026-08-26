#ifdef __WIIU__

#include "port/WiiUThreadLocal.h"

#include <coreinit/thread.h>

namespace LighthouseWiiU {

// 14 and 15 are reserved by wut; 0-13 are free for applications.
static constexpr OSThreadSpecificID kThreadLocalSlot = OS_THREAD_SPECIFIC_13;

ThreadLocals& Locals() {
    auto* locals = static_cast<ThreadLocals*>(OSGetThreadSpecific(kThreadLocalSlot));
    if (locals == nullptr) {
        locals = new ThreadLocals();
        OSSetThreadSpecific(kThreadLocalSlot, locals);
    }
    return *locals;
}

} // namespace LighthouseWiiU

#endif
