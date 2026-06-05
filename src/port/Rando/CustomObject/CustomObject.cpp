#include "CustomObject.h"
#include "port/Rando/Logic/Logic.h"
#include "port/enhancements/events/hooks/Events.h"

#include "spdlog/spdlog.h"

extern "C" {
extern u8 D_80385FF0[0xE];

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

extern u8 D_80383428[0x1C];
extern ActorMarker* D_8036E7C8;

void marker_despawn(ActorMarker* marker);
void item_inc(enum item_e item);
s32 item_adjustByDiffWithHud(enum item_e item, s32 diff);

void fxSparkle_musicNote(s16 position[3]);
void player_getPosition(f32 dst[3]);
void ml_vec3f_to_vec3h(s16 dst[3], f32 src[3]);
}

extern int32_t GetJinjoActorMarkerId(actor_e actorId);

int32_t currentMap = -1;
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
    return foundCustomActor;
}

bool CustomObject::CheckCustomActorMap(RandoCheckId randoCheckId) {
    bool foundCustomActor = false;
    for (auto& [checkId, actorData] : customActorMap) {
        if (checkId == randoCheckId) {
            foundCustomActor = true;
            break;
        }
    }
    return foundCustomActor;
}

void ClearSpawnQueue() {
    currentMap = gsworld_getMap();
    customActorMap.clear();
    actorSpawnQueue.clear();
}

CustomActor CreateCustomActor(RandoCheckId randoCheckId, int32_t position[3]) {
    CustomActor customActor;
    customActor.randoCheckId = randoCheckId;
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

void CustomObject::SpawnJinjoJiggy(int16_t levelId, int16_t position[3]) {
    RandoCheckId jiggyCheckId = Rando::StaticData::GetJinjoJiggyCheckByLevelId(levelId);

    int32_t spawnPosition[3];
    spawnPosition[0] = (int32_t)position[0];
    spawnPosition[1] = (int32_t)position[1];
    spawnPosition[2] = (int32_t)position[2];

    if (jiggyCheckId != RC_UNKNOWN) {
        Actor* actor;
        RandoSaveCheck shuffledObject = Rando::Logic::GetShuffledObject(jiggyCheckId);
        
        if (shuffledObject.randoCheckId != RC_UNKNOWN) {
            actor = CustomObject::SpawnCustomActor(
                (actor_e)Rando::StaticData::Items[shuffledObject.randoItemId].actorId, spawnPosition);
            if (actor != NULL) {
                actor = CustomObject::SetCustomActorParameters(actor, shuffledObject.randoCheckId);
                ApplyCustomActorPhysics(shuffledObject.randoCheckId, actor, true);
            }
        } else {
            actor = CustomObject::SpawnCustomActor((actor_e)Rando::StaticData::Checks[jiggyCheckId].actorId, spawnPosition);
            if (actor != NULL) {
                chjiggy_setJiggyId(actor, Rando::StaticData::Checks[jiggyCheckId].collectionId);
                ApplyCustomActorPhysics(jiggyCheckId, actor, true);
            }
        }
    }
}

void CustomObject::AddToCustomActorMap(RandoCheckId randoCheckId, Actor* actor) {
    Actor customActor = *actor;

    customActorMap.insert_or_assign(randoCheckId, *actor);
}

void CustomObject::AddToSpawnQueue(RandoCheckId randoCheckId, int32_t position[3]) {
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

        RandoSaveCheck shuffledObject = Rando::Logic::GetShuffledObject(customActor.randoCheckId);
        actor_e randoActorId = GetActorIdByShuffledObjectState(shuffledObject);

        if (randoActorId == ACTOR_1_UNKNOWN) {
            isSpawned = true;
            continue;
        }

        Actor* newCustomActor = CustomObject::SpawnCustomActor(randoActorId, customPosition);
        
        if (newCustomActor != NULL) {
            newCustomActor = CustomObject::SetCustomActorParameters(newCustomActor, customActor.randoCheckId);
            CustomObject::AddToCustomActorMap(customActor.randoCheckId, newCustomActor);
            isSpawned = true;
        }
    }
}

// uint8_t prevNoteScores[14];
// void PrintNoteTotals(std::string typeName) {
//     SPDLOG_INFO("{}", typeName.c_str());
//     for (int i = LEVEL_1_MUMBOS_MOUNTAIN; i <= LEVEL_A_MAD_MONSTER_MANSION; i++) {
//         std::string levelName = worldNameList[i - 1];
//         const char* status = (prevNoteScores[i] != D_80385FF0[i]) ? "[ X ]" : "[   ]";
// 
//         SPDLOG_INFO("{} - {} - {} -> {}",status, levelName.c_str(), std::to_string(prevNoteScores[i]).c_str(),
//                     std::to_string(D_80385FF0[i]).c_str());
//     }
//     for (int n = 0; n < 14; n++) {
//         prevNoteScores[n] = D_80385FF0[n];
//     }
// }

void CustomObject::ResolveCustomActorCollision(RandoCheckId randoCheckId) {
    if (randoCheckId == RC_UNKNOWN) {
        return;
    }

    RandoSaveCheck shuffledObject = Rando::Logic::GetShuffledObject(randoCheckId);

    int16_t playerPosI[3];
    f32 playerPosF[3];
    player_getPosition(playerPosF);
    ml_vec3f_to_vec3h(playerPosI, playerPosF);

    switch (shuffledObject.randoItemId) {
        case RI_JINJO_BLUE:
        case RI_JINJO_GREEN:
        case RI_JINJO_ORANGE:
        case RI_JINJO_PINK:
        case RI_JINJO_YELLOW:
            if (Rando::StaticData::Checks[shuffledObject.shuffledCheckId].worldId == map_getLevel(gsworld_getMap())) {
                int32_t jinjoMarkerId = GetJinjoActorMarkerId((actor_e)Rando::StaticData::Items[shuffledObject.randoItemId].actorId);
                item_adjustByDiffWithHud(ITEM_12_JINJOS, (1 << ((jinjoMarkerId + 6) & 0x1F)));
            } else {
                if (Rando::Logic::ShouldSpawnJinjoJiggy(
                        Rando::StaticData::Checks[shuffledObject.shuffledCheckId].worldId)) {
                    CustomObject::SpawnJinjoJiggy(Rando::StaticData::Checks[shuffledObject.shuffledCheckId].worldId, playerPosI);
                }
            }
            break;
        case RI_MUSIC_NOTE:
            D_80385FF0[Rando::StaticData::Checks[shuffledObject.shuffledCheckId].worldId]++;
            if (Rando::StaticData::Checks[shuffledObject.shuffledCheckId].worldId == map_getLevel(gsworld_getMap())) {
                item_set(ITEM_C_NOTE, D_80385FF0[map_getLevel(gsworld_getMap())]);
            }

            UpdateSaveDataNoteScores();
            fxSparkle_musicNote(playerPosI);
            break;
        default:
            break;
    }
}

void CustomObject::CheckObtained(RandoCheckId randoCheckId) {
    bool shouldRemove = false;

    for (auto& pool : Rando::Logic::shuffledPool) {
        if (pool.randoCheckId == randoCheckId && !pool.obtained) {
            pool.obtained = true;
            shouldRemove = true;
            RANDO_SAVE_CHECKS[pool.randoCheckId].obtained = true;
            BK_LOG_INFO("RandoCheckId %s collected!", Rando::StaticData::Checks[randoCheckId].name);
            Rando::StaticData::SendCollisionNotification(pool.randoItemId);
            Rando::StaticData::ModifyRandoInfFlagState(randoCheckId);
            break;
        }
    }

    if (shouldRemove) {
        CustomObject::ResolveCustomActorCollision(randoCheckId);
        if (RANDO_SAVE_CHECKS[randoCheckId].randoItemId == RI_MUSIC_NOTE && customActorMap.contains(randoCheckId)) {
            marker_despawn(customActorMap.at(randoCheckId).marker);
            customActorMap.erase(randoCheckId);
        }
        shouldRemove = false;
    }
}

void CustomObject::ObjectCollected(Prop* prop) {
    for (auto& [randoCheckId, customActor] : customActorMap) {
        if (customActor.position_x == prop->actorProp.x &&
            customActor.position_y == prop->actorProp.y &&
            customActor.position_z == prop->actorProp.z) {
            CustomObject::CheckObtained(randoCheckId);
            return;
        }
    }
}

