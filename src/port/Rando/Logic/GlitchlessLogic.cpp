#include "Logic.h"
#include <libultraship/bridge/consolevariablebridge.h>
#include "port/ui/Notification.h"

typedef struct {
    int32_t actorId;
    int32_t collectionId;
    RandoCheckId shuffledCheckId;
} PlacedCheckObject;

typedef struct {
    int32_t noteCount;
    int32_t jiggyCount;
    int32_t mumboTokenCount;
} PlacedItemCounts;

typedef struct {
    int32_t itemType;
    int32_t itemId;
    int32_t itemCount;
} ProgressionItemData;

typedef struct {
    RandoAccessId progID;
    std::vector<int32_t> abilityIds;
    bool isComplete;
} ProgressionAbilityData;

typedef struct {
    RandoAccessId progId;
    std::vector<ProgressionItemData> itemData;
} ProgressionBaseId;

// clang-format off
std::vector<ProgressionBaseId> progressionItems = {
    { RA_NOTE_DOOR_50,  { { RITYPE_MUSIC_NOTE,  ACTOR_51_MUSIC_NOTE,    50 },
                          { RITYPE_JIGGY,       ACTOR_46_JIGGY,         1 },
                          { RITYPE_MUMBO_TOKEN, ACTOR_2D_MUMBO_TOKEN,   5 } } },

    { RA_NOTE_DOOR_180, { { RITYPE_MUSIC_NOTE,  ACTOR_51_MUSIC_NOTE,    180 },
                          { RITYPE_JIGGY,       ACTOR_46_JIGGY,         8 },
                          { RITYPE_MUMBO_TOKEN, ACTOR_2D_MUMBO_TOKEN,   0 } } },
    
    { RA_NOTE_DOOR_260, { { RITYPE_MUSIC_NOTE,  ACTOR_51_MUSIC_NOTE,    260 },
                          { RITYPE_JIGGY,       ACTOR_46_JIGGY,         15 },
                          { RITYPE_MUMBO_TOKEN, ACTOR_2D_MUMBO_TOKEN,   15 } } },
    
    { RA_NOTE_DOOR_450, { { RITYPE_MUSIC_NOTE,  ACTOR_51_MUSIC_NOTE,    450 },
                          { RITYPE_JIGGY,       ACTOR_46_JIGGY,         32 },
                          { RITYPE_MUMBO_TOKEN, ACTOR_2D_MUMBO_TOKEN,   30 } } },
    
    { RA_NOTE_DOOR_640, { { RITYPE_MUSIC_NOTE,  ACTOR_51_MUSIC_NOTE,    640 },
                          { RITYPE_JIGGY,       ACTOR_46_JIGGY,         54 },
                          { RITYPE_MUMBO_TOKEN, ACTOR_2D_MUMBO_TOKEN,   50 } } },
    
    { RA_NOTE_DOOR_765, { { RITYPE_MUSIC_NOTE,  ACTOR_51_MUSIC_NOTE,    765 },
                          { RITYPE_JIGGY,       ACTOR_46_JIGGY,         69 },
                          { RITYPE_MUMBO_TOKEN, ACTOR_2D_MUMBO_TOKEN,   75 } } },
    
    { RA_NOTE_DOOR_810, { { RITYPE_MUSIC_NOTE,  ACTOR_51_MUSIC_NOTE,    810 },
                          { RITYPE_JIGGY,       ACTOR_46_JIGGY,         94 },
                          { RITYPE_MUMBO_TOKEN, ACTOR_2D_MUMBO_TOKEN,   0 } } },
};

std::vector<ProgressionAbilityData> progressionAbilities = {
    { RA_NOTE_DOOR_50,  { ABILITY_10_TALON_TROT, ABILITY_8_FLAP_FLIP, ABILITY_6_EGGS },     false },
    { RA_NOTE_DOOR_180, { ABILITY_2_BEAK_BUSTER, ABILITY_D_SHOCK_JUMP, ABILITY_F_DIVE },    false },
    { RA_NOTE_DOOR_260, { ABILITY_E_WADING_BOOTS, ABILITY_5_CLIMB },                        false },
    { RA_NOTE_DOOR_450, { ABILITY_9_FLIGHT },                                               false },
    { RA_NOTE_DOOR_640, { ABILITY_F_DIVE },                                                 false },
    { RA_NOTE_DOOR_765, { ABILITY_2_BEAK_BUSTER },                                          false },
    { RA_NOTE_DOOR_810, { ABILITY_1_BEAK_BOMB },                                            false },
};
// clang-format on

bool failSafeTrigger = false;
int32_t prevProgressionIndex = -1;

void UpdateSaveDataItemCounts(PlacedItemCounts itemCounts) {
    item_adjustByDiffWithoutHud(ITEM_C_NOTE, itemCounts.noteCount);
    item_adjustByDiffWithoutHud(ITEM_E_JIGGY, itemCounts.jiggyCount);
    item_adjustByDiffWithoutHud(ITEM_1C_MUMBO_TOKEN, itemCounts.mumboTokenCount);

    switch (itemCounts.mumboTokenCount) {
        case 5:
            fileProgressFlag_set(FILEPROG_90_PAID_TERMITE_COST, 1);
            break;
        case 10:
            fileProgressFlag_set(FILEPROG_93_PAID_CROC_COST, 1);
            break;
        case 15:
            fileProgressFlag_set(FILEPROG_92_PAID_WALRUS_COST, 1);
            break;
        case 20:
            fileProgressFlag_set(FILEPROG_91_PAID_PUMPKIN_COST, 1);
            break;
        case 25:
            fileProgressFlag_set(FILEPROG_94_PAID_BEE_COST, 1);
            break;
        default:
            break;
    }
}

int32_t GetRandomCheckIndexS(Rando::StaticData::RandoLogicData (&checks)[RC_MAX], RandoCheckType checkType, bool shouldExclude, bool shouldRestrict, bool gameComplete) {
    std::vector<int32_t> availableIndex;
    availableIndex.reserve(RC_MAX);

    for (int i = RC_UNKNOWN; i < RC_MAX; ++i) {
        if (i == RC_UNKNOWN) {
            continue;
        }
        
        if (checks[i].canAccess || gameComplete) {
            if (!checks[i].isFilled && checks[i].isShuffled) {
                if (shouldExclude) {
                    if (Rando::StaticData::Checks[(RandoCheckId)i].randoCheckType != checkType) {
                        availableIndex.push_back(i);
                        continue;
                    }
                }
                if (shouldRestrict) {
                    if (Rando::StaticData::Checks[(RandoCheckId)i].randoCheckType == checkType) {
                        availableIndex.push_back(i);
                        continue;
                    }
                }
            }
        }
    }

    if (availableIndex.empty()) {
        return -1;
    }

    int32_t randomCheck = rand() % availableIndex.size();

    return availableIndex[randomCheck];
}

int32_t GetRandomItemIndexS(std::vector<std::tuple<actor_e, int32_t, RandoCheckId>> items, actor_e actorId) {
    std::vector<int32_t> availableIndex;
    availableIndex.reserve(items.size());
    
    for (int i = 0; i < items.size(); ++i) {
        if (std::get<2>(items[i]) == RC_UNKNOWN) {
            continue;
        }

        if (actorId != ACTOR_1_UNKNOWN) {
            if (std::get<0>(items[i]) == actorId) {
                availableIndex.push_back(i);
                continue;
            }
        } else {
            availableIndex.push_back(i);
            continue;
        }
    }

    if (availableIndex.empty()) {
        return -1;
    }

    int32_t randomItem = rand() % availableIndex.size();

    return availableIndex[randomItem];
}

void ResetSaveData() {
    for (int s = 0; s < sizeof(SaveData); s++) {
        gameFile_saveData[selectedFileNum].data[s] = 0;
    }

    for (int a = ABILITY_0_BARGE; a < ABILITY_13_1ST_NOTEDOOR; a++) {
        ability_setLearned((ability_e)a, false);
    }

    itemscore_noteScores_clear();

    item_adjustByDiffWithoutHud(ITEM_C_NOTE, 0);
    item_adjustByDiffWithoutHud(ITEM_E_JIGGY, 0);
    item_adjustByDiffWithoutHud(ITEM_1C_MUMBO_TOKEN, 0);
}

void FlushRemainingPools(Rando::StaticData::RandoLogicData (&reachableChecks)[RC_MAX],
                         PlacedItemCounts placedItems, PlacedCheckObject (&placedCheckItems)[RC_MAX],
                         std::vector<std::tuple<actor_e, int32_t, RandoCheckId>>& itemPool,
                         std::vector<std::tuple<actor_e, int32_t, RandoCheckId>>& abilityItemPool) {
    int32_t checkIndex = 0;
    int32_t itemPoolIndex = 0;

    while (!abilityItemPool.empty()) {
        checkIndex = GetRandomCheckIndexS(reachableChecks, RCTYPE_MOLEHILL, false, true, true);
        itemPoolIndex = GetRandomItemIndexS(abilityItemPool, ACTOR_1_UNKNOWN);

        if (checkIndex < 0 || itemPoolIndex < 0) {
            break;
        }

        placedCheckItems[checkIndex] = {
            .actorId = std::get<0>(abilityItemPool[itemPoolIndex]),
            .collectionId = std::get<1>(abilityItemPool[itemPoolIndex]),
            .shuffledCheckId = std::get<2>(abilityItemPool[itemPoolIndex]),
        };

        reachableChecks[checkIndex].isFilled = true;
        abilityItemPool.erase(abilityItemPool.begin() + itemPoolIndex);
    }
    while (!itemPool.empty()) {
        checkIndex = GetRandomCheckIndexS(reachableChecks, RCTYPE_MOLEHILL, true, false, true);
        itemPoolIndex = GetRandomItemIndexS(itemPool, ACTOR_1_UNKNOWN);

        if (checkIndex < 0 || itemPoolIndex < 0) {
            break;
        }

        placedCheckItems[checkIndex] = {
            .actorId = std::get<0>(itemPool[itemPoolIndex]),
            .collectionId = std::get<1>(itemPool[itemPoolIndex]),
            .shuffledCheckId = std::get<2>(itemPool[itemPoolIndex]),
        };

        reachableChecks[checkIndex].isFilled = true;
        switch (std::get<0>(itemPool[itemPoolIndex])) {
            case ACTOR_46_JIGGY:
                placedItems.jiggyCount++;
                break;
            case ACTOR_51_MUSIC_NOTE:
                placedItems.noteCount++;
                break;
            case ACTOR_2D_MUMBO_TOKEN:
                placedItems.mumboTokenCount++;
                break;
            default:
                break;
        }
        UpdateSaveDataItemCounts(placedItems);
        itemPool.erase(itemPool.begin() + itemPoolIndex);
    }
}

namespace Rando {

namespace Logic {

void GenerateGlitchlessLogicPool(std::vector<RandoCheckId>& checkPool,
                                 std::vector<std::tuple<actor_e, int32_t, RandoCheckId>>& itemPool,
                                 std::vector<RandoCheckId>& abilityCheckPool,
                                 std::vector<std::tuple<actor_e, int32_t, RandoCheckId>>& abilityItemPool, SaveData* saveData) {
    bool isGameComplete = false;
    int32_t checkIndex = 0;
    int32_t itemPoolIndex = 0;
    int32_t progressionIndex = 0;

    PlacedItemCounts placedItems = { .noteCount = 0, .jiggyCount = 0 };
    PlacedCheckObject placedCheckItems[RC_MAX] = {};

    for (auto& shuffledCheck : checkPool) {
        reachableChecks[shuffledCheck].isShuffled = true;
    }
    for (auto& shuffledAbiity : abilityCheckPool) {
        reachableChecks[shuffledAbiity].isShuffled = true;
    }

    // This doesn't have to exist, added strictly for better development testing.
    for (auto& [regionId, regionData] : Rando::Logic::Regions) {
        reachableRegions[regionId].name = regionData.regionName;
        reachableRegions[regionId].canAccess = false;
        reachableRegions[regionId].isFilled = false;
    }

    for (auto& [checkId, checkData] : Rando::StaticData::Checks) {
        reachableChecks[checkId].name = checkData.name;
        reachableChecks[checkId].canAccess = false;
        reachableChecks[checkId].isFilled = false;
    }

    // Starting Region Initialization
    reachableRegions[RR_SPIRAL_MOUNTAIN_ENTRANCE].canAccess = true;
    auto regionData = Rando::Logic::Regions[RR_SPIRAL_MOUNTAIN_ENTRANCE];

    for (auto& availableConnections : regionData.connections) {
        if (availableConnections.second.first()) {
            reachableRegions[availableConnections.first].canAccess = true;
        }
    }
    for (auto& availableEvents : regionData.events) {
        if (availableEvents.second()) {
            reachableEvents[availableEvents.first].canAccess = true;
        }
    }
    for (auto& availableChecks : regionData.checks) {
        if (availableChecks.second.first()) {
            reachableChecks[availableChecks.first].canAccess = true;
        }
    }

    while (!isGameComplete) {
        bool accessibilityAdded = false;

        for (int i = RR_UNKNOWN + 1; i < RR_MAX; i++) {
            if (!reachableRegions[i].canAccess) {
                continue;
            }

            auto currentRegion = Rando::Logic::Regions[(RandoRegionId)i];

            for (auto& conn : currentRegion.connections) {
                if (!reachableRegions[conn.first].canAccess && conn.second.first()) {
                    reachableRegions[conn.first].canAccess = true;
                    accessibilityAdded = true;
                }
            }

            for (auto& ev : currentRegion.events) {
                if (!reachableEvents[ev.first].canAccess && ev.second()) {
                    reachableEvents[ev.first].canAccess = true;
                    accessibilityAdded = true;
                }
            }

            for (auto& chk : currentRegion.checks) {
                if (!reachableChecks[chk.first].canAccess && chk.second.first()) {
                    reachableChecks[chk.first].canAccess = true;
                    accessibilityAdded = true;
                }
            }

            if (i == RR_GRUNTILDAS_LAIR_LOBBY && placedItems.jiggyCount == 0) {
                if (CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_JIGGIES].cvar, 0) == RO_GENERIC_ON) {
                    checkIndex = GetRandomCheckIndexS(reachableChecks, RCTYPE_MOLEHILL, true, false, isGameComplete);
                    itemPoolIndex = GetRandomItemIndexS(itemPool, ACTOR_46_JIGGY);

                    if (checkIndex >= 0 && itemPoolIndex >= 0) {
                        placedCheckItems[checkIndex] = {
                            .actorId = std::get<0>(itemPool[itemPoolIndex]),
                            .collectionId = std::get<1>(itemPool[itemPoolIndex]),
                            .shuffledCheckId = std::get<2>(itemPool[itemPoolIndex]),
                        };
                        reachableChecks[checkIndex].isFilled = true;
                        placedItems.jiggyCount++;
                        UpdateSaveDataItemCounts(placedItems);
                        itemPool.erase(itemPool.begin() + itemPoolIndex);
                        accessibilityAdded = true;
                    } else {
                        Notification::Emit({ .message = "No Checks left for First Jiggy." });
                    }
                }
            }
        }

        if (reachableEvents[RA_GAME_COMPLETE].canAccess) {
            FlushRemainingPools(reachableChecks, placedItems, placedCheckItems, itemPool, abilityItemPool);

            itemPool.clear();
            abilityItemPool.clear();
            for (auto& check : checkPool) {
                itemPool.push_back({ (actor_e)placedCheckItems[check].actorId, placedCheckItems[check].collectionId,
                                        placedCheckItems[check].shuffledCheckId });
            }
            for (auto& mole : abilityCheckPool) {
                abilityItemPool.push_back({ (actor_e)placedCheckItems[mole].actorId, placedCheckItems[mole].collectionId,
                                        placedCheckItems[mole].shuffledCheckId });
            }

            RefreshMetrics();
            ResetSaveData();

            isGameComplete = true;
            Notification::Emit({ .message = "Seed Generation completed!" });
            break;
        }

        if (CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_MOLEHILLS].cvar, 0) == RO_GENERIC_ON) {
            int32_t progCheck = 0;
            for (auto& abilityId : progressionAbilities[progressionIndex].abilityIds) {
                if (ability_isUnlocked((ability_e)abilityId)) {
                    progCheck++;
                    continue;
                }

                auto it = std::find_if(abilityItemPool.begin(), abilityItemPool.end(),
                                       [abilityId](const auto& item) { return std::get<1>(item) == abilityId; });

                if (it != abilityItemPool.end()) {
                    checkIndex = GetRandomCheckIndexS(reachableChecks, RCTYPE_MOLEHILL, false, true, isGameComplete);
                    if (checkIndex >= 0) {
                        placedCheckItems[checkIndex] = {
                            .actorId = std::get<0>(*it),
                            .collectionId = std::get<1>(*it),
                            .shuffledCheckId = std::get<2>(*it),
                        };

                        reachableChecks[checkIndex].isFilled = true;
                        ability_unlock((ability_e)abilityId);

                        if (abilityId == ABILITY_8_FLAP_FLIP) {
                            ability_unlock(ABILITY_A_HOLD_A_JUMP_HIGHER);
                            ability_unlock(ABILITY_7_FEATHERY_FLAP);
                        } else if (abilityId == ABILITY_4_CLAW_SWIPE) {
                            ability_unlock(ABILITY_C_ROLL);
                            ability_unlock(ABILITY_B_RATATAT_RAP);
                        }

                        abilityItemPool.erase(it);
                        progCheck++;
                        accessibilityAdded = true;
                    }
                }
            }
            if (progCheck == progressionAbilities[progressionIndex].abilityIds.size()) {
                progressionAbilities[progressionIndex].isComplete = true;
            }
        } else {
            switch (progressionAbilities[progressionIndex].progID) {
                case RA_NOTE_DOOR_50:
                    __chSmBottles_skipIntroTutorial();
                    ability_unlock(ABILITY_6_EGGS);
                    ability_unlock(ABILITY_10_TALON_TROT);
                    ability_unlock(ABILITY_2_BEAK_BUSTER);
                    break;
                case RA_NOTE_DOOR_180:
                    ability_unlock(ABILITY_D_SHOCK_JUMP);
                    ability_unlock(ABILITY_12_WONDERWING);
                    ability_unlock(ABILITY_9_FLIGHT);
                    break;
                case RA_NOTE_DOOR_260:
                    ability_unlock(ABILITY_E_WADING_BOOTS);
                    break;
                case RA_NOTE_DOOR_450:
                    ability_unlock(ABILITY_1_BEAK_BOMB);
                    ability_unlock(ABILITY_11_TURBO_TALON);
                    break;
                default:
                    break;
            }

            for (auto& abilityId : progressionAbilities[progressionIndex].abilityIds) {
                if (!ability_isUnlocked((ability_e)abilityId)) {
                    ability_unlock((ability_e)abilityId);

                    if (abilityId == ABILITY_8_FLAP_FLIP) {
                        ability_unlock(ABILITY_A_HOLD_A_JUMP_HIGHER);
                        ability_unlock(ABILITY_7_FEATHERY_FLAP);
                    } else if (abilityId == ABILITY_4_CLAW_SWIPE) {
                        ability_unlock(ABILITY_C_ROLL);
                        ability_unlock(ABILITY_B_RATATAT_RAP);
                    }
                }
            }
            progressionAbilities[progressionIndex].isComplete = true;
        }

        if (reachableEvents[progressionItems[progressionIndex].progId].canAccess) {
            if (CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_JIGGIES].cvar, 0) == RO_GENERIC_ON) {
                while (placedItems.jiggyCount < progressionItems[progressionIndex].itemData[1].itemCount) {
                    checkIndex = GetRandomCheckIndexS(reachableChecks, RCTYPE_MOLEHILL, true, false, isGameComplete);
                    itemPoolIndex = GetRandomItemIndexS(itemPool, ACTOR_46_JIGGY);

                    if (checkIndex < 0 || itemPoolIndex < 0)
                        break;

                    placedCheckItems[checkIndex] = {
                        .actorId = std::get<0>(itemPool[itemPoolIndex]),
                        .collectionId = std::get<1>(itemPool[itemPoolIndex]),
                        .shuffledCheckId = std::get<2>(itemPool[itemPoolIndex]),
                    };

                    reachableChecks[checkIndex].isFilled = true;
                    placedItems.jiggyCount++;
                    UpdateSaveDataItemCounts(placedItems);
                    itemPool.erase(itemPool.begin() + itemPoolIndex);
                    accessibilityAdded = true;
                }
            } else {
                placedItems.jiggyCount = progressionItems[progressionIndex].itemData[1].itemCount;
                UpdateSaveDataItemCounts(placedItems);
            }

            if (CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_MUMBO_TOKENS].cvar, 0) == RO_GENERIC_ON) {
                while (placedItems.mumboTokenCount < progressionItems[progressionIndex].itemData[2].itemCount) {
                    checkIndex = GetRandomCheckIndexS(reachableChecks, RCTYPE_MOLEHILL, true, false, isGameComplete);
                    itemPoolIndex = GetRandomItemIndexS(itemPool, ACTOR_2D_MUMBO_TOKEN);

                    if (checkIndex < 0 || itemPoolIndex < 0)
                        break;

                    placedCheckItems[checkIndex] = {
                        .actorId = std::get<0>(itemPool[itemPoolIndex]),
                        .collectionId = std::get<1>(itemPool[itemPoolIndex]),
                        .shuffledCheckId = std::get<2>(itemPool[itemPoolIndex]),
                    };

                    reachableChecks[checkIndex].isFilled = true;
                    placedItems.mumboTokenCount++;
                    UpdateSaveDataItemCounts(placedItems);
                    itemPool.erase(itemPool.begin() + itemPoolIndex);
                    accessibilityAdded = true;
                }
            } else {
                placedItems.mumboTokenCount = progressionItems[progressionIndex].itemData[2].itemCount;
                UpdateSaveDataItemCounts(placedItems);
            }

            if (CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_MUSIC_NOTES].cvar, 0) == RO_GENERIC_ON) {
                while (placedItems.noteCount < progressionItems[progressionIndex].itemData[0].itemCount) {
                    checkIndex = GetRandomCheckIndexS(reachableChecks, RCTYPE_MOLEHILL, true, false, isGameComplete);
                    itemPoolIndex = GetRandomItemIndexS(itemPool, ACTOR_51_MUSIC_NOTE);

                    if (checkIndex < 0 || itemPoolIndex < 0)
                        break;

                    placedCheckItems[checkIndex] = {
                        .actorId = std::get<0>(itemPool[itemPoolIndex]),
                        .collectionId = std::get<1>(itemPool[itemPoolIndex]),
                        .shuffledCheckId = std::get<2>(itemPool[itemPoolIndex]),
                    };

                    reachableChecks[checkIndex].isFilled = true;
                    placedItems.noteCount++;
                    UpdateSaveDataItemCounts(placedItems);
                    itemPool.erase(itemPool.begin() + itemPoolIndex);
                    accessibilityAdded = true;
                }
            } else {
                placedItems.noteCount = progressionItems[progressionIndex].itemData[0].itemCount;
                UpdateSaveDataItemCounts(placedItems);
            }
        }

        if (placedItems.noteCount >= progressionItems[progressionIndex].itemData[0].itemCount &&
            placedItems.jiggyCount >= progressionItems[progressionIndex].itemData[1].itemCount &&
            placedItems.mumboTokenCount >= progressionItems[progressionIndex].itemData[2].itemCount &&
            progressionAbilities[progressionIndex].isComplete) {

            if (progressionIndex < progressionAbilities.size() - 1) {
                progressionIndex++;
            }
        }

        if (!accessibilityAdded) {
            if (!failSafeTrigger) {
                failSafeTrigger = true;
                for (int a = 0; a < abilityItemPool.size(); a++) {
                    if (!ability_isUnlocked((ability_e)std::get<1>(abilityItemPool[a]))) {
                        checkIndex = GetRandomCheckIndexS(reachableChecks, RCTYPE_MOLEHILL, false, true, isGameComplete);
                        if (checkIndex >= 0) {
                            placedCheckItems[checkIndex] = {
                                .actorId = std::get<0>(abilityItemPool[a]),
                                .collectionId = std::get<1>(abilityItemPool[a]),
                                .shuffledCheckId = std::get<2>(abilityItemPool[a]),
                            };

                            reachableChecks[checkIndex].isFilled = true;
                            ability_unlock((ability_e)std::get<1>(abilityItemPool[a]));

                            if (std::get<1>(abilityItemPool[a]) == ABILITY_8_FLAP_FLIP) {
                                ability_unlock(ABILITY_A_HOLD_A_JUMP_HIGHER);
                                ability_unlock(ABILITY_7_FEATHERY_FLAP);
                            } else if (std::get<1>(abilityItemPool[a]) == ABILITY_4_CLAW_SWIPE) {
                                ability_unlock(ABILITY_C_ROLL);
                                ability_unlock(ABILITY_B_RATATAT_RAP);
                            }

                            abilityItemPool.erase(abilityItemPool.begin() + a);
                            a = 0;
                            failSafeTrigger = false;
                        }
                    }
                }
            } else {
                if (prevProgressionIndex == progressionIndex) {
                    Notification::Emit({ .message = "Seed Configuration impossible, failed to generate." });
                    RefreshMetrics();
                    ResetSaveData();
                    break;
                } else {
                    prevProgressionIndex = progressionIndex;
                }
            }
        }
    }
}

} // namespace Logic

} // namespace Rando