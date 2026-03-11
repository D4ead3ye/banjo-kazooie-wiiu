#include "2.0L/PR/libaudio.h"

#include <ultra64.h>
// [port] N64 SDK audio library - stubbed for PC port
#if 0

void alCSPSetSeq(ALCSPlayer *seqp, ALCSeq *seq)
{
    ALEvent evt;

    evt.type = AL_SEQP_SEQ_EVT;
    evt.msg.spseq.seq = seq;

    alEvtqPostEvent(&seqp->evtq, &evt, 0);
}

#endif // [port] N64 SDK audio stub
