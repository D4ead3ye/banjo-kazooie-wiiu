#ifdef __WIIU__

#include "port/WiiUThreadLocal.h"

#include <coreinit/thread.h>
#include <whb/log.h>
#include <coreinit/memheap.h>
#include <coreinit/memexpheap.h>
#include <coreinit/memory.h>

namespace LighthouseWiiU {

// 14 and 15 are reserved by wut; 0-13 are free for applications.
static constexpr OSThreadSpecificID kThreadLocalSlot = OS_THREAD_SPECIFIC_13;

uint32_t FreeMem2() {
    MEMHeapHandle heap = MEMGetBaseHeapHandle(MEM_BASE_HEAP_MEM2);
    if (heap == nullptr) {
        return 0;
    }
    return MEMGetAllocatableSizeForExpHeapEx(heap, 4);
}

ThreadLocals& Locals() {
    auto* locals = static_cast<ThreadLocals*>(OSGetThreadSpecific(kThreadLocalSlot));
    if (locals == nullptr) {
        WHBLogPrintf("[tls] allocating block for thread %p, mem2 free=%u", (void*)OSGetCurrentThread(), (unsigned)FreeMem2());
        locals = new ThreadLocals();
        OSSetThreadSpecific(kThreadLocalSlot, locals);
        WHBLogPrintf("[tls] block %p installed", (void*)locals);
    }
    return *locals;
}

} // namespace LighthouseWiiU

#endif
