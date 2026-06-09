#include "MiscBehavior.h"
#include "port/enhancements/events/hooks/Events.h"

#include "spdlog/spdlog.h"

extern "C" {
s32 getGameMode(void);
void mapSpecificFlags_set(s32 i, s32 val);

enum map_e gsworld_getMap(void);
enum level_e map_getLevel(enum map_e map);

Struct70s* func_8034C5AC(s32 arg0);
void func_8034E71C(Struct73s* arg0, s32 arg1, f32 arg2);
}

#define EMPTY_HONEYCOMB_OPTION_ENABLED RANDO_SAVE_OPTIONS[RO_SHUFFLE_EMPTY_HONEYCOMBS].optionValue
#define JIGGY_OPTION_ENABLED RANDO_SAVE_OPTIONS[RO_SHUFFLE_JIGGIES].optionValue

bool isPauseMenu = false;

void Rando::StaticData::ModifyRandoInfFlagState(RandoCheckId randoCheckId) {
    RandoInf randoInfFlag = RANDO_INF_UNKNOWN;

    switch (randoCheckId) {
        case RC_CC_JIGGY_CLANKER_RAISED:
            randoInfFlag = RANDO_INF_CLANKER_RAISED;
            break;
        case RC_CC_JIGGY_RINGS:
            randoInfFlag = RANDO_INF_MINIGAME_RINGS_COMPLETED;
            break;
        case RC_RBB_JIGGY_SNORKEL:
            randoInfFlag = RANDO_INF_ANCHOR_RAISED;
            break;
        default:
            break;
    }

    if (randoInfFlag != RANDO_INF_UNKNOWN) {
        CALL_EVENT(SetRandoInfFlag, randoInfFlag, 1);
    }
}

void Rando::MiscBehavior::InitWorldStateBehavior() {
    REGISTER_LISTENER(SetRandoInfFlag, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        SetRandoInfFlag* ev = (SetRandoInfFlag*)event;
        RandoInf flagId = (RandoInf)ev->flagId;

        if (!IS_RANDO) {
            return;
        }

        if (flagId < RANDO_INF_UNKNOWN && flagId > RANDO_INF_MAX) {
            return;
        }

        RANDO_SAVE_FLAGS[(RandoInf)flagId].flagState = ev->flagState;
    })

    REGISTER_LISTENER(OnGetLevelSpecificFlag, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnGetLevelSpecificFlag* ev = (OnGetLevelSpecificFlag*)event;
        ev->result = 0;

        level_e currentLevel = map_getLevel(gsworld_getMap());

        switch (ev->flagId) {
            case LEVEL_FLAG_29_FP_XMAS_TREE_COMPLETE:
                if (currentLevel == LEVEL_5_FREEZEEZY_PEAK) {
                    return;
                }
                event->Cancelled = true;
                ev->result = 1;
                break;
            default:
                break;
        }
    })

    REGISTER_LISTENER(OnActorSpawn, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnActorSpawn* ev = (OnActorSpawn*)event;

        if (!IS_RANDO) {
            return;
        }

        level_e levelId = map_getLevel(gsworld_getMap());

        switch (levelId) {
            case LEVEL_1_MUMBOS_MOUNTAIN:
                if (RANDO_SAVE_CHECKS[RC_MM_JIGGY_CHIMPY].obtained) {
                    if (ev->actorId == ACTOR_F_CHIMPY) {
                        event->Cancelled = true;
                        ev->result = NULL;
                    }
                }
                mapSpecificFlags_set(MM_SPECIFIC_FLAG_0_CHIMPY_STUMP_RAISED,
                                     RANDO_SAVE_CHECKS[RC_MM_JIGGY_CHIMPY].obtained);
                mapSpecificFlags_set(MM_SPECIFIC_FLAG_2_ORANGE_HAS_BEEN_RETURNED,
                                     RANDO_SAVE_CHECKS[RC_MM_JIGGY_CHIMPY].obtained);
                mapSpecificFlags_set(MM_SPECIFIC_FLAG_3_CHIMPY_HAS_LEFT,
                                     RANDO_SAVE_CHECKS[RC_MM_JIGGY_CHIMPY].obtained);
                break;
            case LEVEL_3_CLANKERS_CAVERN:
                if (gsworld_getMap() == MAP_22_CC_INSIDE_CLANKER &&
                    RANDO_SAVE_FLAGS[RANDO_INF_MINIGAME_RINGS_COMPLETED].flagState) {
                    func_8034E71C((Struct73s*)func_8034C5AC(0x131), 0x190, 12.0f);
                }
                break;
            case LEVEL_9_RUSTY_BUCKET_BAY:
                if (ev->actorId == 0x18F) {
                    mapSpecificFlags_set(0, RANDO_SAVE_CHECKS[RC_RBB_EMPTY_HONEYCOMB_BOAT_HOUSE].obtained);
                }
                break;
            default:
                break;
        }
    })

    REGISTER_LISTENER(OnIsJiggyScoreCollected, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnIsJiggyScoreCollected* ev = (OnIsJiggyScoreCollected*)event;

        if (!IS_RANDO) {
            return;
        }

        if (!JIGGY_OPTION_ENABLED) {
            return;
        }

        if (getGameMode() == GAME_MODE_4_PAUSED) {
            return;
        }

        for (auto& [randoCheckId, randoStaticCheck] : Rando::StaticData::Checks) {
            if (randoStaticCheck.randoCheckType != RCTYPE_JIGGY) {
                continue;
            }

            if (randoStaticCheck.collectionId == ev->jiggyId) {
                event->Cancelled = true;
                ev->result = RANDO_SAVE_CHECKS[randoCheckId].obtained;
                return;
            }
        }
    })

    REGISTER_LISTENER(OnIsJiggyScoreSpawned, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnIsJiggyScoreSpawned* ev = (OnIsJiggyScoreSpawned*)event;

        if (!IS_RANDO) {
            return;
        }

        if (!JIGGY_OPTION_ENABLED) {
            return;
        }

        switch (ev->jiggyId) {
            case JIGGY_17_CC_CLANKER_RAISED:
                event->Cancelled = true;
                ev->result = RANDO_SAVE_FLAGS[RANDO_INF_CLANKER_RAISED].flagState;
                break;
            case JIGGY_53_RBB_SNORKEL:
                event->Cancelled = true;
                ev->result = RANDO_SAVE_FLAGS[RANDO_INF_ANCHOR_RAISED].flagState;
                break;
            default:
                break;
        }

    })

    REGISTER_LISTENER(OnIsHoneycombScoreCollected, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnIsHoneycombScoreCollected* ev = (OnIsHoneycombScoreCollected*)event;

        if (!IS_RANDO) {
            return;
        }

        if (!EMPTY_HONEYCOMB_OPTION_ENABLED) {
            return;
        }

        for (auto& [randoCheckId, randoStaticCheck] : Rando::StaticData::Checks) {
            if (randoStaticCheck.randoCheckType != RCTYPE_EMPTY_HONEYCOMB) {
                continue;
            }

            if (randoStaticCheck.collectionId == ev->honeycombId) {
                event->Cancelled = true;
                ev->result = RANDO_SAVE_CHECKS[randoCheckId].obtained;
                return;
            }
        }
    })
}