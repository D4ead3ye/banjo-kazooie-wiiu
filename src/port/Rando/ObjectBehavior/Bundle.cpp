#include "ObjectBehavior.h"
#include "port/Rando/Logic/Logic.h"
#include "port/Rando/CustomObject/CustomObject.h"

#include <libultraship/bridge.h>
#include "port/ui/cvar_prefixes.h"
#include <libultraship/bridge/consolevariablebridge.h>
#include "port/enhancements/events/PortEnhancements.h"
#include "port/enhancements/events/hooks/Events.h"

extern "C" {
extern f32 gBundle_yaw;

enum map_e gsworld_getMap(void);
enum level_e map_getLevel(enum map_e map);
}

bool applyCustomPhysics = false;

RandoCheckId CheckMultiSpawnMap(int32_t spawnPosition[3]) {
    for (auto& [checkId, spawnPos] : multiSpawnCheckMap) {
        if (std::get<0>(spawnPos) == spawnPosition[0] && std::get<1>(spawnPos) == spawnPosition[1] &&
            std::get<2>(spawnPos) == spawnPosition[2]) {
            return checkId;
        }
    }

    return RC_UNKNOWN;
}

void Rando::ObjectBehavior::InitBundleBehavior() {
    COND_VB_SHOULD(VB_OVERRIDE_BUNDLE_SPAWN, EVENT_PRIORITY_NORMAL, true, {
        bundle_e bundleId = va_arg(args, bundle_e);
        BundleInfo* bundleInfo = va_arg(args, BundleInfo*);
        s32 bundleCount = va_arg(args, s32);
		f32* position = va_arg(args, f32*);
        Actor** actor = va_arg(args, Actor**);

        if (!IS_RANDO) {
            return;
        }

        RandoCheckId randoCheckId = RC_UNKNOWN;

        BK_LOG_INFO("Bundle Spawn: %i", bundleId);

        int32_t spawnPosition[3];
        spawnPosition[0] = (int32_t)position[0];
        spawnPosition[1] = (int32_t)position[1];
        spawnPosition[2] = (int32_t)position[2];

        RandoSaveCheck shuffledObject;
        shuffledObject.randoCheckId = RC_UNKNOWN;
        level_e levelId = map_getLevel(gsworld_getMap());
        applyCustomPhysics = false;

        switch (bundleId) {
            case BUNDLE_3_MM_HUT_JINJO_GREEN:
                shuffledObject = Rando::Logic::GetShuffledObject(RC_MM_JINJO_GREEN);
                break;
            case BUNDLE_4_MM_HUT_JIGGY:
                if (spawnPosition[1] < 2000) {
                    shuffledObject = Rando::Logic::GetShuffledObject(RC_MM_JIGGY_ORANGE_PADS);
                } else {
                    shuffledObject = Rando::Logic::GetShuffledObject(RC_MM_JIGGY_HUTS);
                    applyCustomPhysics = true;
                }
                break;
            case BUNDLE_0_MM_HUT_MUSIC_NOTE:
                shuffledObject =
                    Rando::Logic::GetShuffledObject((RandoCheckId)((int32_t)RC_MM_NOTE_HUT_BUNDLE_1 + bundleCount));
                break;
            case BUNDLE_1F_SM_EMPTY_HONEYCOMB:
                if (spawnPosition[1] >= 500 && spawnPosition[1] <= 800) {
                    shuffledObject = Rando::Logic::GetShuffledObject(RC_SM_EMPTY_HONEYCOMB_COLLIWOBBLE);
                } else if (spawnPosition[1] >= -200 && spawnPosition[1] <= 0) {
                    shuffledObject = Rando::Logic::GetShuffledObject(RC_SM_EMPTY_HONEYCOMB_QUARRIES);
                }
                break;
            case BUNDLE_7__JIGGY:
                if (map_getLevel(gsworld_getMap()) == LEVEL_4_BUBBLEGLOOP_SWAMP) {
                    shuffledObject = Rando::Logic::GetShuffledObject(RC_BGS_JIGGY_CROCTUS);
                }
                if (map_getLevel(gsworld_getMap()) == LEVEL_2_TREASURE_TROVE_COVE) {
                    shuffledObject = Rando::Logic::GetShuffledObject(RC_TTC_JIGGY_BLUBBER);
                }
                break;
            case BUNDLE_8__JIGGY:
                if (map_getLevel(gsworld_getMap()) == LEVEL_4_BUBBLEGLOOP_SWAMP) {
                    switch (spawnPosition[2]) {
                        default:
                             shuffledObject = Rando::Logic::GetShuffledObject(RC_BGS_JIGGY_MR_VILE);
                             break;
                    }
                }
                break;
            case BUNDLE_9__JIGGY:
                if (map_getLevel(gsworld_getMap()) == LEVEL_4_BUBBLEGLOOP_SWAMP) {
                    switch (spawnPosition[2]) {
                        case 1140:
                             shuffledObject = Rando::Logic::GetShuffledObject(RC_BGS_JIGGY_TANKTUP);
                             break;
                    }
                }
                break;
            case BUNDLE_10__JIGGY:
                switch (levelId) {
                    case LEVEL_1_MUMBOS_MOUNTAIN:
                        shuffledObject = Rando::Logic::GetShuffledObject(RC_MM_JIGGY_JUJU);
                        break;
                    case LEVEL_4_BUBBLEGLOOP_SWAMP:
                        switch (spawnPosition[2]) {
                            case 49:
                                shuffledObject = Rando::Logic::GetShuffledObject(RC_BGS_JIGGY_ELEVATED_WALKWAY);
                                break;
                            case -1386:
                                shuffledObject = Rando::Logic::GetShuffledObject(RC_BGS_JIGGY_FLIBBITS);
                                break;
                            case 2799:
                                shuffledObject = Rando::Logic::GetShuffledObject(RC_BGS_JIGGY_PINKEGG);
                                break;
                            case -1020:
                                shuffledObject = Rando::Logic::GetShuffledObject(RC_BGS_JIGGY_TIPTUP);
                                break;
                            case -6148:
                                shuffledObject = Rando::Logic::GetShuffledObject(RC_BGS_JIGGY_MAZE);
                                break;
                        }
                        break;
                    case LEVEL_6_LAIR:
                        randoCheckId = CheckMultiSpawnMap(spawnPosition);

                        if (randoCheckId == RC_UNKNOWN) {
                            randoCheckId = Rando::StaticData::GetCheckByPosition(spawnPosition[0], spawnPosition[1],
                                                                                 spawnPosition[2]);
                            applyCustomPhysics = true;
                        }

                        shuffledObject = Rando::Logic::GetShuffledObject(randoCheckId);
                        break;
                    case LEVEL_7_GOBIS_VALLEY:
                        shuffledObject = Rando::Logic::GetShuffledObject(Rando::StaticData::GetCheckByPosition(
                            spawnPosition[0], spawnPosition[1], spawnPosition[2]));
                        break;
                    default:
                        break;
                }
                break;
            case BUNDLE_C_BGS_HUT_JIGGY:
                if (map_getLevel(gsworld_getMap()) == LEVEL_6_LAIR) {
                    if (spawnPosition[1] == Rando::StaticData::Checks[RC_GL_JIGGY_WITCH_SWITCH_MUMBOS_MOUNTAIN].posY) {
                        shuffledObject = Rando::Logic::GetShuffledObject(RC_GL_JIGGY_WITCH_SWITCH_MUMBOS_MOUNTAIN);
                    } else if (spawnPosition[1] ==
                               Rando::StaticData::Checks[RC_GL_JIGGY_WITCH_SWITCH_RUSTY_BUCKET_BAY].posY) {
                        shuffledObject = Rando::Logic::GetShuffledObject(RC_GL_JIGGY_WITCH_SWITCH_RUSTY_BUCKET_BAY);
                    } else if (spawnPosition[1] ==
                               Rando::StaticData::Checks[RC_GL_JIGGY_WITCH_SWITCH_CLICK_CLOCK_WOOD].posY) {
                        shuffledObject = Rando::Logic::GetShuffledObject(RC_GL_JIGGY_WITCH_SWITCH_CLICK_CLOCK_WOOD);
                    }
                }
                if (map_getLevel(gsworld_getMap()) == LEVEL_4_BUBBLEGLOOP_SWAMP) {
                    shuffledObject = Rando::Logic::GetShuffledObject(RC_BGS_JIGGY_HUTS);
                }
                break;
            default:
                return;
        }
        if (shuffledObject.randoCheckId == RC_UNKNOWN) {
            return;
        }

        actor_e randoActorId = GetActorIdByShuffledObjectState(shuffledObject);
        if (randoActorId == ACTOR_1_UNKNOWN) {
            *should = true;
            return;
        }

        *actor = CustomObject::SpawnCustomActor(randoActorId, spawnPosition);
        if (*actor == NULL) {
            return;
        }

        *actor = CustomObject::SetCustomActorParameters(*actor, shuffledObject.randoCheckId);
        CustomObject::AddToCustomActorMap(shuffledObject.randoCheckId, *actor);
        if (applyCustomPhysics) {
            ApplyCustomActorPhysics(shuffledObject.randoCheckId, *actor, false);
        } else {
            ApplyBundleActorPhysics(*actor, bundleId, (BundleInfo*)bundleInfo, gBundle_yaw);
        }

        *should = true;
	})
}
