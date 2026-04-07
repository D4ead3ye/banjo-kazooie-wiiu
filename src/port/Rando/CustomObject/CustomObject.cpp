#include "CustomObject.h"
// #include "port/Rando/Logic/Logic.h"
// #include "port/Rando/CheckTracker/CheckTracker.h"
#include "port/ui/Notification.h"
#include "port/enhancements/events/hooks/Events.h"

extern "C" {
#include "actor.h"
typedef struct chjiggy_s {
    u32 unk0;
    u32 index;
} ActorLocal_Jiggy;

// Custom Actor
s32 dummy_func_80320248(void);
extern s32 sSpawnableActorSize;
extern ActorSpawn* sSpawnableActorList;
extern ActorInfo D_803662A8;

extern void spawnableActorList_add(ActorInfo* arg0, Actor* (*arg1)(s32[3], s32, ActorInfo*, u32), u32 arg2);
extern Actor* actor_spawnWithYaw_s32(enum actor_e id, s32 (*pos)[3], s32 rot);

enum map_e map_get(void);
}

std::map<actor_e, std::array<int32_t, 3>> actorSpawnQueue;
static bool canSpawn = true;

void CustomObject::InitializeSpawnQueue() {
    if (!actorSpawnQueue.empty() && canSpawn) {
        canSpawn = false;
        for (auto& [id, position] : actorSpawnQueue) {
            int32_t spawnPos[3] = { position[0], position[1], position[2] };

            Actor* newActor = actor_spawnWithYaw_s32(id, &spawnPos, 0);
            ActorLocal_Jiggy* actorLocal = (ActorLocal_Jiggy*)&newActor->local;
            actorLocal->index = JIGGY_03_MM_MUMBOS_SKULL;
        }
        actorSpawnQueue.clear();
        canSpawn = true;
    }
}

Actor* SpawnRandoActor(actor_e actorId, int32_t posX, int32_t posY, int32_t posZ) {
    s32 pos[3];
    pos[0] = posX;
    pos[1] = posY;
    pos[2] = posZ;
    s32 i;
    actorId = (!dummy_func_80320248()) ? (ACTOR_4_BIGBUTT) : (actorId);
    for (i = 0; i < sSpawnableActorSize; i++) {
        if (actorId == sSpawnableActorList[i].infoPtr->actorId) {
            return sSpawnableActorList[i].spawnFunc(pos, 0, ((0, sSpawnableActorList[i])).infoPtr,
                                                    sSpawnableActorList[i].unk8);
        }
    }

    return NULL;
}

void CustomObject::AddToSpawnQueue(actor_e id, int32_t posX, int32_t posY, int32_t posZ) {
    std::array<int32_t, 3> position = { posX, posY, posZ };

    actorSpawnQueue.emplace(id, position);
}

Actor* CustomObject::SpawnRandoObject(actor_e actorId, int32_t posX, int32_t posY, int32_t posZ) {
    Actor* newActor = SpawnRandoActor(actorId, posX, posY, posZ);
    //ActorLocal_Jiggy* actorLocal = (ActorLocal_Jiggy*)&newActor->local;
    //actorLocal->index = JIGGY_03_MM_MUMBOS_SKULL;

    return newActor;
}
