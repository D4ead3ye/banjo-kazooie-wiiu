#include "CustomObject.h"
#include "port/Rando/Logic/Logic.h"
// #include "port/Rando/CheckTracker/CheckTracker.h"
#include "port/enhancements/events/hooks/Events.h"

extern "C" {
void chjiggy_setJiggyId(Actor* thisx, u32 id);

typedef struct {
    enum mumbotoken_e uid;
} ActorLocal_MumboToken;

typedef struct {
    enum honeycomb_e uid;
    s32 unk4;
} ActorLocal_EmptyHoneycomb;

// Custom Actor
s32 dummy_func_80320248(void);
extern s32 sSpawnableActorSize;
extern ActorSpawn* sSpawnableActorList;

enum map_e map_get(void);
enum level_e map_getLevel(enum map_e map);
}

typedef struct {
    RandoCheckId randoCheckId;
    actor_e actorId;
    int32_t collectionId;
    RandoItemType itemType;
    std::array<int32_t, 3> location;
    bool isSpawned;
} QueuedActor;

std::map<RandoCheckId, ActorProp> randoActorMap;
std::vector<QueuedActor> actorSpawnQueue;
int32_t currentLevel = -1;

Actor* CustomObject::SpawnRandoActor(actor_e actorId, int32_t position[3]) {
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

Actor* SetCustomObjectParameters(Actor* actor, QueuedActor actorData) {
    switch (actorData.itemType) {
        case RITYPE_EMPTY_HONEYCOMB:
            ActorLocal_EmptyHoneycomb* honeycombLocal;
            honeycombLocal = (ActorLocal_EmptyHoneycomb*)&actor->local;
            honeycombLocal->uid = (honeycomb_e)actorData.collectionId;
            break;
        case RITYPE_JIGGY:
            chjiggy_setJiggyId(actor, actorData.collectionId);
            break;
        case RITYPE_MUMBO_TOKEN:
            ActorLocal_MumboToken* tokenLocal;
            tokenLocal = (ActorLocal_MumboToken*)&actor->local;
            tokenLocal->uid = (mumbotoken_e)actorData.collectionId;
            break;
        default:
            break;
    }

    return actor;
}

QueuedActor CreateCustomQueuedActor(RandoCheckId randoCheckId, std::array<int32_t, 3> position) {
    QueuedActor queuedActor;
    queuedActor.actorId = ACTOR_1_UNKNOWN;
    
    if (randoCheckId == RC_UNKNOWN) {
        return queuedActor;
    }

    Rando::StaticData::RandoShuffledPool shuffledCheck = Rando::Logic::GetShuffledObject(randoCheckId);

    if (shuffledCheck.randoCheckId != RC_UNKNOWN) {
        queuedActor.randoCheckId = randoCheckId;
        queuedActor.actorId = (actor_e)Rando::StaticData::Items[shuffledCheck.randoItemId].actorId;
        queuedActor.collectionId = shuffledCheck.randoCollectionId;
        queuedActor.itemType = Rando::StaticData::Items[shuffledCheck.randoItemId].randoItemType;
        queuedActor.location = position;
        queuedActor.isSpawned = false;
    }

    return queuedActor;
}

bool CustomObject::CheckSpawnQueue(RandoCheckId randoCheckId) {
    bool foundQueuedActor = false;
    for (auto& spawn : actorSpawnQueue) {
        if (spawn.randoCheckId == randoCheckId) {
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

void CustomObject::AddToRandoActorMap(RandoCheckId randoCheckId, Actor* actor) {
    ActorProp actorProperty = *actor->marker->propPtr;
    randoActorMap.emplace(randoCheckId, actorProperty);
}

void CustomObject::InitializeSpawnQueue() {
    ClearSpawnQueue();
    if (!actorSpawnQueue.empty()) {
        for (auto& spawn : actorSpawnQueue) {
            if (spawn.isSpawned) {
                continue;
            }

            int32_t spawnPos[3] = { spawn.location[0], spawn.location[1], spawn.location[2] };
            Actor* newActor = CustomObject::SpawnRandoActor(spawn.actorId, spawnPos);

            newActor = SetCustomObjectParameters(newActor, spawn);
            spawn.isSpawned = true;
            AddToRandoActorMap(spawn.randoCheckId, newActor);
        }
    }
}

void CustomObject::AddToSpawnQueue(RandoCheckId randoCheckId, int32_t posX, int32_t posY, int32_t posZ) {
    ClearSpawnQueue();
    for (auto& queue : actorSpawnQueue) {
        if (queue.randoCheckId == randoCheckId) {
            return;
        }
    }

    QueuedActor queuedActor = CreateCustomQueuedActor(randoCheckId, { posX, posY, posZ });
    if (queuedActor.actorId == ACTOR_1_UNKNOWN) {
        return;
    }

    actorSpawnQueue.push_back(queuedActor);
}

void CustomObject::ObjectCollected(Prop* prop) {
    for (auto& [randoCheckId, customActor] : randoActorMap) {
        if (customActor.words[0] == prop->actorProp.words[0]) {
            for (auto& pool : Rando::Logic::shuffledPool) {
                if (pool.randoCheckId == randoCheckId && !pool.obtained) {
                    pool.obtained = true;
                    BK_LOG_INFO("RandoCheckId %s collected!", Rando::StaticData::Checks[randoCheckId].name);
                    return;
                }
            }
            return;
        }
    }
}