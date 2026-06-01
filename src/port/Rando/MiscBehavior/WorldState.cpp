#include "MiscBehavior.h"
#include "port/enhancements/events/hooks/Events.h"

#include "spdlog/spdlog.h"

extern "C" {
extern f32 D_80389C00[3];
struct {
    u8 D_803832C0[0xD];
    u8 D_803832CD[0xD];
} jiggyscore;

u32 jiggyscore_isCollected(enum jiggy_e jiggy_id);
void jiggy_spawn(enum jiggy_e jiggy_id, f32 pos[3]);

void mapSpecificFlags_set(s32 i, s32 val);

enum map_e gsworld_getMap(void);
enum level_e map_getLevel(enum map_e map);

Struct70s* func_8034C5AC(s32 arg0);
void func_8034E71C(Struct73s* arg0, s32 arg1, f32 arg2);
}

#define JIGGY_OPTION_ENABLED RANDO_SAVE_OPTIONS[RO_SHUFFLE_JIGGIES].optionValue

void Rando::StaticData::ModifyRandoInfFlagState(RandoCheckId randoCheckId) {
    RandoInf randoInfFlag = RANDO_INF_UNKNOWN;

    switch (randoCheckId) {
        case RC_CC_JIGGY_CLANKER_RAISED:
            randoInfFlag = RANDO_INF_CLANKER_RAISED;
            break;
        case RC_CC_JIGGY_RINGS:
            randoInfFlag = RANDO_INF_MINIGAME_RINGS_COMPLETED;
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
    REGISTER_LISTENER(OnActorSpawn, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnActorSpawn* ev = (OnActorSpawn*)event;

        if (!IS_RANDO) {
            return;
        }

        level_e currentLevel = map_getLevel(gsworld_getMap());

        switch (currentLevel) {
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

        switch (ev->jiggyId) {
            case JIGGY_A_MM_CONGA:
                if (!RANDO_SAVE_CHECKS[RC_MM_JIGGY_CONGA].obtained) {
                    event->Cancelled = true;
                    ev->result = 0;
                }
                break;
            case JIGGY_17_CC_CLANKER_RAISED:
                event->Cancelled = true;
                ev->result = RANDO_SAVE_CHECKS[RC_CC_JIGGY_CLANKER_RAISED].obtained;
                break;
            case JIGGY_37_LAIR_BGS_WITCH_SWITCH:
                event->Cancelled = true;
                if (!RANDO_SAVE_CHECKS[RC_GL_JIGGY_WITCH_SWITCH_BUBBLEGLOOP_SWAMP].obtained) {
                    ev->result = 0;
                } else {
                    ev->result = 1;
                }
                break;
            case JIGGY_2A_FP_BOGGY_1:
                event->Cancelled = true;
                ev->result = RANDO_SAVE_CHECKS[RC_FP_JIGGY_SLED_TO_BOGGY].obtained;
                break;
            case JIGGY_2C_FP_BOGGY_3:
                event->Cancelled = true;
                ev->result = RANDO_SAVE_CHECKS[RC_FP_JIGGY_BOGGY_RACE_2].obtained;
                break;
            case JIGGY_2D_FP_SNOWMAN_BUTTONS:
                event->Cancelled = true;
                ev->result = RANDO_SAVE_CHECKS[RC_FP_JIGGY_SNOWMANS_BUTTONS].obtained;
                break;
            case JIGGY_30_FP_BOGGY_2:
                event->Cancelled = true;
                ev->result = RANDO_SAVE_CHECKS[RC_FP_JIGGY_BOGGY_RACE_1].obtained;
                break;
            case JIGGY_32_FP_WOZZA:
                event->Cancelled = true;
                ev->result = RANDO_SAVE_CHECKS[RC_FP_JIGGY_WOZZA].obtained;
                break;
            case JIGGY_31_FP_SIR_SLUSH:
                event->Cancelled = true;
                ev->result = RANDO_SAVE_CHECKS[RC_FP_JIGGY_BEAT_ALL_SIR_SLUSH].obtained;

            default:
                break;
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
            default:
                break;
        }

    })
}