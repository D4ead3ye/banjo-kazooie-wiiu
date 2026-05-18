#include "Logic.h"

namespace Rando {

namespace Logic {

void GenerateNoLogicPool(std::vector<std::tuple<actor_e, int32_t, RandoCheckId>>& itemPool,
                         std::vector<std::tuple<actor_e, int32_t, RandoCheckId>>& abilityItemPool) {

    if (!itemPool.empty()) {
        Rando::Logic::ShuffleRandoItems("", itemPool);
    }

    if (!abilityItemPool.empty()) {
        Rando::Logic::ShuffleRandoItems("", abilityItemPool);
    }
}

} // namespace Logic

} // namespace Rando