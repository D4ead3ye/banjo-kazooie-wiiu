#include "Logic.h"

typedef struct {
    int32_t itemType;
    int32_t itemId;
    RandoCheckId shuffledCheckId;
} PlacedCheckObject;

bool CheckPlacedItem(int32_t itemId, std::vector<int32_t> placedItemPool) {
    for (auto& item : placedItemPool) {
        if (item == itemId) {
            return true;
        }
    }

    return false;
}

bool CheckPlacedAbility(int32_t abilityId, std::vector<int32_t> placedAbilityPool) {
    for (auto& ability : placedAbilityPool) {
        if (ability == abilityId) {
            return true;
        }
    }

    return false;
}

namespace Rando {

namespace Logic {

void GenerateGlitchlessLogicPool(std::vector<RandoCheckId>& checkPool,
                                 std::vector<std::tuple<actor_e, int32_t, RandoCheckId>>& itemPool,
                                 std::vector<RandoCheckId>& abilityCheckPool,
                                 std::vector<std::tuple<actor_e, int32_t, RandoCheckId>>& abilityItemPool) {

    PlacedCheckObject placedItems[RC_MAX];

    Rando::StaticData::RandoLogicData accessibleRegions[RR_MAX] = {};
    Rando::StaticData::RandoLogicData accessibleEvents[RA_MAX] = {};
    Rando::StaticData::RandoLogicData accessibleChecks[RC_MAX] = {};

    // Starting Region
    accessibleRegions[RR_SPIRAL_MOUNTAIN_ENTRANCE].canAccess = true;

    // Initial Generation
    bool isChanged = true;
    while (isChanged) {
        for (int i = RR_UNKNOWN; i < RR_MAX; i++) {
            isChanged = false;
            if (i == RR_UNKNOWN) {
                continue;
            }

            if (accessibleRegions[i].canAccess) {
                auto regionData = Rando::Logic::Regions[(RandoRegionId)i];
                for (auto& availableConnections : regionData.connections) {
                    if (availableConnections.second.first()) {
                        accessibleRegions[availableConnections.first].canAccess = true;
                        isChanged = true;
                    }
                }

                for (auto& availableEvents : regionData.events) {
                    if (availableEvents.second()) {
                        accessibleEvents[availableEvents.first].canAccess = true;
                        isChanged = true;
                    }
                }

                for (auto& availableChecks : regionData.checks) {
                    if (availableChecks.second.first()) {
                        accessibleChecks[availableChecks.first].canAccess = true;
                        isChanged = true;
                    }
                }
            }

            if (accessibleEvents[RA_GAME_COMPLETE].canAccess) {
                // Game Completed, Stop Here.
                isChanged = false;
            }
        }

        if (!isChanged) {
            int32_t totalChecks = 0;
            for (auto& checkCount : accessibleChecks) {
                if (checkCount.canAccess) {
                    totalChecks++;
                }
            }

            int32_t checkRoll = (rand() % totalChecks);
            for (int i = RR_UNKNOWN; i < RR_MAX; i++) {
                if (accessibleRegions[i].canAccess) {
                    auto regionData = Rando::Logic::Regions[(RandoRegionId)i];
                    for (auto& regionChecks : regionData.checks) {
                        if (regionChecks.first == Rando::StaticData::Checks[(RandoCheckId)checkRoll].randoCheckId) {
                            
                        }
                    }
                }
            }
        }
    }
    
}

} // namespace Logic

} // namespace Rando