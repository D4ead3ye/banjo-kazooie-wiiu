#include <PR/ultraerror.h>
#include "synthInternals.h"
#include <ultra64.h>
#ifdef __WIIU__
#include <whb/log.h>
#endif

void *alHeapDBAlloc(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size)
{
    s32 bytes;
    u8 *ptr = 0;

    bytes = (num*size + AL_CACHE_ALIGN) & ~AL_CACHE_ALIGN;

#ifdef __WIIU__
    if ((hp->cur + bytes) > (hp->base + hp->len)) {
        WHBLogPrintf("[alheap] EXHAUSTED: want %d (%dx%d), used %d of %d", (int)bytes, (int)num, (int)size,
                     (int)(hp->cur - hp->base), (int)hp->len);
    }
#endif
    
#ifdef _DEBUG
    hp->count++;    
    bytes += sizeof(HeapInfo);
#endif
    
    if ((hp->cur + bytes) <= (hp->base + hp->len)) {

        ptr = hp->cur;
        hp->cur += bytes;

#ifdef _DEBUG    
        ((HeapInfo *)ptr)->magic = AL_HEAP_MAGIC;
        ((HeapInfo *)ptr)->size  = bytes;
        ((HeapInfo *)ptr)->count = hp->count;
        if (file) {
            ((HeapInfo *)ptr)->file  = file;
            ((HeapInfo *)ptr)->line  = line;
        } else {
            ((HeapInfo *)ptr)->file  = (u8 *) "unknown";
            ((HeapInfo *)ptr)->line  = 0;
        }
        
        ptr += sizeof(HeapInfo);        
#endif

    } else {
#ifdef _DEBUG
        __osError(ERR_ALHEAPNOFREE, 1, size);
#endif        
    }

    return ptr;
}