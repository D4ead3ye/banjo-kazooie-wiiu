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
}

#define JIGGY_OPTION_ENABLED RANDO_SAVE_OPTIONS[RO_SHUFFLE_JIGGIES].optionValue

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
        SPDLOG_INFO("Flag ID {} set to {}", std::to_string(ev->flagId).c_str(), std::to_string(ev->flagState).c_str());
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
                if (ev->actorId == ACTOR_3C_CC_KEY) {
                    if (!RANDO_SAVE_CHECKS[RC_CC_JIGGY_CLANKER_RAISED].obtained &&
                        RANDO_SAVE_FLAGS[RANDO_INF_CLANKER_RAISED].flagState) {
                        jiggy_spawn(JIGGY_17_CC_CLANKER_RAISED, D_80389C00);
                        SPDLOG_INFO("Spawning Clanker Jiggy");
                    }
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

        bool hasJiggy = (jiggyscore.D_803832C0[(ev->jiggyId - 1) / 8] & (1 << (ev->jiggyId & 7))) != 0;

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
            default:
                break;
        }
    })
}