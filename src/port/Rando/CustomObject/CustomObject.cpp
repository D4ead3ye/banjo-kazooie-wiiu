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

s32 dummy_func_80320248(void);
extern s32 sSpawnableActorSize;
extern ActorSpawn* sSpawnableActorList;

enum map_e gsworld_getMap(void);
enum level_e map_getLevel(enum map_e map);
}

int32_t currentLevel = -1;
std::map<RandoCheckId, Actor> customActorMap;
std::vector<std::pair<CustomActor, bool>> actorSpawnQueue;

bool CustomObject::CheckSpawnQueue(RandoCheckId randoCheckId) {
    bool foundCustomActor = false;
    for (auto& [actorData, spawned] : actorSpawnQueue) {
        if (actorData.randoCheckId == randoCheckId) {
            foundCustomActor = true;
            break;
        }
    }
    return false;
}

void ClearSpawnQueue() {
    if (currentLevel != map_getLevel(gsworld_getMap())) {
        currentLevel = map_getLevel(gsworld_getMap());
        actorSpawnQueue.clear();
        customActorMap.clear();
    }
}

CustomActor CreateCustomActor(RandoCheckId randoCheckId, int32_t position[3]) {
    CustomActor customActor;
    customActor.randoCheckId = Rando::Logic::GetShuffledObject(randoCheckId).randoCheckId;
    customActor.location[0] = position[0];
    customActor.location[1] = position[1];
    customActor.location[2] = position[2];

    return customActor;
}

Actor* CustomObject::SetCustomActorParameters(Actor* actor, RandoCheckId randoCheckId) {
    switch (Rando::Logic::GetShuffledObject(randoCheckId).randoItemId) {
        case RI_EMPTY_HONEYCOMB:
            ActorLocal_EmptyHoneycomb* honeycombLocal;
            honeycombLocal = (ActorLocal_EmptyHoneycomb*)&actor->local;
            honeycombLocal->uid = (honeycomb_e)Rando::Logic::GetShuffledObject(randoCheckId).randoCollectionId;
            break;
        case RI_JIGGY:
            chjiggy_setJiggyId(actor, Rando::Logic::GetShuffledObject(randoCheckId).randoCollectionId);
            break;
        case RI_MUMBO_TOKEN:
            ActorLocal_MumboToken* tokenLocal;
            tokenLocal = (ActorLocal_MumboToken*)&actor->local;
            tokenLocal->uid = (mumbotoken_e)Rando::Logic::GetShuffledObject(randoCheckId).randoCollectionId;
            break;
        default:
            break;
    }

    return actor;
}

Actor* CustomObject::SpawnCustomActor(actor_e actorId, int32_t position[3]) {
    actorId = (!dummy_func_80320248()) ? (ACTOR_4_BIGBUTT) : (actorId);
    for (int i = 0; i < sSpawnableActorSize; i++) {
        if (actorId == sSpawnableActorList[i].infoPtr->actorId) {
            return sSpawnableActorList[i].spawnFunc(position, 0, ((0, sSpawnableActorList[i])).infoPtr,
                                                    sSpawnableActorList[i].unk8);
        }
    }

    return NULL;
}

Actor* CustomObject::GetCustomActor(RandoCheckId randoCheckId) {
    for (auto& [checkId, customActor] : customActorMap) {
        if (checkId == randoCheckId) {
            return &customActor;
        }
    }
    return NULL;
}

void CustomObject::AddToCustomActorMap(RandoCheckId randoCheckId, Actor* actor) {
    Actor customActor = *actor;
    customActorMap.emplace(randoCheckId, customActor);
}

void CustomObject::AddToSpawnQueue(RandoCheckId randoCheckId, int32_t position[3]) {
    ClearSpawnQueue();
    if (randoCheckId == RC_UNKNOWN) {
        return;
    }

    for (auto& [actorData, spawned] : actorSpawnQueue) {
        if (actorData.randoCheckId == randoCheckId) {
            return;
        }
    }

    CustomActor customActor = CreateCustomActor(randoCheckId, position);

    actorSpawnQueue.push_back({ customActor, false });
}

void CustomObject::InitializeSpawnQueue() {
    ClearSpawnQueue();
    if (actorSpawnQueue.empty()) {
        return;
    }

    for (auto& [customActor, isSpawned] : actorSpawnQueue) {
        if (isSpawned) {
            continue;
        }
        int32_t customPosition[3];
        customPosition[0] = customActor.location[0];
        customPosition[1] = customActor.location[1];
        customPosition[2] = customActor.location[2];

        Rando::StaticData::RandoShuffledPool shuffledObject = Rando::Logic::GetShuffledObject(customActor.randoCheckId);

        Actor* newCustomActor = CustomObject::SpawnCustomActor((actor_e)Rando::StaticData::Items[shuffledObject.randoItemId].actorId, customPosition);
        
        if (newCustomActor != NULL) {
            newCustomActor = CustomObject::SetCustomActorParameters(newCustomActor, customActor.randoCheckId);
            CustomObject::AddToCustomActorMap(customActor.randoCheckId, newCustomActor);
            isSpawned = true;
        }
    }
}

void CustomObject::CheckObtained(RandoCheckId randoCheckId) {
    for (auto& pool : Rando::Logic::shuffledPool) {
        if (pool.randoCheckId == randoCheckId && !pool.obtained) {
            pool.obtained = true;
            BK_LOG_INFO("RandoCheckId %s collected!", Rando::StaticData::Checks[randoCheckId].name);
            return;
        }
    }
}

void CustomObject::ObjectCollected(Prop* prop) {
    for (auto& [randoCheckId, customActor] : customActorMap) {
        if (customActor.marker->propPtr->words[0] == prop->actorProp.words[0]) {
            CustomObject::CheckObtained(randoCheckId);
            return;
        }
    }
}

