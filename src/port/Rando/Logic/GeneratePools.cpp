#include "Logic.h"
// #include "port/Rando/Spoiler/Spoiler.h"
#include "port/ui/Notification.h"
#include <libultraship/bridge/consolevariablebridge.h>
#include <sstream>
#include <random>

#include "enums.h"

namespace Rando {

namespace Logic {
std::vector<RandoCheckId> checkPool;
std::vector<std::pair<actor_e, int32_t>> itemPool;

std::vector<RandoCheckId> abilityCheckPool;
std::vector<std::pair<actor_e, int32_t>> abilityItemPool;

std::vector<Rando::StaticData::RandoShuffledPool> shuffledPool;

uint32_t GetRandoSeed(const std::string& input) {
    // if (finalSeed > 0) {
    //     return finalSeed;
    // }

    std::random_device rd;

    // if (CVarGetInteger("gRandoSettings.ManualSeedEntry", 0)) {
    //     if (input.empty()) {
    //         return rd();
    //     } else {
    //         return Ship_Hash(input);
    //     }
    // }

    return rd();
}

void ShuffleRandoItems(const std::string& input, std::vector<std::pair<actor_e, int32_t>>& pool) {
    uint32_t seed = GetRandoSeed(input);

    std::mt19937 g(seed);
    std::shuffle(pool.begin(), pool.end(), g);

    // finalSeed = seed;
}


void GenerateShufflePool() {
    checkPool.clear();
    itemPool.clear();
    abilityCheckPool.clear();
    abilityItemPool.clear();
    shuffledPool.clear();

    for (auto& [randoCheckId, randoStaticCheck] : Rando::StaticData::Checks) {
        if (randoCheckId == RC_UNKNOWN) {
            continue;
        }

        if (randoStaticCheck.randoCheckType == RCTYPE_EMPTY_HONEYCOMB &&
            CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_EMPTY_HONEYCOMBS].cvar, 0) == RO_GENERIC_OFF) {
            continue;
        }

        if (randoStaticCheck.randoCheckType == RCTYPE_JIGGY &&
            CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_JIGGIES].cvar, 0) == RO_GENERIC_OFF) {
            continue;
        }

        if (randoStaticCheck.randoCheckType == RCTYPE_JINJO &&
            CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_JINJOS].cvar, 0) == RO_GENERIC_OFF) {
            continue;
        }

        if (randoStaticCheck.randoCheckType == RCTYPE_MOLEHILL) {
            if (CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_MOLEHILLS].cvar, 0) == RO_GENERIC_ON) {
                abilityCheckPool.push_back(randoCheckId);
                abilityItemPool.push_back({ (actor_e)randoStaticCheck.actorId, randoStaticCheck.collectionId });
            }
            continue;
        }

        if (randoStaticCheck.randoCheckType == RCTYPE_MUMBO_TOKEN &&
            CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_MUMBO_TOKENS].cvar, 0) == RO_GENERIC_OFF) {
            continue;
        }

        if (randoStaticCheck.randoCheckType == RCTYPE_MUSIC_NOTE &&
            CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_MUSIC_NOTES].cvar, 0) == RO_GENERIC_OFF) {
            continue;
        }

        checkPool.push_back(randoCheckId);
        itemPool.push_back({ (actor_e)randoStaticCheck.actorId, randoStaticCheck.collectionId });
    }

    ShuffleRandoItems("", itemPool);

    for (int i = 0; i < checkPool.size(); i++) {
        Rando::StaticData::RandoShuffledPool randoShuffleEntry = {
            .randoCheckId = checkPool[i],
            .randoItemId = Rando::StaticData::GetRandoItemByActorId(itemPool[i].first),
            .randoCollectionId = itemPool[i].second,
            .isShuffled = true,
            .obtained = false,
            .skipped = false,
        };

        shuffledPool.push_back(randoShuffleEntry);
    }

    if (!abilityCheckPool.empty()) {
        ShuffleRandoItems("", abilityItemPool);

        for (int a = 0; a < abilityCheckPool.size(); a++) {
            Rando::StaticData::RandoShuffledPool randoShuffleEntry = {
                .randoCheckId = abilityCheckPool[a],
                .randoItemId = Rando::StaticData::GetRandoItemByActorId(abilityItemPool[a].first),
                .randoCollectionId = abilityItemPool[a].second,
                .isShuffled = true,
                .obtained = false,
                .skipped = false,
            };

            shuffledPool.push_back(randoShuffleEntry);
        }
    }
}


} // namespace Logic

} // namespace Rando