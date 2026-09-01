#include "Logic.h"
// #include "port/Rando/Spoiler/Spoiler.h"
#include "port/UI/Notification.h"
#include "port/UI/cvar_prefixes.h"
#include <libultraship/bridge/consolevariablebridge.h>
#include <sstream>
#include <random>
#include <chrono>
#include <cstdint>

#include "enums.h"

int32_t randoFinalSeed = 0;

namespace Rando {

namespace Logic {
std::vector<RandoCheckId> checkPool;
std::vector<std::tuple<actor_e, int32_t, RandoCheckId>> itemPool;

std::vector<RandoCheckId> abilityCheckPool;
std::vector<std::tuple<actor_e, int32_t, RandoCheckId>> abilityItemPool;

std::vector<RandoSaveCheck> shuffledPool;

#ifdef __WIIU__
// coreinit/time.h drags in headers the decomp already defines; declare just this.
extern "C" int64_t OSGetSystemTime(void);
#endif

uint32_t GetRandoSeed(const std::string& input) {
    if (CVarGetInteger(CVAR_RANDOMIZER_SETTING("ManualInput"), 0) && !input.empty()) {
        return Ship_Hash(input);
    }

    // [port] Gather entropy from several independent sources and mix it properly.
    //
    // The wall clock alone was not enough. On Wii U it comes from
    // std::chrono::system_clock, which needs an RTC the console does not
    // reliably expose to the C library, so it could return the same value every
    // boot - the same seed, and the same shuffle, every run. Worse, this is
    // called once per pool: two calls landing in the same clock tick handed both
    // pools an identical seed, correlating their layouts even when the seed did
    // change between runs. The counter makes every call distinct regardless.
    uint64_t x = 0x9E3779B97F4A7C15ULL;
    x ^= static_cast<uint64_t>(GetUnixTimestamp());
    x ^= static_cast<uint64_t>(
             std::chrono::steady_clock::now().time_since_epoch().count()) * 0xD1B54A32D192ED03ULL;
    static uint32_t sCallCounter = 0;
    x ^= static_cast<uint64_t>(++sCallCounter) * 0xBF58476D1CE4E5B9ULL;
#ifdef __WIIU__
    x ^= static_cast<uint64_t>(OSGetSystemTime()) * 2654435761ULL;
#endif

    // splitmix64 finalizer: spreads a small change in any input across all bits,
    // so seeds from adjacent ticks are unrelated rather than adjacent.
    x += 0x9E3779B97F4A7C15ULL;
    x = (x ^ (x >> 30)) * 0xBF58476D1CE4E5B9ULL;
    x = (x ^ (x >> 27)) * 0x94D049BB133111EBULL;
    x = x ^ (x >> 31);

    const uint32_t seed = static_cast<uint32_t>(x ^ (x >> 32));
    BK_LOG_INFO("[rando] seed %u (call %u)", (unsigned)seed, (unsigned)sCallCounter);
    return seed;
}

void ShuffleRandoItems(const std::string& input, std::vector<std::tuple<actor_e, int32_t, RandoCheckId>>& pool) {
    uint32_t seed = GetRandoSeed(input);

    std::mt19937 rando(seed);
    std::shuffle(pool.begin(), pool.end(), rando);

    randoFinalSeed = seed;
}

void GenerateShufflePool(SaveData* saveData) {
    checkPool.clear();
    itemPool.clear();
    abilityCheckPool.clear();
    abilityItemPool.clear();
    shuffledPool.clear();

    for (auto& [randoCheckId, randoStaticCheck] : Rando::StaticData::Checks) {
        if (randoStaticCheck.randoCheckType == RCTYPE_BLUE_EGG &&
            CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_BLUE_EGGS].cvar, 0) == RO_GENERIC_OFF) {
            continue;
        }

        if (randoStaticCheck.randoCheckType == RCTYPE_EMPTY_HONEYCOMB &&
            CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_EMPTY_HONEYCOMBS].cvar, 0) == RO_GENERIC_OFF) {
            continue;
        }

        if (randoStaticCheck.randoCheckType == RCTYPE_EXTRA_LIFE &&
            CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_EXTRA_LIVES].cvar, 0) == RO_GENERIC_OFF) {
            continue;
        }

        if (randoStaticCheck.randoCheckType == RCTYPE_HONEYCOMB &&
            CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_BEEHIVE_HONEYCOMBS].cvar, 0) == RO_GENERIC_OFF) {
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
                abilityItemPool.push_back({ Rando::StaticData::GetActorIdByRandoItemId(randoStaticCheck.randoItemId),
                                            randoStaticCheck.collectionId, randoCheckId });
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

        if (randoStaticCheck.randoCheckType == RCTYPE_STOP_N_SWOP &&
            CVarGetInteger(Rando::StaticData::Options[RO_SHUFFLE_STOP_N_SWOP].cvar, 0) == RO_GENERIC_OFF) {
            continue;
        }

        checkPool.push_back(randoCheckId);
        itemPool.push_back({ Rando::StaticData::GetActorIdByRandoItemId(randoStaticCheck.randoItemId),
                             randoStaticCheck.collectionId, randoCheckId });
    }

    if (!itemPool.empty()) {
        Rando::Logic::ShuffleRandoItems(CVarGetString(CVAR_RANDOMIZER_SETTING("InputSeed"), ""), itemPool);
    }

    if (!abilityItemPool.empty()) {
        Rando::Logic::ShuffleRandoItems(CVarGetString(CVAR_RANDOMIZER_SETTING("InputSeed"), ""), abilityItemPool);
    }

    if (RANDO_SAVE_OPTIONS[RO_LOGIC].optionValue == RO_LOGIC_GLITCHLESS) {
        Rando::Logic::GenerateGlitchlessLogicPool(checkPool, itemPool, abilityCheckPool, abilityItemPool, saveData);
    } else if (RANDO_SAVE_OPTIONS[RO_LOGIC].optionValue == RO_LOGIC_NO_LOGIC) {
        Rando::Logic::GenerateNoLogicPool(itemPool, abilityItemPool);
    }

    if (checkPool.size() != itemPool.size()) {
        return;
    }

    for (int i = 0; i < checkPool.size(); i++) {
        RandoSaveCheck randoShuffleEntry = {
            .name = Rando::StaticData::Checks[checkPool[i]].name,
            .randoCheckId = checkPool[i],
            .randoItemId = Rando::StaticData::Checks[std::get<2>(itemPool[i])].randoItemId,
            .randoCollectionId = std::get<1>(itemPool[i]),
            .isShuffled = checkPool[i] == RC_UNKNOWN ? false : true,
            .eligible = false,
            .skipped = false,
        };

        shuffledPool.push_back(randoShuffleEntry);
        RANDO_SAVE_CHECKS[checkPool[i]] = randoShuffleEntry;
    }

    if (!abilityCheckPool.empty()) {
        for (int a = 0; a < abilityCheckPool.size(); a++) {
            RandoSaveCheck randoShuffleEntry = {
                .name = Rando::StaticData::Checks[abilityCheckPool[a]].name,
                .randoCheckId = abilityCheckPool[a],
                .randoItemId = Rando::StaticData::Checks[std::get<2>(abilityItemPool[a])].randoItemId,
                .randoCollectionId = std::get<1>(abilityItemPool[a]),
                .isShuffled = true,
                .eligible = false,
                .skipped = false,
            };

            shuffledPool.push_back(randoShuffleEntry);
            RANDO_SAVE_CHECKS[abilityCheckPool[a]] = randoShuffleEntry;
        }
    }

    for (auto& [optionId, optionValue] : Rando::StaticData::Options) {
        RANDO_SAVE_OPTIONS[optionId].name = optionValue.name;
        RANDO_SAVE_OPTIONS[optionId].optionValue = CVarGetInteger(optionValue.cvar, optionValue.defaultValue);
    }

    saveData->shipSaveData.randoSaveData.seedId = randoFinalSeed;
}

void GeneratePoolFromSaveData(SaveData* saveData) {
    for (int i = RC_UNKNOWN; i < RC_MAX; i++) {
        RandoSaveCheck randoSaveCheck = saveData->shipSaveData.randoSaveData.randoSaveCheck[i];

        if (randoSaveCheck.isShuffled == false) {
            continue;
        }

        RandoSaveCheck shuffledObject = {
            .name = Rando::StaticData::Checks[(RandoCheckId)i].name,
            .randoCheckId = randoSaveCheck.randoCheckId,
            .randoItemId = randoSaveCheck.randoItemId,
            .randoCollectionId = randoSaveCheck.randoCollectionId,
            .isShuffled = randoSaveCheck.isShuffled,
            .eligible = randoSaveCheck.eligible,
            .received = randoSaveCheck.received,
            .skipped = randoSaveCheck.skipped,
        };

        Rando::Logic::shuffledPool.push_back(shuffledObject);
    }
}

} // namespace Logic

} // namespace Rando