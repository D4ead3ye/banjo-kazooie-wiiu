#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "2.0L/PR/libaudio.h"
// [port] N64 SDK audio library - stubbed for PC port
#if 0

s32 alCSPGetTempo(ALCSPlayer *seqp){
    if(seqp->target == NULL)
        return 0;
    return seqp->uspt / seqp->target->qnpt;
}


#endif // [port] N64 SDK audio stub
