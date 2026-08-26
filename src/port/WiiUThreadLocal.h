#pragma once
#ifdef __WIIU__

#include <cstdint>
#include <vector>

/*  The RPX format has no TLS segment - elf2rpl rejects the TPREL relocations
    that `thread_local` generates - so per-thread state uses one of the OS's
    sixteen thread-specific slots instead. Everything that needs to be
    per-thread lives in a single block hung off that slot.

    The block is allocated on first touch by each thread and is not freed;
    threads here are few and live for the length of the session.
*/
namespace LighthouseWiiU {

struct ThreadLocals {
    int audioMaskDepth = 0;
    bool isGameThread = false;
    int colorVariant = -1;
    std::vector<int16_t> scaledAudio;
};

ThreadLocals& Locals();

} // namespace LighthouseWiiU

#endif
