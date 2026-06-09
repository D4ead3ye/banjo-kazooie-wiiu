#include "CustomObject.h"
#include "port/Rando/Logic/Logic.h"
#include "port/enhancements/events/hooks/Events.h"

#include "actor.h"

#include "spdlog/spdlog.h"

extern "C" {
extern u8 D_80385FF0[0xE];

typedef struct chjiggy_s {
    u32 unk0;
    u32 index;
} ActorLocal_Jiggy;

typedef struct {
    enum mumbotoken_e uid;
} ActorLocal_MumboToken;

typedef struct {
    enum honeycomb_e uid;
    s32 unk4;
} ActorLocal_EmptyHoneycomb;

s32 item_adjustByDiffWithHud(enum item_e item, s32 diff);
void fxSparkle_musicNote(s16 position[3]);
void player_getPosition(f32 dst[3]);
void ml_vec3f_to_vec3h(s16 dst[3], f32 src[3]);

Actor* actor_new(s32 position[3], s32 yaw, ActorInfo* actorInfo, u32 flags);
extern ActorInfo chJinjoBlue;
extern ActorInfo chJinjoGreen;
extern ActorInfo chJinjoYellow;
extern ActorInfo chJinjoPink;
extern ActorInfo chJinjoOrange;
extern ActorInfo chJiggy;
extern ActorInfo D_80366C80;
extern ActorInfo D_803685A0;
extern ActorInfo sumusicNote;

extern ActorInfo D_80366CA4;
extern ActorInfo D_80367D00;
extern ActorInfo D_80367D24;
extern ActorInfo D_80367D48;
}

typedef struct {
    int32_t position[3];
    RandoCheckId randoCheckId;
    Actor randoActor;
} RandoActorExtension;

typedef struct {
    int32_t position[3];
    RandoCheckId randoCheckId;
    bool isSpawned;
} QueuedRandoProp;

std::vector<RandoActorExtension> randoActorList;
std::vector<QueuedRandoProp> randoActorQueue;
bool shouldRemoveEX = false;

// clang-format off
std::map<actor_e, std::pair<ActorInfo, int32_t>> actorInfoMap = {
    { ACTOR_2D_MUMBO_TOKEN,     { D_803685A0,       ACTOR_FLAG_UNKNOWN_6 } },
    { ACTOR_46_JIGGY,           { chJiggy,          ACTOR_FLAG_UNKNOWN_6 | ACTOR_FLAG_UNKNOWN_7 | ACTOR_FLAG_UNKNOWN_21 } },
    { ACTOR_47_EMPTY_HONEYCOMB, { D_80366C80,       ACTOR_FLAG_UNKNOWN_6 | ACTOR_FLAG_UNKNOWN_21 } },
    { ACTOR_51_MUSIC_NOTE,      { sumusicNote,      ACTOR_FLAG_UNKNOWN_21 } },
    { ACTOR_5E_JINJO_YELLOW,    { chJinjoYellow,    ACTOR_FLAG_UNKNOWN_6 | ACTOR_FLAG_UNKNOWN_8 } },
    { ACTOR_5F_JINJO_ORANGE,    { chJinjoOrange,    ACTOR_FLAG_UNKNOWN_6 | ACTOR_FLAG_UNKNOWN_8 } },
    { ACTOR_60_JINJO_BLUE,      { chJinjoBlue,      ACTOR_FLAG_UNKNOWN_6 | ACTOR_FLAG_UNKNOWN_8 } },
    { ACTOR_61_JINJO_PINK,      { chJinjoPink,      ACTOR_FLAG_UNKNOWN_6 | ACTOR_FLAG_UNKNOWN_8 } },
    { ACTOR_62_JINJO_GREEN,     { chJinjoGreen,     ACTOR_FLAG_UNKNOWN_6 | ACTOR_FLAG_UNKNOWN_8 } },
    { ACTOR_50_HONEYCOMB,       { D_80366CA4,       ACTOR_FLAG_UNKNOWN_6 | ACTOR_FLAG_UNKNOWN_21 } },
    { ACTOR_52_BLUE_EGG,        { D_80367D00,       ACTOR_FLAG_UNKNOWN_21 } },
    { ACTOR_129_RED_FEATHER,    { D_80367D24,       ACTOR_FLAG_UNKNOWN_21 } },
    { ACTOR_370_GOLD_FEATHER,   { D_80367D48,       ACTOR_FLAG_UNKNOWN_21 } },
};
// clang-format on

extern int32_t GetJinjoActorMarkerId(actor_e actorId);
int32_t currentMap = -1;

bool CheckRandoActorListEX(RandoCheckId randoCheckId) {
    for (auto& customActor : randoActorList) {
        if (customActor.randoCheckId == randoCheckId) {
            return true;
        }
    }
    return false;
}

void CustomObject::ClearRandoActorListEX() {
    if (currentMap != gsworld_getMap()) {
        currentMap = gsworld_getMap();
        randoActorList.clear();
        randoActorQueue.clear();
        SPDLOG_INFO("RandoActorList Cleared");
    }
}

Actor* GetCustomActorByCheckEX(RandoCheckId randoCheckId) {
    for (auto& customActor : randoActorList) {
        if (customActor.randoCheckId == randoCheckId) {
            return &customActor.randoActor;
        }
    }

    return NULL;
}

Actor* CustomObject::SetCustomActorParametersEX(RandoCheckId randoCheckId, Actor* customActor) {
    RandoSaveCheck shuffledObject = Rando::Logic::GetShuffledObject(randoCheckId);

    switch (shuffledObject.randoItemId) {
        case RI_EMPTY_HONEYCOMB:
            ActorLocal_EmptyHoneycomb* honeycombLocal;
            honeycombLocal = (ActorLocal_EmptyHoneycomb*)&customActor->local;
            honeycombLocal->uid = (honeycomb_e)Rando::Logic::GetShuffledObject(randoCheckId).randoCollectionId;
            break;
        case RI_JIGGY:
            ActorLocal_Jiggy* jiggyLocal;
            jiggyLocal = (ActorLocal_Jiggy*)&customActor->local;
            jiggyLocal->index = shuffledObject.randoCollectionId;
            break;
        case RI_MUMBO_TOKEN:
            ActorLocal_MumboToken* tokenLocal;
            tokenLocal = (ActorLocal_MumboToken*)&customActor->local;
            tokenLocal->uid = (mumbotoken_e)Rando::Logic::GetShuffledObject(randoCheckId).randoCollectionId;
            break;
        default:
            break;
    }
    return customActor;
}

Actor* CustomObject::SpawnCustomActorEX(RandoCheckId randoCheckId, int32_t position[3], ActorInfo* actorInfo, int32_t flags) {
    if (randoCheckId == RC_UNKNOWN) {
        return NULL;
    }

    Actor* customActor = actor_new(position, 0, actorInfo, flags);

    if (customActor != NULL) {
        customActor = SetCustomActorParametersEX(randoCheckId, customActor);
        randoActorList.push_back({ { position[0], position[1], position[2] }, randoCheckId, *customActor });
    }
    return customActor;
}

void CustomObject::FlushRandoSpawnQueue() {
    if (randoActorQueue.empty()) {
        return;
    }

    for (auto& queue : randoActorQueue) {
        if (queue.isSpawned) {
            continue;
        }

        RandoSaveCheck randoSaveCheck = Rando::Logic::GetShuffledObject(queue.randoCheckId);
        if (randoSaveCheck.randoCheckId == RC_UNKNOWN) {
            queue.isSpawned = true;
            continue;
        }

        actor_e randoActorId = (actor_e)Rando::StaticData::Items[randoSaveCheck.randoItemId].actorId;
        if (randoSaveCheck.obtained) {
            randoActorId = GetActorIdByShuffledObjectState(randoSaveCheck);
        }

        
        Actor* customActor = CustomObject::SpawnCustomActorEX(randoSaveCheck.randoCheckId, queue.position,
                                                              &actorInfoMap.at(randoActorId).first,
                                                              actorInfoMap.at(randoActorId).second);

        queue.isSpawned = true;
    }
}

void CustomObject::AddPropToSpawnQueueEX(int32_t position[3], RandoCheckId randoCheckId) {
    randoActorQueue.push_back({ { position[0], position[1], position[2] }, randoCheckId, false });
}

Actor* CustomObject::ShouldCreateCustomActorEX(RandoCheckId randoCheckId, int32_t position[3], bool isProp) {
    if (randoCheckId == RC_UNKNOWN) {
        return NULL;
    }

    RandoSaveCheck randoSaveCheck = Rando::Logic::GetShuffledObject(randoCheckId);
    actor_e randoActorId = (actor_e)Rando::StaticData::Items[randoSaveCheck.randoItemId].actorId;
    if (!randoSaveCheck.isShuffled) {
        return NULL;
    }

    if (CheckRandoActorListEX(randoCheckId)) {
        return GetCustomActorByCheckEX(randoCheckId);
    } else {
        if (isProp) {
            CustomObject::AddPropToSpawnQueueEX(position, randoCheckId);
            return NULL;
        }
        
        if (randoSaveCheck.obtained) {
            randoActorId = GetActorIdByShuffledObjectState(randoSaveCheck);
        }

        if (randoSaveCheck.randoCheckId == RC_UNKNOWN) {
            return NULL;
        }
        return CustomObject::SpawnCustomActorEX(randoCheckId, position, &actorInfoMap.at(randoActorId).first,
                                                actorInfoMap.at(randoActorId).second);
    }

    return NULL;
}

void CustomObject::ResolveCustomActorCollisionEX(RandoCheckId randoCheckId) {
    RandoSaveCheck shuffledObject = Rando::Logic::GetShuffledObject(randoCheckId);
    if (shuffledObject.randoCheckId == RC_UNKNOWN) {
        return;
    }

    int16_t playerPosI[3];
    f32 playerPosF[3];
    player_getPosition(playerPosF);
    ml_vec3f_to_vec3h(playerPosI, playerPosF);

    switch (shuffledObject.randoItemId) {
        case RI_JIGGY:
            if (gsworld_getMap() == MAP_26_MMM_NAPPERS_ROOM && randoCheckId != RC_MMM_JIGGY_MANSION_TABLE) {
                // TODO: Reimplement with new system...
                // marker_despawn(customActorMap.at(randoCheckId).marker);
                // customActorMap.erase(randoCheckId);
            }
            break;
        case RI_JINJO_BLUE:
        case RI_JINJO_GREEN:
        case RI_JINJO_ORANGE:
        case RI_JINJO_PINK:
        case RI_JINJO_YELLOW:
            if (Rando::StaticData::Checks[shuffledObject.shuffledCheckId].worldId == map_getLevel(gsworld_getMap())) {
                int32_t jinjoMarkerId =
                    GetJinjoActorMarkerId((actor_e)Rando::StaticData::Items[shuffledObject.randoItemId].actorId);
                item_adjustByDiffWithHud(ITEM_12_JINJOS, (1 << ((jinjoMarkerId + 6) & 0x1F)));
            } else {
                if (Rando::Logic::ShouldSpawnJinjoJiggy(
                        Rando::StaticData::Checks[shuffledObject.shuffledCheckId].worldId)) {
                    RandoCheckId jiggyCheckId = Rando::StaticData::GetJinjoJiggyCheckByLevelId(
                        Rando::StaticData::Checks[shuffledObject.shuffledCheckId].worldId);

                    if (jiggyCheckId != RC_UNKNOWN) {
                        Actor* customActor = ShouldCreateCustomActorEX(jiggyCheckId, (int32_t*)playerPosI, false);
                        if (customActor != NULL) {
                            ApplyCustomActorPhysics(jiggyCheckId, customActor, true);
                        }
                    }
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

void CustomObject::CheckObtainedEX(RandoCheckId randoCheckId) {
    for (auto& pool : Rando::Logic::shuffledPool) {
        if (pool.randoCheckId == randoCheckId && !pool.obtained) {
            pool.obtained = true;
            shouldRemoveEX = true;
            RANDO_SAVE_CHECKS[pool.randoCheckId].obtained = true;
            BK_LOG_INFO("(NEW) RandoCheckId %s collected!", Rando::StaticData::Checks[randoCheckId].name);
            Rando::StaticData::SendCollisionNotification(pool.randoItemId);
            Rando::StaticData::ModifyRandoInfFlagState(randoCheckId);
            break;
        }
    }
}

void CustomObject::ObjectCollectedEX(RandoCheckId randoCheckId) {
    shouldRemoveEX = false;
    for (int i = 0; i < randoActorList.size(); i++) {
        if (randoActorList[i].randoCheckId == randoCheckId) {
            CustomObject::CheckObtainedEX(randoCheckId);
            if (shouldRemoveEX) {
                CustomObject::ResolveCustomActorCollisionEX(randoCheckId);
                randoActorList.erase(randoActorList.begin() + i);
            }
            break;
        }
    }
}
