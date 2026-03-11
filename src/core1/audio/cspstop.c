#include <ultra64.h>
#include "2.0L/PR/libaudio.h"
// [port] N64 SDK audio library - stubbed for PC port
#if 0

void alCSPStop(ALCSPlayer *seqp)
{
    ALEvent     evt;

    evt.type = AL_SEQP_STOPPING_EVT;                    
    alEvtqPostEvent(&seqp->evtq, &evt, 0);
}

#endif // [port] N64 SDK audio stub
