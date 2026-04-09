#include "StaticData.h"
#include "port/ShipUtils.h"

#include "prop.h"

namespace Rando {

namespace StaticData {
std::array<std::string, RC_MAX> CheckNames = std::array<std::string, RC_MAX>();

#define RC(id, type, world, actor, cId, x, y, z)      \
    {                                                 \
        id, {                                         \
            id, #id, type, world, actor, cId, x, y, z \
        }                                             \
    }

// clang-format off
std::map<RandoCheckId, RandoStaticCheck> Checks = {
    RC(RC_UNKNOWN,                                      RCTYPE_UNKNOWN,     LEVEL_D_CUTSCENE,           ACTOR_1_UNKNOWN,            NULL,                                           0, 0, 0),
    RC(RC_MM_JIGGY_CHIMPY,                              RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_9_MM_CHIMPY,                              -5599, -20, 5599),
    RC(RC_MM_JIGGY_CONGA,                               RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_A_MM_CONGA,                               -3758, -172, 5028),
    RC(RC_MM_JIGGY_HILL,                                RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_07_MM_HILL,                               4335, 1485, 794),
    RC(RC_MM_JIGGY_HUTS,                                RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_5_MM_HUTS,                                3319, 2165, -1410), //This one changes
    RC(RC_MM_JIGGY_JINJO,                               RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_01_MM_JINJO,                              1702, -41, 2941), //This one changes
    RC(RC_MM_JIGGY_JUJU,                                RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_4_MM_JUJU,                                4309, 2355, -1487),
    RC(RC_MM_JIGGY_MUMBOS_SKULL,                        RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_03_MM_MUMBOS_SKULL,                       5580, 2756, -2708),
    RC(RC_MM_JIGGY_ORANGE_PADS,                         RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_8_MM_ORANGE_PADS,                         -3804, -198, 5036), //This one changes
    RC(RC_MM_JIGGY_RUINS,                               RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_06_MM_RUINS,                              -3699, 2438, -2113),
    RC(RC_MM_JIGGY_TICKERS_TOWER,                       RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_02_MM_TICKERS_TOWER,                      373, 4221, -906),
    RC(RC_MM_JINJO_BLUE,                                RCTYPE_JINJO,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_60_JINJO_BLUE,        NULL,                                           -4060, 1355, 1092),
    RC(RC_MM_JINJO_GREEN,                               RCTYPE_JINJO,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_62_JINJO_GREEN,       NULL,                                           -3297, 2755, -1421),
    RC(RC_MM_JINJO_ORANGE,                              RCTYPE_JINJO,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_5F_JINJO_ORANGE,      NULL,                                           1702, -125, 2941),
    RC(RC_MM_JINJO_PINK,                                RCTYPE_JINJO,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_61_JINJO_PINK,        NULL,                                           5876, 299, 2369),
    RC(RC_MM_JINJO_YELLOW,                              RCTYPE_JINJO,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_5E_JINJO_YELLOW,      NULL,                                           3379, 2280, -2352),
    RC(RC_MM_MUMBO_TOKEN_BEHIND_PINK_JINJO,             RCTYPE_MUMBO_TOKEN, LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_2D_MUMBO_TOKEN,       MUMBOTOKEN_04_MM_BEHIND_PINK_JINJO,             5855, 0, 2169),
    RC(RC_MM_MUMBO_TOKEN_BEHIND_RUINS,                  RCTYPE_MUMBO_TOKEN, LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_2D_MUMBO_TOKEN,       MUMBOTOKEN_02_MM_BEHIND_RUINS,                  -4869, 2255, -2506),
    RC(RC_MM_MUMBO_TOKEN_INSIDE_TICKERS_TOWER,          RCTYPE_MUMBO_TOKEN, LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_2D_MUMBO_TOKEN,       MUMBOTOKEN_05_MM_INSIDE_TICKERS_TOWER,          -343, 345, -518),
    RC(RC_MM_MUMBO_TOKEN_STUMP_NEAR_CONGA,              RCTYPE_MUMBO_TOKEN, LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_2D_MUMBO_TOKEN,       MUMBOTOKEN_01_MM_STUMP_NEAR_CONGA,              -4431, 475, 6183),
    RC(RC_MM_MUMBO_TOKEN_UNDER_RAMP_TO_MUMBOS_SKULL,    RCTYPE_MUMBO_TOKEN, LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_2D_MUMBO_TOKEN,       MUMBOTOKEN_03_MM_UNDER_RAMP_TO_MUMBOS_SKULL,    5150, 2168, -2447),
};
// clang-format on

RandoCheckId GetCheckByPosition(std::array<int32_t, 3> position) {
    for (auto& [randoCheckId, randoStaticCheck] : Checks) {
        std::array<int32_t, 3> checkPosition = {
            randoStaticCheck.posX,
            randoStaticCheck.posY,
            randoStaticCheck.posZ,
        };

        if (checkPosition == position) {
            return randoCheckId;
        }
    }

    return RC_UNKNOWN;
}

} // namespace StaticData
} // namespace Rando