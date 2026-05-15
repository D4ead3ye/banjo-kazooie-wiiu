#ifndef RANDO_LOGIC_H
#define RANDO_LOGIC_H

#include "port/Rando/Rando.h"
#include "port/ShipUtils.h"

extern "C" {
s32 item_getCount(enum item_e item);

int ability_isUnlocked(enum ability_e uid);

bool fileProgressFlag_get(enum file_progress_e index);
s32 __transformation_getCost(enum transformation_e trans_id);
s32 _puzzleCost(s32 index);

u32 player_getTransformation(void);
}

extern std::map<ability_e, std::pair<const char*, const char*>> abilityLoadoutMap;
extern std::map<item_e, std::pair<const char*, const char*>> itemLoadoutMap;

namespace Rando {

namespace Logic {
extern std::vector<Rando::StaticData::RandoShuffledPool> shuffledPool;

void GenerateShufflePool();
void GeneratePoolFromSaveData(SaveData* saveData);
void InitializeSaveData(SaveData* saveData);
void GenerateSaveData(SaveData* saveData);
void GrantStartingLoadout();

inline bool IsCheckShuffled(RandoCheckId randoCheckId) {
    bool isShuffled = false;

    for (auto& object : shuffledPool) {
        if (object.randoCheckId == randoCheckId) {
            isShuffled = object.isShuffled;
            break;
        }
    }

    return isShuffled;
}

inline Rando::StaticData::RandoShuffledPool GetShuffledObject(RandoCheckId randoCheckId) {
    Rando::StaticData::RandoShuffledPool shuffledObject;
    shuffledObject.randoCheckId = RC_UNKNOWN;

    if (!IsCheckShuffled(randoCheckId)) {
        return shuffledObject;
    }

    for (auto& object : shuffledPool) {
        if (object.randoCheckId == randoCheckId) {
            shuffledObject = object;
            break;
        }
    }

    return shuffledObject;
}

inline bool IsCheckObtained(RandoCheckId randoCheckId) {
    bool isObtained = false;

    for (auto& object : shuffledPool) {
        if (object.randoCheckId == randoCheckId) {
            isObtained = object.obtained;
            break;
        }
    }

    return isObtained;
}

inline bool ShouldSpawnJinjoJiggy(int16_t levelId) {
    bool shouldSpawn = false;
    int16_t jinjoCount = 0;

    for (auto& pool : Rando::Logic::shuffledPool) {
        if (!pool.obtained) {
            continue;
        }

        if (Rando::StaticData::Checks[pool.shuffleCheckId].worldId != levelId) {
            continue;
        }

        if (pool.randoItemId >= RI_JINJO_BLUE && pool.randoItemId <= RI_JINJO_YELLOW) {
            jinjoCount++;
        }
    }

    if (jinjoCount == 5) {
        shouldSpawn = true;
    }

    return shouldSpawn;
}

// Regions
inline std::string LogicString(std::string condition) {
    if (condition == "true")
        return "";

    return condition;
}

struct RandoRegion {
    const char* regionName;
    int16_t levelId;
    std::map<RandoCheckId, std::pair<std::function<bool()>, std::string>> checks;
    std::map<RandoRegionId, std::pair<std::function<bool()>, std::string>> connections;
    std::vector<std::pair<RandoAccessId, std::function<bool()>>> events;
};

extern std::map<RandoRegionId, RandoRegion> Regions;

#define CHECK(check, condition)                               \
    {                                                         \
        check, {                                              \
            [] { return condition; }, LogicString(#condition) \
        }                                                     \
    }

#define CONNECTION(region, condition)                         \
    {                                                         \
        region, {                                             \
            [] { return condition; }, LogicString(#condition) \
        }                                                     \
    }

#define EVENT(randoEvent, condition)         \
    {                                        \
        randoEvent, [] { return condition; } \
    }

// Check Logic
inline bool CanAccessEvent(RandoAccessId randoAccessId) {
    bool canAccess = false;
    for (auto& [regionId, regionData] : Rando::Logic::Regions) {
        if (canAccess) {
            break;
        }
        for (auto& [accessId, logic] : regionData.events) {
            if (accessId == randoAccessId) {
                if (logic) {
                    canAccess = true;
                    break;
                }
            }
        }
    }

    return canAccess;
}

inline bool CanUseTransformation(transformation_e transId) {
    file_progress_e progressId = (file_progress_e)((transId - TRANSFORM_2_TERMITE) + FILEPROG_90_PAID_TERMITE_COST);

    if (fileProgressFlag_get(progressId)) {
        return true;
    } else {
        if (__transformation_getCost(transId) <= item_getCount(ITEM_1C_MUMBO_TOKEN)) {
            return true;
        }
    }
    return false;
}

inline bool CanOpenWorld(level_e levelId) {
    if (levelId == LEVEL_6_LAIR) {
        return false;
    }

    int32_t levelNum = levelId;

    if (levelNum > LEVEL_6_LAIR) {
        levelNum = levelId - 1;
    }

    if (fileProgressFlag_get(worldOpenFlags[levelNum - 1])) {
        return true;
    }

    int32_t puzzleCost = levelId == LEVEL_6_LAIR ? 25 : _puzzleCost(levelNum - 1);
    int32_t jiggyCount = item_getCount(ITEM_26_JIGGY_TOTAL);

    if (jiggyCount >= puzzleCost) {
        RandoAccessId puzzleBoardAccessID = RA_MAX;
        switch (levelId) {
            case LEVEL_1_MUMBOS_MOUNTAIN:
                puzzleBoardAccessID = RA_PUZZLE_BOARD_MUMBOS_MOUNTAIN;
                break;
            case LEVEL_2_TREASURE_TROVE_COVE:
                puzzleBoardAccessID = RA_PUZZLE_BOARD_TREASURE_TROVE_COVE;
                break;
            case LEVEL_3_CLANKERS_CAVERN:
                puzzleBoardAccessID = RA_PUZZLE_BOARD_CLANKERS_CAVERN;
                break;
            case LEVEL_4_BUBBLEGLOOP_SWAMP:
                puzzleBoardAccessID = RA_PUZZLE_BOARD_BUBBLEGLOOP_SWAMP;
                break;
            case LEVEL_5_FREEZEEZY_PEAK:
                puzzleBoardAccessID = RA_PUZZLE_BOARD_FREEZEEZY_PEAK;
                break;
            case LEVEL_6_LAIR:
                puzzleBoardAccessID = RA_PUZZLE_BOARD_GRUNTILDA;
                break;
            case LEVEL_7_GOBIS_VALLEY:
                puzzleBoardAccessID = RA_PUZZLE_BOARD_GOBIS_VALLEY;
                break;
            case LEVEL_8_CLICK_CLOCK_WOOD:
                puzzleBoardAccessID = RA_PUZZLE_BOARD_CLICK_CLOCK_WOOD;
                break;
            case LEVEL_9_RUSTY_BUCKET_BAY:
                puzzleBoardAccessID = RA_PUZZLE_BOARD_RUSTY_BUCKET_BAY;
                break;
            case LEVEL_A_MAD_MONSTER_MANSION:
                puzzleBoardAccessID = RA_PUZZLE_BOARD_MAD_MONSTER_MANSION;
                break;
            default:
                break;
        }
        if (puzzleBoardAccessID != RA_MAX) {
            return CanAccessEvent(puzzleBoardAccessID);
        }
    }

    return false;
}

inline bool CanBreakObject(RandoAccessId objectType) {
    bool canBreakObject = false;

    switch (objectType) {
        case RA_BREAK_OBJECT_BOULDER:
            if (ability_isUnlocked(ABILITY_0_BARGE) || ability_isUnlocked(ABILITY_6_EGGS) ||
                ability_isUnlocked(ABILITY_2_BEAK_BUSTER) || ability_isUnlocked(ABILITY_12_WONDERWING)) {
                canBreakObject = true;
            }
            break;
        case RA_BREAK_OBJECT_BRICK_WALL:
        case RA_BREAK_OBJECT_CELLAR_CASK:
        case RA_BREAK_OBJECT_WOODEN_DOOR:
            if (ability_isUnlocked(ABILITY_0_BARGE) || ability_isUnlocked(ABILITY_12_WONDERWING) ||
                ability_isUnlocked(ABILITY_B_RATATAT_RAP) || ability_isUnlocked(ABILITY_6_EGGS)) {
                canBreakObject = true;
            }
            break;
        case RA_BREAK_OBJECT_GNAWTYS_BOULDER:
            if (ability_isUnlocked(ABILITY_0_BARGE) || ability_isUnlocked(ABILITY_2_BEAK_BUSTER) ||
                ability_isUnlocked(ABILITY_6_EGGS) || ability_isUnlocked(ABILITY_12_WONDERWING)) {
                canBreakObject = true;
            }
            break;
        case RA_BREAK_OBJECT_GRATE:
            if (ability_isUnlocked(ABILITY_6_EGGS) || ability_isUnlocked(ABILITY_B_RATATAT_RAP)) {
                canBreakObject = true;
            }
            break;
        case RA_BREAK_OBJECT_IRON_GATE:
            if (ability_isUnlocked(ABILITY_0_BARGE) || ability_isUnlocked(ABILITY_12_WONDERWING) ||
                ability_isUnlocked(ABILITY_B_RATATAT_RAP)) {
                canBreakObject = true;
            }
            break;
        case RA_BREAK_OBJECT_WEB:
            canBreakObject = ability_isUnlocked(ABILITY_6_EGGS);
            break;
        case RA_BREAK_OBJECT_WINDOWS:
            if (ability_isUnlocked(ABILITY_6_EGGS) || ability_isUnlocked(ABILITY_12_WONDERWING) ||
                ability_isUnlocked(ABILITY_B_RATATAT_RAP)) {
                canBreakObject = true;
            }
            break;
        default:
            break;
    }
    
    return canBreakObject;
}

#define CAN_ACCESS(accessId) CanAccessEvent(accessId)

#define CAN_ATTACK                                                                                                   \
    (CAN_USE_ABILITY(ABILITY_B_RATATAT_RAP) || CAN_USE_ABILITY(ABILITY_4_CLAW_SWIPE) ||                              \
     CAN_USE_ABILITY(ABILITY_6_EGGS) || CAN_USE_ABILITY(ABILITY_2_BEAK_BUSTER) || CAN_USE_ABILITY(ABILITY_C_ROLL) || \
     CAN_USE_ABILITY(ABILITY_12_WONDERWING) || CAN_USE_ABILITY(ABILITY_0_BARGE))

#define CAN_EXTEND_JUMP_DISTANCE                                                           \
    (CAN_USE_ABILITY(ABILITY_7_FEATHERY_FLAP) || CAN_USE_ABILITY(ABILITY_B_RATATAT_RAP) || \
     CAN_USE_ABILITY(ABILITY_10_TALON_TROT))

#define CAN_BREAK_OBJECT(objectType) CanBreakObject(objectType)
#define CAN_UNLOCK_NOTE_DOOR(noteCount) item_getCount(ITEM_C_NOTE) >= noteCount&& CAN_ACCESS(RA_NOTE_DOOR_##noteCount)
#define CAN_UNLOCK_WORLD(levelId) CanOpenWorld(levelId)
#define CAN_USE_ABILITY(abilityId) ability_isUnlocked(abilityId)
#define CAN_USE_TRANSFORMATION(transId) CanUseTransformation(transId)
#define GET_CURRENT_TRANSFORMATION(transId) player_getTransformation() == transId


} // namespace Logic

} // namespace Rando

#endif // RANDO_LOGIC_H
