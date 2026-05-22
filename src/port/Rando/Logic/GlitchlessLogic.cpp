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

int32_t GetRandomAvailableCheckIndexExcludingType(Rando::StaticData::RandoLogicData (&checks)[RC_MAX],
                                                  RandoCheckType checkType) {
    std::vector<int32_t> availableIndex;
    availableIndex.reserve(RC_MAX);

    for (int i = RC_UNKNOWN; i < RC_MAX; ++i) {
        if (i == RC_UNKNOWN) {
            continue;
        }

        if (!checks[i].isFilled && Rando::StaticData::Checks[(RandoCheckId)i].randoCheckType != checkType) {
            availableIndex.push_back(i);
        }
    }

    if (availableIndex.empty()) {
        return -1;
    }

    int32_t randomCheck = rand() % availableIndex.size();

    return availableIndex[randomCheck];
}

int32_t GetRandomAvailableCheckIndexByType(Rando::StaticData::RandoLogicData (&checks)[RC_MAX],
                                                  RandoCheckType checkType) {
    std::vector<int32_t> availableIndex;
    availableIndex.reserve(RC_MAX);

    for (int i = RC_UNKNOWN; i < RC_MAX; ++i) {
        if (i == RC_UNKNOWN) {
            continue;
        }

        if (!checks[i].isFilled && Rando::StaticData::Checks[(RandoCheckId)i].randoCheckType == checkType) {
            availableIndex.push_back(i);
        }
    }

    if (availableIndex.empty()) {
        return -1;
    }

    int32_t randomCheck = rand() % availableIndex.size();

    return availableIndex[randomCheck];
}

int32_t GetRandomCheckIndexByType(Rando::StaticData::RandoLogicData (&checks)[RC_MAX], RandoCheckType checkType) {
    std::vector<int32_t> availableIndex;
    availableIndex.reserve(RC_MAX);

    for (int i = RC_UNKNOWN; i < RC_MAX; ++i) {
        if (i == RC_UNKNOWN) {
            continue;
        }

        if (checks[i].canAccess && !checks[i].isFilled && Rando::StaticData::Checks[(RandoCheckId)i].randoCheckType == checkType) {
            availableIndex.push_back(i);
        }
    }

    if (availableIndex.empty()) {
        return -1;
    }

    int32_t randomCheck = rand() % availableIndex.size();

    return availableIndex[randomCheck];
}

int32_t GetRandomCheckIndexExcludingType(Rando::StaticData::RandoLogicData (&checks)[RC_MAX], RandoCheckType checkType) {
    std::vector<int32_t> availableIndex;
    availableIndex.reserve(RC_MAX);

    for (int i = RC_UNKNOWN; i < RC_MAX; ++i) {
        if (i == RC_UNKNOWN) {
            continue;
        }

        if (checks[i].canAccess && !checks[i].isFilled && Rando::StaticData::Checks[(RandoCheckId)i].randoCheckType != checkType) {
            availableIndex.push_back(i);
        }
    }

    if (availableIndex.empty()) {
        return -1;
    }

    int32_t randomCheck = rand() % availableIndex.size();

    return availableIndex[randomCheck];
}

int32_t GetRandomItemIndex(std::vector<std::tuple<actor_e, int32_t, RandoCheckId>> items) {
    std::vector<int32_t> availableIndex;
    availableIndex.reserve(items.size());

    for (int i = 0; i < items.size(); ++i) {
        availableIndex.push_back(i);
    }

    if (availableIndex.empty()) {
        return -1;
    }

    int32_t randomItem = rand() % availableIndex.size();

    return availableIndex[randomItem];
}

int32_t GetRandomItemIndexByActor(std::vector<std::tuple<actor_e, int32_t, RandoCheckId>> items, actor_e actorId) {
    std::vector<int32_t> availableIndex;
    availableIndex.reserve(items.size());

    for (int i = 0; i < items.size(); ++i) {
        if (std::get<0>(items[i]) == actorId) {
            availableIndex.push_back(i);
        }
    }

    if (availableIndex.empty()) {
        return -1;
    }

    int32_t randomItem = rand() % availableIndex.size();

    return availableIndex[randomItem];
}

namespace Rando {

namespace Logic {

void TestLogic(std::vector<RandoCheckId>& checkPool, std::vector<std::tuple<actor_e, int32_t, RandoCheckId>>& itemPool,
               std::vector<RandoCheckId>& abilityCheckPool,
               std::vector<std::tuple<actor_e, int32_t, RandoCheckId>>& abilityItemPool, SaveData* saveData) {
    bool isGameComplete = false;
    int32_t checkIndex = 0;
    int32_t itemPoolIndex = 0;
    int32_t progressionIndex = 0;

    PlacedItemCounts placedItems = { .noteCount = 0, .jiggyCount = 0 };
    PlacedCheckObject placedCheckItems[RC_MAX] = {};

    Rando::StaticData::RandoLogicData accessibleRegions[RR_MAX] = {};
    Rando::StaticData::RandoLogicData accessibleEvents[RA_MAX] = {};
    Rando::StaticData::RandoLogicData accessibleChecks[RC_MAX] = {};

    // This doesn't have to exist, added strictly for better development testing.
    for (auto& [regionId, regionData] : Rando::Logic::Regions) {
        accessibleRegions[regionId].name = regionData.regionName;
    }

    for (auto& [checkId, checkData] : Rando::StaticData::Checks) {
        accessibleChecks[checkId].name = checkData.name;
    }

    // Starting Region Initialization
    accessibleRegions[RR_SPIRAL_MOUNTAIN_ENTRANCE].canAccess = true;
    auto regionData = Rando::Logic::Regions[RR_SPIRAL_MOUNTAIN_ENTRANCE];

    for (auto& availableConnections : regionData.connections) {
        if (availableConnections.second.first()) {
            accessibleRegions[availableConnections.first].canAccess = true;
        }
    }
    for (auto& availableEvents : regionData.events) {
        if (availableEvents.second()) {
            accessibleEvents[availableEvents.first].canAccess = true;
        }
    }
    for (auto& availableChecks : regionData.checks) {
        if (availableChecks.second.first()) {
            accessibleChecks[availableChecks.first].canAccess = true;
        }
    }

    while (!isGameComplete) {
        bool accessibilityAdded = false;

        for (int i = RR_UNKNOWN + 1; i < RR_MAX; i++) {
            if (!accessibleRegions[i].canAccess) {
                continue;
            }

            auto currentRegion = Rando::Logic::Regions[(RandoRegionId)i];

            for (auto& conn : currentRegion.connections) {
                if (!accessibleRegions[conn.first].canAccess && conn.second.first()) {
                    accessibleRegions[conn.first].canAccess = true;
                    accessibilityAdded = true;
                }
            }

            for (auto& ev : currentRegion.events) {
                if (!accessibleEvents[ev.first].canAccess && ev.second()) {
                    accessibleEvents[ev.first].canAccess = true;
                    accessibilityAdded = true;
                }
            }

            for (auto& chk : currentRegion.checks) {
                if (!accessibleChecks[chk.first].canAccess && chk.second.first()) {
                    accessibleChecks[chk.first].canAccess = true;
                    accessibilityAdded = true;
                }
            }

            if (i == RR_GRUNTILDAS_LAIR_LOBBY && placedItems.jiggyCount == 0) {
                if (CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_JIGGIES].cvar, 0) == RO_GENERIC_ON) {
                    checkIndex = GetRandomCheckIndexExcludingType(accessibleChecks, RCTYPE_MOLEHILL);
                    itemPoolIndex = GetRandomItemIndexByActor(itemPool, ACTOR_46_JIGGY);

                    if (checkIndex >= 0 && itemPoolIndex >= 0) {
                        placedCheckItems[checkIndex] = {
                            .actorId = std::get<0>(itemPool[itemPoolIndex]),
                            .collectionId = std::get<1>(itemPool[itemPoolIndex]),
                            .shuffledCheckId = std::get<2>(itemPool[itemPoolIndex]),
                        };
                        accessibleChecks[checkIndex].isFilled = true;
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

        if (accessibleEvents[RA_GAME_COMPLETE].canAccess) {
            itemPool.clear();
            abilityItemPool.clear();
            for (int i = RC_UNKNOWN; i < RC_MAX; i++) {
                if (i == RC_UNKNOWN) {
                    continue;
                }

                if (Rando::StaticData::Checks[(RandoCheckId)i].randoCheckType != RCTYPE_MOLEHILL) {
                    itemPool.push_back({ (actor_e)placedCheckItems[i].actorId, placedCheckItems[i].collectionId,
                                         placedCheckItems[i].shuffledCheckId });
                } else {
                    abilityItemPool.push_back({ (actor_e)placedCheckItems[i].actorId, placedCheckItems[i].collectionId,
                                                placedCheckItems[i].shuffledCheckId });
                }
            }

            for (int s = 0; s < sizeof(SaveData); s++) {
                gameFile_saveData[selectedFileNum].data[s] = 0;
            }

            for (auto& ability : abilityItemPool) {
                ability_setLearned(std::get<1>(ability), false);
                if (std::get<1>(ability) == ABILITY_8_FLAP_FLIP) {
                    ability_setLearned(ABILITY_A_HOLD_A_JUMP_HIGHER, false);
                    ability_setLearned(ABILITY_7_FEATHERY_FLAP, false);
                } else if (std::get<1>(ability) == ABILITY_4_CLAW_SWIPE) {
                    ability_setLearned(ABILITY_C_ROLL, false);
                    ability_setLearned(ABILITY_B_RATATAT_RAP, false);
                }
            }

            item_adjustByDiffWithoutHud(ITEM_C_NOTE, 0);
            item_adjustByDiffWithoutHud(ITEM_E_JIGGY, 0);
            item_adjustByDiffWithoutHud(ITEM_1C_MUMBO_TOKEN, 0);

            isGameComplete = true;
            Notification::Emit({ .message = "Seed Generation completed!" });
            break;
        }

        RefreshReachableRegions();

        if (progressionIndex >= progressionItems.size()) {
            while (!abilityItemPool.empty()) {
                checkIndex = GetRandomAvailableCheckIndexByType(accessibleChecks, RCTYPE_MOLEHILL);
                itemPoolIndex = GetRandomItemIndex(abilityItemPool);

                if (checkIndex < 0 || itemPoolIndex < 0) {
                    break;
                }

                placedCheckItems[checkIndex] = {
                    .actorId = std::get<0>(abilityItemPool[itemPoolIndex]),
                    .collectionId = std::get<1>(abilityItemPool[itemPoolIndex]),
                    .shuffledCheckId = std::get<2>(abilityItemPool[itemPoolIndex]),
                };

                accessibleChecks[checkIndex].isFilled = true;
                abilityItemPool.erase(abilityItemPool.begin() + itemPoolIndex);
                accessibilityAdded = true;
            }
            while (!itemPool.empty()) {
                checkIndex = GetRandomAvailableCheckIndexExcludingType(accessibleChecks, RCTYPE_MOLEHILL);
                itemPoolIndex = GetRandomItemIndex(itemPool);

                if (checkIndex < 0 || itemPoolIndex < 0) {
                    break;
                }

                placedCheckItems[checkIndex] = {
                    .actorId = std::get<0>(itemPool[itemPoolIndex]),
                    .collectionId = std::get<1>(itemPool[itemPoolIndex]),
                    .shuffledCheckId = std::get<2>(itemPool[itemPoolIndex]),
                };

                accessibleChecks[checkIndex].isFilled = true;
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
                accessibilityAdded = true;
            }
            continue;
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
                    checkIndex = GetRandomCheckIndexByType(accessibleChecks, RCTYPE_MOLEHILL);
                    if (checkIndex >= 0) {
                        placedCheckItems[checkIndex] = {
                            .actorId = std::get<0>(*it),
                            .collectionId = std::get<1>(*it),
                            .shuffledCheckId = std::get<2>(*it),
                        };

                        accessibleChecks[checkIndex].isFilled = true;
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
        }

        if (accessibleEvents[progressionItems[progressionIndex].progId].canAccess) {
            if (CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_JIGGIES].cvar, 0) == RO_GENERIC_ON) {
                while (placedItems.jiggyCount < progressionItems[progressionIndex].itemData[1].itemCount) {
                    checkIndex = GetRandomCheckIndexExcludingType(accessibleChecks, RCTYPE_MOLEHILL);
                    itemPoolIndex = GetRandomItemIndexByActor(itemPool, ACTOR_46_JIGGY);

                    if (checkIndex < 0 || itemPoolIndex < 0)
                        break;

                    placedCheckItems[checkIndex] = {
                        .actorId = std::get<0>(itemPool[itemPoolIndex]),
                        .collectionId = std::get<1>(itemPool[itemPoolIndex]),
                        .shuffledCheckId = std::get<2>(itemPool[itemPoolIndex]),
                    };

                    accessibleChecks[checkIndex].isFilled = true;
                    placedItems.jiggyCount++;
                    UpdateSaveDataItemCounts(placedItems);
                    itemPool.erase(itemPool.begin() + itemPoolIndex);
                    accessibilityAdded = true;
                }
            }

            if (CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_MUMBO_TOKENS].cvar, 0) == RO_GENERIC_ON) {
                while (placedItems.mumboTokenCount < progressionItems[progressionIndex].itemData[2].itemCount) {
                    checkIndex = GetRandomCheckIndexExcludingType(accessibleChecks, RCTYPE_MOLEHILL);
                    itemPoolIndex = GetRandomItemIndexByActor(itemPool, ACTOR_2D_MUMBO_TOKEN);

                    if (checkIndex < 0 || itemPoolIndex < 0)
                        break;

                    placedCheckItems[checkIndex] = {
                        .actorId = std::get<0>(itemPool[itemPoolIndex]),
                        .collectionId = std::get<1>(itemPool[itemPoolIndex]),
                        .shuffledCheckId = std::get<2>(itemPool[itemPoolIndex]),
                    };

                    accessibleChecks[checkIndex].isFilled = true;
                    placedItems.mumboTokenCount++;
                    UpdateSaveDataItemCounts(placedItems);
                    itemPool.erase(itemPool.begin() + itemPoolIndex);
                    accessibilityAdded = true;
                }
            }

            if (CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_MUSIC_NOTES].cvar, 0) == RO_GENERIC_ON) {
                while (placedItems.noteCount < progressionItems[progressionIndex].itemData[0].itemCount) {
                    checkIndex = GetRandomCheckIndexExcludingType(accessibleChecks, RCTYPE_MOLEHILL);
                    itemPoolIndex = GetRandomItemIndexByActor(itemPool, ACTOR_51_MUSIC_NOTE);

                    if (checkIndex < 0 || itemPoolIndex < 0)
                        break;

                    placedCheckItems[checkIndex] = {
                        .actorId = std::get<0>(itemPool[itemPoolIndex]),
                        .collectionId = std::get<1>(itemPool[itemPoolIndex]),
                        .shuffledCheckId = std::get<2>(itemPool[itemPoolIndex]),
                    };

                    accessibleChecks[checkIndex].isFilled = true;
                    placedItems.noteCount++;
                    UpdateSaveDataItemCounts(placedItems);
                    itemPool.erase(itemPool.begin() + itemPoolIndex);
                    accessibilityAdded = true;
                }
            }
        }

        if (placedItems.noteCount >= progressionItems[progressionIndex].itemData[0].itemCount &&
            placedItems.jiggyCount >= progressionItems[progressionIndex].itemData[1].itemCount &&
            placedItems.mumboTokenCount >= progressionItems[progressionIndex].itemData[2].itemCount &&
            (CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_MOLEHILLS].cvar, 0) == RO_GENERIC_OFF ||
             progressionAbilities[progressionIndex].isComplete)) {
            progressionIndex++;
        }

        if (!accessibilityAdded) {
            if (!failSafeTrigger) {
                failSafeTrigger = true;
                for (int a = 0; a < abilityItemPool.size(); a++) {
                    if (!ability_isUnlocked((ability_e)std::get<1>(abilityItemPool[a]))) {
                        checkIndex = GetRandomCheckIndexByType(accessibleChecks, RCTYPE_MOLEHILL);
                        if (checkIndex >= 0) {
                            placedCheckItems[checkIndex] = {
                                .actorId = std::get<0>(abilityItemPool[a]),
                                .collectionId = std::get<1>(abilityItemPool[a]),
                                .shuffledCheckId = std::get<2>(abilityItemPool[a]),
                            };

                            accessibleChecks[checkIndex].isFilled = true;
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