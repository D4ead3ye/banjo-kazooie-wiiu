#include "CustomObject.h"
// #include "port/Rando/Logic/Logic.h"
// #include "port/Rando/CheckTracker/CheckTracker.h"
#include "port/ui/Notification.h"
#include "port/enhancements/events/hooks/Events.h"

extern "C" {
typedef struct chjiggy_s {
    u32 unk0;
    u32 index;
} ActorLocal_Jiggy;

// Custom Item
s32 dummy_func_80320248(void);
extern s32 sSpawnableActorSize;
extern ActorSpawn* sSpawnableActorList;
}

Actor* SpawnRandoActor(int32_t actorId, int32_t posX, int32_t posY, int32_t posZ, int32_t rot) {
    s32 pos[3];
    pos[0] = posX;
    pos[1] = posY;
    pos[2] = posZ;
    s32 i;
    actorId = (!dummy_func_80320248()) ? (ACTOR_4_BIGBUTT) : (actorId);
    for (i = 0; i < sSpawnableActorSize; i++) {
        if (actorId == sSpawnableActorList[i].infoPtr->actorId) {
            return sSpawnableActorList[i].spawnFunc(pos, rot, ((0, sSpawnableActorList[i])).infoPtr,
                                                    sSpawnableActorList[i].unk8);
        }
    }

    return NULL;
}


void CustomObject::SpawnObject(int32_t actorId, int32_t posX, int32_t posY, int32_t posZ, int32_t rot) {
    Actor* newActor = SpawnRandoActor(actorId, posX, posY, posZ, rot);
    ActorLocal_Jiggy* actorLocal = (ActorLocal_Jiggy*)&newActor->local;
    actorLocal->index = JIGGY_03_MM_MUMBOS_SKULL;
}
