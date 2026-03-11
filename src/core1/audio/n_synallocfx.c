#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "n_synth.h"
// [port] N64 SDK audio library - stubbed for PC port
#if 0

ALFxRef n_alSynAllocFX( s16 bus, ALSynConfig *c, ALHeap *hp)
{
    n_alFxNew(&n_syn->auxBus->fx_array[bus], c, hp);
    return(n_syn->auxBus->fx_array[bus]);
}

#endif // [port] N64 SDK audio stub
