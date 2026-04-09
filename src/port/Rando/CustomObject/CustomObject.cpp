#include "CustomObject.h"
#include "port/Rando/Logic/Logic.h"
// #include "port/Rando/CheckTracker/CheckTracker.h"
#include "port/ui/Notification.h"
#include "port/enhancements/events/hooks/Events.h"

extern "C" {
void chjiggy_setJiggyId(Actor* thisx, u32 id);

// Custom Actor
s32 dummy_func_80320248(void);
extern s32 sSpawnableActorSize;
extern ActorSpawn* sSpawnableActorList;

enum map_e map_get(void);
enum level_e map_getLevel(enum map_e map);
}

typedef struct {
    actor_e actorId;
    int32_t collectionId;
    RandoItemType itemType;
    std::array<int32_t, 3> location;
    bool isSpawned;
} QueuedActor;

std::vector<QueuedActor> actorSpawnQueue;
int32_t currentLevel = -1;

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

QueuedActor CreateCustomQueuedActor(std::array<int32_t, 3> position) {
    QueuedActor queuedActor;
    queuedActor.actorId = ACTOR_1_UNKNOWN;
    
    RandoCheckId randoCheckId = Rando::StaticData::GetCheckByPosition(position);
    if (randoCheckId == RC_UNKNOWN) {
        return queuedActor;
    }

    Rando::StaticData::RandoShuffledPool shuffledCheck = Rando::Logic::GetShuffledObject(randoCheckId);

    if (shuffledCheck.randoCheckId != RC_UNKNOWN) {
        queuedActor.actorId = (actor_e)Rando::StaticData::Items[shuffledCheck.randoItemId].actorId;
        queuedActor.collectionId = shuffledCheck.randoCollectionId;
        queuedActor.itemType = Rando::StaticData::Items[shuffledCheck.randoItemId].randoItemType;
        queuedActor.location = position;
        queuedActor.isSpawned = false;
    }

    return queuedActor;
}

bool CustomObject::CheckSpawnQueue(int32_t posX, int32_t posY, int32_t posZ) {
    std::array<int32_t, 3> position = { posX, posY, posZ };
    bool foundQueuedActor = false;
    for (auto& spawn : actorSpawnQueue) {
        if (spawn.location == position) {
            foundQueuedActor = true;
            break;
        }
    }
    return foundQueuedActor;
}

void ClearSpawnQueue() {
    if (currentLevel != map_getLevel(map_get())) {
        currentLevel = map_getLevel(map_get());
        actorSpawnQueue.clear();
    }
}

void CustomObject::InitializeSpawnQueue() {
    ClearSpawnQueue();
    if (!actorSpawnQueue.empty()) {
        for (auto& spawn : actorSpawnQueue) {
            if (spawn.isSpawned) {
                continue;
            }

            int32_t spawnPos[3] = { spawn.location[0], spawn.location[1], spawn.location[2] };
            Actor* newActor = SpawnRandoActor(spawn.actorId, spawnPos);

            if (spawn.itemType == RITYPE_JIGGY) {
                chjiggy_setJiggyId(newActor, spawn.collectionId);
            }
            spawn.isSpawned = true;
        }
    }
}

void CustomObject::AddToSpawnQueue(int32_t posX, int32_t posY, int32_t posZ) {
    ClearSpawnQueue();
    for (auto& queue : actorSpawnQueue) {
        std::array<int32_t, 3> position = { posX, posY, posZ };
        if (queue.location == position) {
            return;
        }
    }

    QueuedActor queuedActor = CreateCustomQueuedActor({ posX, posY, posZ });
    if (queuedActor.actorId == ACTOR_1_UNKNOWN) {
        return;
    }

    actorSpawnQueue.push_back(queuedActor);
}
