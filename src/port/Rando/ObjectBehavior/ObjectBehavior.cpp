#include "ObjectBehavior.h"
#include <libultraship/bridge/consolevariablebridge.h>
// #include "port/Rando/Logic/Logic.h"
// #include "port/hooks/list/PlayerEvent.h"
#include "port/enhancements/events/hooks/Events.h"

extern "C" {
Actor* actor_spawnWithYaw_s32(enum actor_e id, s32 (*pos)[3], s32 rot);

typedef struct chjiggy_s {
    u32 unk0;
    u32 index;
} ActorLocal_Jiggy;


// Custom Item
s32 dummy_func_80320248(void);
extern s32 sSpawnableActorSize;
extern ActorSpawn* sSpawnableActorList;
}

Actor* SpawnRandoActor(enum actor_e arg0, int32_t posX, int32_t posY, int32_t posZ, s32 rot) {
    s32 pos[3];
    pos[0] = posX;
    pos[1] = posY;
    pos[2] = posZ;
    s32 i;
    arg0 = (!dummy_func_80320248()) ? (ACTOR_4_BIGBUTT) : (arg0);
    for (i = 0; i < sSpawnableActorSize; i++) {
        if (arg0 == sSpawnableActorList[i].infoPtr->actorId) {
            return sSpawnableActorList[i].spawnFunc(pos, rot, ((0, sSpawnableActorList[i])).infoPtr,
                                                    sSpawnableActorList[i].unk8);
        }
    }

    return NULL;
}

// Entry point for the module, run once on game boot
void Rando::ObjectBehavior::Init() {
    REGISTER_LISTENER(OnActorSpawn, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnActorSpawn* ev = (OnActorSpawn*)event;

        if (!IS_RANDO) {
            return;
        }

        if (ev->arg == ACTOR_61_JINJO_PINK) {
            event->cancelled = true;
            Actor* newActor = SpawnRandoActor(ACTOR_46_JIGGY, ev->posX, ev->posY, ev->posZ, 0);
            ActorLocal_Jiggy* actorLocal = (ActorLocal_Jiggy*)&newActor->local;
            actorLocal->index = JIGGY_03_MM_MUMBOS_SKULL;
        }
    })
}
