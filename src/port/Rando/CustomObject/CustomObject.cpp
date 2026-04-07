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
}

typedef struct {
    actor_e actorId;
    int32_t collectionId;
    RandoItemType itemType;
    std::array<int32_t, 3> location;
    bool isSpawned;
} QueuedActor;

std::vector<QueuedActor> actorSpawnQueueEX;

Actor* SpawnRandoActor(actor_e actorId, int32_t position[3]) {
    s32 i;
    actorId = (!dummy_func_80320248()) ? (ACTOR_4_BIGBUTT) : (actorId);
    for (i = 0; i < sSpawnableActorSize; i++) {
        if (actorId == sSpawnableActorList[i].infoPtr->actorId) {
            return sSpawnableActorList[i].spawnFunc(position, 0, ((0, sSpawnableActorList[i])).infoPtr,
                                                    sSpawnableActorList[i].unk8);
        }
    }

    return NULL;
}

void CustomObject::InitializeSpawnQueue() {
    if (!actorSpawnQueueEX.empty()) {
        for (auto& spawn : actorSpawnQueueEX) {
            if (spawn.isSpawned) {
                continue;
            }

            int32_t spawnPos[3] = { spawn.location[0], spawn.location[1], spawn.location[2] };
            Actor* newActor = SpawnRandoActor(spawn.actorId, spawnPos);

            if (spawn.itemType == RITYPE_JIGGY) {
                ActorLocal_Jiggy* actorLocal = (ActorLocal_Jiggy*)&newActor->local;
                actorLocal->index = spawn.collectionId;
            }
            spawn.isSpawned = true;
        }
    }
}

void CustomObject::AddToSpawnQueue(actor_e id, int32_t collection, RandoItemType type, int32_t posX, int32_t posY, int32_t posZ) {
    for (auto& queue : actorSpawnQueueEX) {
        std::array<int32_t, 3> position = { posX, posY, posZ };
        if (queue.location == position) {
            return;
        }
    }

    QueuedActor queuedActor;
    queuedActor.actorId = id;
    queuedActor.collectionId = collection;
    queuedActor.itemType = type;
    queuedActor.location = { posX, posY, posZ };
    queuedActor.isSpawned = false;

    actorSpawnQueueEX.push_back(queuedActor);
}
