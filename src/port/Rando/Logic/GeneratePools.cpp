#include "Logic.h"
// #include "port/Rando/Spoiler/Spoiler.h"
#include "port/ui/Notification.h"
#include "port/ui/cvar_prefixes.h"
#include <libultraship/bridge/consolevariablebridge.h>
#include <sstream>
#include <random>

#include "enums.h"

namespace Rando {

namespace Logic {
std::vector<RandoCheckId> checkPool;
std::vector<std::tuple<actor_e, int32_t, RandoCheckId>> itemPool;

std::vector<RandoCheckId> abilityCheckPool;
std::vector<std::tuple<actor_e, int32_t, RandoCheckId>> abilityItemPool;

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

void Rando::Logic::ShuffleRandoItems(const std::string& input, std::vector<std::tuple<actor_e, int32_t, RandoCheckId>>& pool) {
    uint32_t seed = GetRandoSeed(input);

    std::mt19937 g(seed);
    std::shuffle(pool.begin(), pool.end(), g);

    // finalSeed = seed;
}


void GenerateShufflePool(SaveData* saveData) {
    checkPool.clear();
    itemPool.clear();
    abilityCheckPool.clear();
    abilityItemPool.clear();
    shuffledPool.clear();

    for (auto& [randoCheckId, randoStaticCheck] : Rando::StaticData::Checks) {
        // if (randoCheckId == RC_UNKNOWN) {
        //     continue;
        // }

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
                abilityItemPool.push_back({ (actor_e)randoStaticCheck.actorId, randoStaticCheck.collectionId, randoCheckId });
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
        itemPool.push_back({ (actor_e)randoStaticCheck.actorId, randoStaticCheck.collectionId, randoCheckId });
    }

    if (!itemPool.empty()) {
        Rando::Logic::ShuffleRandoItems("", itemPool);
    }

    if (!abilityItemPool.empty()) {
        Rando::Logic::ShuffleRandoItems("", abilityItemPool);
    }

    if (RANDO_SAVE_OPTIONS[RO_LOGIC].optionValue == RO_LOGIC_GLITCHLESS) {
        Rando::Logic::GenerateGlitchlessLogicPool(checkPool, itemPool, abilityCheckPool, abilityItemPool, saveData);
    } else if (RANDO_SAVE_OPTIONS[RO_LOGIC].optionValue == RO_LOGIC_NO_LOGIC) {
        Rando::Logic::GenerateNoLogicPool(itemPool, abilityItemPool);
    }

    for (int i = 0; i < checkPool.size(); i++) {
        Rando::StaticData::RandoShuffledPool randoShuffleEntry = {
            .name = Rando::StaticData::Checks[checkPool[i]].name,
            .randoCheckId = checkPool[i],
            .shuffleCheckId = std::get<2>(itemPool[i]),
            .randoItemId = Rando::StaticData::GetRandoItemByActorId(std::get<0>(itemPool[i])),
            .randoCollectionId = std::get<1>(itemPool[i]),
            .isShuffled = checkPool[i] == RC_UNKNOWN ? false : true,
            .obtained = false,
            .skipped = false,
        };

        shuffledPool.push_back(randoShuffleEntry);

        RANDO_SAVE_CHECKS[randoShuffleEntry.randoCheckId].name = randoShuffleEntry.name;
        RANDO_SAVE_CHECKS[randoShuffleEntry.randoCheckId].randoItemId = randoShuffleEntry.randoItemId;
        RANDO_SAVE_CHECKS[randoShuffleEntry.randoCheckId].randoCollectionId = randoShuffleEntry.randoCollectionId;
        RANDO_SAVE_CHECKS[randoShuffleEntry.randoCheckId].shuffledCheckId = randoShuffleEntry.shuffleCheckId;
        RANDO_SAVE_CHECKS[randoShuffleEntry.randoCheckId].isShuffled = randoShuffleEntry.isShuffled;
        RANDO_SAVE_CHECKS[randoShuffleEntry.randoCheckId].obtained = randoShuffleEntry.obtained;
        RANDO_SAVE_CHECKS[randoShuffleEntry.randoCheckId].skipped = randoShuffleEntry.skipped;
    }

    if (!abilityCheckPool.empty()) {
        for (int a = 0; a < abilityCheckPool.size(); a++) {
            Rando::StaticData::RandoShuffledPool randoShuffleEntry = {
                .name = Rando::StaticData::Checks[abilityCheckPool[a]].name,
                .randoCheckId = abilityCheckPool[a],
                .shuffleCheckId = std::get<2>(abilityItemPool[a]),
                .randoItemId = Rando::StaticData::GetRandoItemByActorId(std::get<0>(abilityItemPool[a])),
                .randoCollectionId = std::get<1>(abilityItemPool[a]),
                .isShuffled = true,
                .obtained = false,
                .skipped = false,
            };

            shuffledPool.push_back(randoShuffleEntry);

            RANDO_SAVE_CHECKS[randoShuffleEntry.randoCheckId].name = randoShuffleEntry.name;
            RANDO_SAVE_CHECKS[randoShuffleEntry.randoCheckId].randoItemId = randoShuffleEntry.randoItemId;
            RANDO_SAVE_CHECKS[randoShuffleEntry.randoCheckId].randoCollectionId = randoShuffleEntry.randoCollectionId;
            RANDO_SAVE_CHECKS[randoShuffleEntry.randoCheckId].shuffledCheckId = randoShuffleEntry.shuffleCheckId;
            RANDO_SAVE_CHECKS[randoShuffleEntry.randoCheckId].isShuffled = randoShuffleEntry.isShuffled;
            RANDO_SAVE_CHECKS[randoShuffleEntry.randoCheckId].obtained = randoShuffleEntry.obtained;
            RANDO_SAVE_CHECKS[randoShuffleEntry.randoCheckId].skipped = randoShuffleEntry.skipped;
        }
    }

    for (auto& [optionId, optionValue] : Rando::StaticData::Options) {
        RANDO_SAVE_OPTIONS[optionId].name = optionValue.name;
        RANDO_SAVE_OPTIONS[optionId].optionValue = CVarGetInteger(optionValue.cvar, optionValue.defaultValue);
    }
}

void GeneratePoolFromSaveData(SaveData* saveData) {
    for (int i = RC_UNKNOWN; i < RC_MAX; i++) {
        RandoSaveCheck randoSaveCheck = saveData->shipSaveData.randoSaveData.randoSaveCheck[i];

        if (randoSaveCheck.isShuffled == false) {
            continue;
        }

        Rando::StaticData::RandoShuffledPool shuffledObject = {
            .name = Rando::StaticData::Checks[(RandoCheckId)i].name,
            .randoCheckId = (RandoCheckId)i,
            .shuffleCheckId = randoSaveCheck.shuffledCheckId,
            .randoCollectionId = randoSaveCheck.randoCollectionId,
            .randoItemId = randoSaveCheck.randoItemId,
            .isShuffled = randoSaveCheck.isShuffled,
            .obtained = randoSaveCheck.obtained,
            .skipped = randoSaveCheck.skipped,
        };

        Rando::Logic::shuffledPool.push_back(shuffledObject);
    }
}


} // namespace Logic

} // namespace Rando