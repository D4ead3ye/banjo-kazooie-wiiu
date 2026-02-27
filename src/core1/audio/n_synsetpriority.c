#include <ultra64.h>
#include <PR/n_libaudio.h>

void n_alSynSetPriority(N_ALVoice *voice, s16 priority){
    voice->priority = priority;
}
