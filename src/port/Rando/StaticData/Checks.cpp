#include "StaticData.h"
#include "port/ShipUtils.h"

#include "prop.h"

namespace Rando {

namespace StaticData {
std::array<std::string, RC_MAX> CheckNames = std::array<std::string, RC_MAX>();

#define RC(id, type, world, actor, x, y, z)      \
    {                                            \
        id, {                                    \
            id, #id, type, world, actor, x, y, z \
        }                                        \
    }

// clang-format off
std::map<RandoCheckId, RandoStaticCheck> Checks = {
    RC(RC_UNKNOWN,                  RCTYPE_UNKNOWN,     LEVEL_D_CUTSCENE,               ACTOR_1_UNKNOWN,            0, 0, 0),
    RC(RC_MM_JIGGY_CHIMPY,          RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,        ACTOR_46_JIGGY,             -5599, -20, 5599),
    RC(RC_MM_JIGGY_CONGA,           RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,        ACTOR_46_JIGGY,             -3758, -172, 5028),
    RC(RC_MM_JIGGY_HILL,            RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,        ACTOR_46_JIGGY,             4335, 1485, 794),
    RC(RC_MM_JIGGY_HUTS,            RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,        ACTOR_46_JIGGY,             3319, 2165, -1410), //This one changes
    RC(RC_MM_JIGGY_JINJO,           RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,        ACTOR_46_JIGGY,             1702, -41, 2941), //This one changes
    RC(RC_MM_JIGGY_JUJU,            RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,        ACTOR_46_JIGGY,             4309, 2355, -1487),
    RC(RC_MM_JIGGY_MUMBOS_SKULL,    RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,        ACTOR_46_JIGGY,             5580, 2756, -2708),
    RC(RC_MM_JIGGY_ORANGE_PADS,     RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,        ACTOR_46_JIGGY,             -3804, -198, 5036), //This one changes
    RC(RC_MM_JIGGY_RUINS,           RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,        ACTOR_46_JIGGY,             -3699, 2438, -2113),
    RC(RC_MM_JIGGY_TICKERS_TOWER,   RCTYPE_JIGGY,       LEVEL_1_MUMBOS_MOUNTAIN,        ACTOR_46_JIGGY,             373, 4221, -906),
    RC(RC_MM_JINJO_BLUE,            RCTYPE_JINJO,       LEVEL_1_MUMBOS_MOUNTAIN,        ACTOR_60_JINJO_BLUE,        -4060, 1355, 1092),
    RC(RC_MM_JINJO_GREEN,           RCTYPE_JINJO,       LEVEL_1_MUMBOS_MOUNTAIN,        ACTOR_62_JINJO_GREEN,       -3297, 2755, -1421),
    RC(RC_MM_JINJO_ORANGE,          RCTYPE_JINJO,       LEVEL_1_MUMBOS_MOUNTAIN,        ACTOR_5F_JINJO_ORANGE,      1702, -125, 2941),
    RC(RC_MM_JINJO_PINK,            RCTYPE_JINJO,       LEVEL_1_MUMBOS_MOUNTAIN,        ACTOR_61_JINJO_PINK,        5876, 299, 2369),
    RC(RC_MM_JINJO_YELLOW,          RCTYPE_JINJO,       LEVEL_1_MUMBOS_MOUNTAIN,        ACTOR_5E_JINJO_YELLOW,      3379, 2280, -2352),
};
// clang-format on

} // namespace StaticData
} // namespace Rando