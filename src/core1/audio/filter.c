#include <ultra64.h>
#include "synthInternals.h"
// [port] N64 SDK audio library - stubbed for PC port
#if 0

void alFilterNew(ALFilter *f, ALCmdHandler h, ALSetParam s, s32 type)
{
    f->source    = 0;
    f->handler   = h;
    f->setParam  = s;
    f->inp       = 0;
    f->outp      = 0;
    f->type      = type;
}

#endif // [port] N64 SDK audio stub
