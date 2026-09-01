#ifdef __WIIU__

#include "port/WiiUThreadLocal.h"

#include <coreinit/thread.h>
#include <whb/log.h>
#include <coreinit/memheap.h>
#include <coreinit/memexpheap.h>
#include <coreinit/memfrmheap.h>
#include <coreinit/memblockheap.h>
#include <coreinit/memory.h>

namespace LighthouseWiiU {

// 14 and 15 are reserved by wut; 0-13 are free for applications.
static constexpr OSThreadSpecificID kThreadLocalSlot = OS_THREAD_SPECIFIC_13;

uint32_t FreeMem2() {
    MEMHeapHandle heap = MEMGetBaseHeapHandle(MEM_BASE_HEAP_MEM2);
    if (heap == nullptr) {
        return 0;
    }
    // [port] The accessors are per heap kind and the wrong one just returns 0 -
    // which reads as "out of memory" and is worse than printing nothing, because
    // it sends you looking for an exhaustion that is not happening. This was
    // reporting free=0 on every boot while allocations right after it succeeded.
    // Dispatch on the heap's own tag.
    switch (heap->tag) {
        case MEM_EXPANDED_HEAP_TAG:
            return MEMGetTotalFreeSizeForExpHeap(heap);
        case MEM_FRAME_HEAP_TAG:
            return MEMGetAllocatableSizeForFrmHeapEx(heap, 4);
        case MEM_BLOCK_HEAP_TAG:
            return MEMGetTotalFreeSizeForBlockHeap(heap);
        default:
            return 0;
    }
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
