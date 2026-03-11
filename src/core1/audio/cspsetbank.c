#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "2.0L/PR/libaudio.h"
// [port] N64 SDK audio library - stubbed for PC port
#if 0

void alCSPSetBank(ALCSPlayer *seqp, ALBank *b)
{
    ALEvent evt;

    evt.type = AL_SEQP_BANK_EVT;
    evt.msg.spbank.bank = b;

    alEvtqPostEvent(&seqp->evtq, &evt, 0);
}

#endif // [port] N64 SDK audio stub
