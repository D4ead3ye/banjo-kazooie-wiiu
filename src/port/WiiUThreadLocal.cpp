#ifdef __WIIU__

#include "port/WiiUThreadLocal.h"

#include <coreinit/thread.h>
#include <whb/log.h>
#include <coreinit/memheap.h>
#include <coreinit/memexpheap.h>
#include <coreinit/memfrmheap.h>
#include <coreinit/memblockheap.h>
#include <coreinit/memunitheap.h>
#include <coreinit/memory.h>

namespace LighthouseWiiU {

// 14 and 15 are reserved by wut; 0-13 are free for applications.
static constexpr OSThreadSpecificID kThreadLocalSlot = OS_THREAD_SPECIFIC_13;

static void reportUnknownHeap(uint32_t tag) {
    static uint32_t sReported = 0;
    if (sReported != tag) {
        sReported = tag;
        WHBLogPrintf("[mem] MEM2 base heap tag %08X has no byte accessor - free is unknown, not zero", (unsigned)tag);
    }
}

uint32_t FreeMem2() {
    MEMHeapHandle heap = MEMGetBaseHeapHandle(MEM_BASE_HEAP_MEM2);
    if (heap == nullptr) {
        reportUnknownHeap(0);
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
        case MEM_UNIT_HEAP_TAG:
            // Unit heaps only count blocks, not bytes; the block size is not
            // exposed, so this is a count and the log says so.
            return MEMCountFreeBlockForUnitHeap(heap);
        default:
            // Still zero, but say why once. A bare 0 reads as exhaustion and is
            // worse than no number - the whole reason this was rewritten, and it
            // was still doing it because an unhandled kind lands here.
            reportUnknownHeap(heap->tag);
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
