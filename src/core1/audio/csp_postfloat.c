#include "2.0L/PR/libaudio.h"
#include "functions.h"
#include "variables.h"
#include <ultra64.h>
// [port] N64 SDK audio library - stubbed for PC port
#if 0

void func_8025F3F0(ALCSPlayer *seqp, f32 arg1, f32 arg2)
{
    ALEvent evt;
    
    evt.type = AL_UNK18_EVT; //event type not listed;
    evt.msg.unk18.unk0 = arg1;
    evt.msg.unk18.unk4 = arg2;
    alEvtqPostEvent(&seqp->evtq, &evt, 0);
}

#endif // [port] N64 SDK audio stub
