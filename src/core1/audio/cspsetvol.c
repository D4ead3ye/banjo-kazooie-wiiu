#include <ultra64.h>
#include "functions.h"
#include "variables.h"

#include "2.0L/PR/libaudio.h"
// [port] N64 SDK audio library - stubbed for PC port
#if 0

void alCSPSetVol(ALCSPlayer *seqp, s16 vol)
{
    ALEvent       evt;

    evt.type            = AL_SEQP_VOL_EVT;
    evt.msg.spvol.vol   = vol;
    
    alEvtqPostEvent(&seqp->evtq, &evt, 0);
}


#endif // [port] N64 SDK audio stub
