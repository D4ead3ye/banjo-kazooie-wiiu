#include "ObjectBehavior.h"
#include "port/Rando/Logic/Logic.h"
#include "port/Rando/CustomObject/CustomObject.h"

#include <libultraship/bridge.h>
#include "port/ui/cvar_prefixes.h"
#include <libultraship/bridge/consolevariablebridge.h>
#include "port/enhancements/events/PortEnhancements.h"
#include "port/enhancements/events/hooks/Events.h"
#include "spdlog/spdlog.h"

extern "C" {
extern f32 gBundle_yaw;
}

bool applyCustomPhysics = false;

void Rando::ObjectBehavior::InitBundleBehavior() {
    COND_VB_SHOULD(VB_BUNDLE_SPAWN_SET_ACTOR_DATA, EVENT_PRIORITY_NORMAL, IS_RANDO, {
        Actor* refActor = va_arg(args, Actor*);

        if (refActor == NULL) {
            *should = false;
        }
    })

    COND_VB_SHOULD(VB_OVERRIDE_BUNDLE_SPAWN, EVENT_PRIORITY_NORMAL, true, {
        bundle_e bundleId = va_arg(args, bundle_e);
        BundleInfo* bundleInfo = va_arg(args, BundleInfo*);
        s32 bundleCount = va_arg(args, s32);
		f32* position = va_arg(args, f32*);
        Actor** actor = va_arg(args, Actor**);

        if (!IS_RANDO) {
            return;
        }

        level_e levelId = map_getLevel(gsworld_getMap());

        int32_t spawnPosition[3];
        spawnPosition[0] = (int32_t)position[0];
        spawnPosition[1] = (int32_t)position[1];
        spawnPosition[2] = (int32_t)position[2];

        SPDLOG_INFO("Bundle Spawn: {}", std::to_string(bundleId));
        
        RandoCheckId randoCheckId = RC_UNKNOWN;
        applyCustomPhysics = false;

        switch (levelId) {
            case LEVEL_1_MUMBOS_MOUNTAIN:
                switch (bundleId) {
                    case BUNDLE_0_MM_HUT_MUSIC_NOTE:
                        randoCheckId = (RandoCheckId)((int32_t)RC_MM_NOTE_HUT_BUNDLE_1 + bundleCount);
                        applyCustomPhysics = true;
                        break;
                    case BUNDLE_3_MM_HUT_JINJO_GREEN:
                        randoCheckId = RC_MM_JINJO_GREEN;
                        applyCustomPhysics = true;
                        break;
                    case BUNDLE_4_MM_HUT_JIGGY:
                        if (spawnPosition[1] < 2000) {
                            randoCheckId = RC_MM_JIGGY_ORANGE_PADS;
                        } else {
                            randoCheckId = RC_MM_JIGGY_HUTS;
                            applyCustomPhysics = true;
                        }
                        break;
                    case BUNDLE_7__JIGGY:
                        randoCheckId = RC_MM_JIGGY_CHIMPY;
                        break;
                    case BUNDLE_10__JIGGY:
                        randoCheckId = RC_MM_JIGGY_JUJU;
                        break;
                    case BUNDLE_15__JIGGY:
                        randoCheckId = RC_MM_JIGGY_CONGA;
                        break;
                    default:
                        break;
                }
                break;
            case LEVEL_2_TREASURE_TROVE_COVE:
                switch (bundleId) {
                    case BUNDLE_4_MM_HUT_JIGGY:
                        randoCheckId = RC_TTC_JIGGY_RED_X;
                        break;
                    case BUNDLE_7__JIGGY:
                        randoCheckId = RC_TTC_JIGGY_BLUBBER;
                        break;
                    default:
                        break;
                }
                break;
            case LEVEL_3_CLANKERS_CAVERN:
                switch (bundleId) {
                    case BUNDLE_10__JIGGY:
                        switch (spawnPosition[1]) {
                            case 1536:
                                randoCheckId = RC_CC_JIGGY_TOOTH;
                                break;
                            default:
                                randoCheckId = RC_CC_JIGGY_RINGS;
                                break;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case LEVEL_4_BUBBLEGLOOP_SWAMP:
                switch (bundleId) {
                    case BUNDLE_7__JIGGY:
                        randoCheckId = RC_BGS_JIGGY_CROCTUS;
                        break;
                    case BUNDLE_8__JIGGY:
                        randoCheckId = RC_BGS_JIGGY_MR_VILE;
                        break;
                    case BUNDLE_9__JIGGY:
                        randoCheckId = RC_BGS_JIGGY_TANKTUP;
                        break;
                    case BUNDLE_B_BGS_HUT_MUSIC_NOTE:
                        randoCheckId = (RandoCheckId)((int32_t)RC_BGS_NOTE_HUT_BUNDLE_1 + bundleCount);
                        break;
                    case BUNDLE_C_BGS_HUT_JIGGY:
                        randoCheckId = RC_BGS_JIGGY_HUTS;
                        break;
                    case BUNDLE_10__JIGGY:
                        switch (spawnPosition[2]) {
                            case 49:
                                randoCheckId = RC_BGS_JIGGY_ELEVATED_WALKWAY;
                                break;
                            case -1386:
                                randoCheckId = RC_BGS_JIGGY_FLIBBITS;
                                break;
                            case 2799:
                                randoCheckId = RC_BGS_JIGGY_PINKEGG;
                                break;
                            case -1020:
                                randoCheckId = RC_BGS_JIGGY_TIPTUP;
                                break;
                            case -6148:
                                randoCheckId = RC_BGS_JIGGY_MAZE;
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case LEVEL_5_FREEZEEZY_PEAK:
                switch (bundleId) {
                    case BUNDLE_9__JIGGY:
                        randoCheckId = RC_FP_JIGGY_SLED_TO_BOGGY;
                        break;
                    case BUNDLE_10__JIGGY:
                        if (spawnPosition[1] > 1900) {
                            randoCheckId = RC_FP_JIGGY_BOGGY_RACE_1;
                        } else if (spawnPosition[1] < 850) {
                            randoCheckId = RC_FP_JIGGY_WOZZA;
                        }
                        break;
                    case BUNDLE_7__JIGGY:
                        randoCheckId = RC_FP_JIGGY_BOGGY_RACE_2;
                        break;
                }
                break;
            case LEVEL_6_LAIR:
                switch (bundleId) {
                    case BUNDLE_C_BGS_HUT_JIGGY:
                        if (spawnPosition[1] ==
                            Rando::StaticData::Checks[RC_GL_JIGGY_WITCH_SWITCH_MUMBOS_MOUNTAIN].posY) {
                            randoCheckId = RC_GL_JIGGY_WITCH_SWITCH_MUMBOS_MOUNTAIN;
                            break;
                        }
                        if (spawnPosition[1] ==
                            Rando::StaticData::Checks[RC_GL_JIGGY_WITCH_SWITCH_RUSTY_BUCKET_BAY].posY) {
                            randoCheckId = RC_GL_JIGGY_WITCH_SWITCH_RUSTY_BUCKET_BAY;
                            break;
                        }
                        if (spawnPosition[1] ==
                            Rando::StaticData::Checks[RC_GL_JIGGY_WITCH_SWITCH_CLICK_CLOCK_WOOD].posY) {
                            randoCheckId = RC_GL_JIGGY_WITCH_SWITCH_CLICK_CLOCK_WOOD;
                            break;
                        }
                        break;
                    case BUNDLE_10__JIGGY:
                        applyCustomPhysics = true;
                        randoCheckId = Rando::StaticData::GetCheckByPosition(spawnPosition[0], spawnPosition[1], spawnPosition[2]);
                        break;
                    default:
                        break;
                }
                break;
            case LEVEL_7_GOBIS_VALLEY:
                switch (bundleId) {
                    case BUNDLE_10__JIGGY:
                        randoCheckId = Rando::StaticData::GetCheckByPosition(spawnPosition[0], spawnPosition[1], spawnPosition[2]);
                        break;
                    case BUNDLE_D__EMPTY_HONEYCOMB:
                        randoCheckId = RC_GV_EMPTY_HONEYCOMB_GOBI;
                    default:
                        break;
                }
                break;
            case LEVEL_B_SPIRAL_MOUNTAIN:
                switch (bundleId) {
                    case BUNDLE_1F_SM_EMPTY_HONEYCOMB:
                        if (spawnPosition[1] >= 500 && spawnPosition[1] <= 800) {
                            randoCheckId = RC_SM_EMPTY_HONEYCOMB_COLLIWOBBLE;
                        } else {
                            randoCheckId = RC_SM_EMPTY_HONEYCOMB_QUARRIES;
                        }
                        break;
                    default:
                        break;
                }
                break;
            default:
                return;
        }

        if (randoCheckId == RC_UNKNOWN) {
            return;
        }

        *actor = CustomObject::ShouldCreateCustomActorEX(randoCheckId, spawnPosition, false);
        if (*actor == NULL) {
            *should = true;
            return;
        }
        if (applyCustomPhysics) {
            ApplyCustomActorPhysics(randoCheckId, *actor, false);
        } else {
            ApplyBundleActorPhysics(*actor, bundleId, (BundleInfo*)bundleInfo, gBundle_yaw);
        }

        *should = true;
	})
}
