#include "2.0L/PR/libaudio.h"
#include "functions.h"
#include "variables.h"
#include <ultra64.h>
// [port] N64 SDK audio library - stubbed for PC port
#if 0

void alCSPPlay(ALCSPlayer *seqp)
{
    ALEvent evt;
    
    evt.type = AL_SEQP_PLAY_EVT;                    
    alEvtqPostEvent(&seqp->evtq, &evt, 0);
}

#endif // [port] N64 SDK audio stub
