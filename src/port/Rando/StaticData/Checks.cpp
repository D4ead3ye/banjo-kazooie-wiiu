#include "StaticData.h"
#include "port/ShipUtils.h"

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
    RC(RC_UNKNOWN,          RCTYPE_UNKNOWN,     LEVEL_D_CUTSCENE,           ACTOR_1_UNKNOWN,            0, 0, 0),
    RC(RC_MM_JINJO_BLUE,    RCTYPE_JINJO,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_60_JINJO_BLUE,        -4060, 1355, 1092),
    RC(RC_MM_JINJO_GREEN,   RCTYPE_JINJO,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_62_JINJO_GREEN,       -3297, 2755, -1421),
    RC(RC_MM_JINJO_ORANGE,  RCTYPE_JINJO,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_5F_JINJO_ORANGE,      1702, -125, 2941),
    RC(RC_MM_JINJO_PINK,    RCTYPE_JINJO,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_61_JINJO_PINK,        5876, 299, 2369),
    RC(RC_MM_JINJO_YELLOW,  RCTYPE_JINJO,       LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_5E_JINJO_YELLOW,      3379, 2280, -2352),
};
// clang-format on

} // namespace StaticData
} // namespace Rando