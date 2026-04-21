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
    RC(RC_UNKNOWN,                                      RCTYPE_UNKNOWN,         LEVEL_D_CUTSCENE,           ACTOR_1_UNKNOWN,            NULL,                                           0, 0, 0),
    RC(RC_MM_EMPTY_HONEYCOMB_HILL,                      RCTYPE_EMPTY_HONEYCOMB, LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_47_EMPTY_HONEYCOMB,   HONEYCOMB_1_MM_HILL,                            985, 484, 1681),
    RC(RC_MM_EMPTY_HONEYCOMB_JUJU,                      RCTYPE_EMPTY_HONEYCOMB, LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_47_EMPTY_HONEYCOMB,   HONEYCOMB_2_MM_JUJU,                            4302, 2900, -1490),
    RC(RC_MM_JIGGY_CHIMPY,                              RCTYPE_JIGGY,           LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_9_MM_CHIMPY,                              -5599, -20, 5599),
    RC(RC_MM_JIGGY_CONGA,                               RCTYPE_JIGGY,           LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_A_MM_CONGA,                               -3758, -172, 5028),
    RC(RC_MM_JIGGY_HILL,                                RCTYPE_JIGGY,           LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_07_MM_HILL,                               4335, 1485, 794),
    RC(RC_MM_JIGGY_HUTS,                                RCTYPE_JIGGY,           LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_5_MM_HUTS,                                3319, 2165, -1410),
    RC(RC_MM_JIGGY_JINJO,                               RCTYPE_JIGGY,           LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_01_MM_JINJO,                              1702, -41, 2941),
    RC(RC_MM_JIGGY_JUJU,                                RCTYPE_JIGGY,           LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_4_MM_JUJU,                                4309, 2374, -1487),
    RC(RC_MM_JIGGY_MUMBOS_SKULL,                        RCTYPE_JIGGY,           LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_03_MM_MUMBOS_SKULL,                       5580, 2756, -2708),
    RC(RC_MM_JIGGY_ORANGE_PADS,                         RCTYPE_JIGGY,           LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_8_MM_ORANGE_PADS,                         -3804, -198, 5036),
    RC(RC_MM_JIGGY_RUINS,                               RCTYPE_JIGGY,           LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_06_MM_RUINS,                              -3699, 2438, -2113),
    RC(RC_MM_JIGGY_TICKERS_TOWER,                       RCTYPE_JIGGY,           LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_46_JIGGY,             JIGGY_02_MM_TICKERS_TOWER,                      373, 4221, -906),
    RC(RC_MM_JINJO_BLUE,                                RCTYPE_JINJO,           LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_60_JINJO_BLUE,        NULL,                                           1702, -125, 2941),
    RC(RC_MM_JINJO_GREEN,                               RCTYPE_JINJO,           LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_62_JINJO_GREEN,       NULL,                                           4599, 2205, -826),
    RC(RC_MM_JINJO_ORANGE,                              RCTYPE_JINJO,           LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_5F_JINJO_ORANGE,      NULL,                                           -3297, 2755, -1421),
    RC(RC_MM_JINJO_PINK,                                RCTYPE_JINJO,           LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_61_JINJO_PINK,        NULL,                                           5876, 299, 2369),
    RC(RC_MM_JINJO_YELLOW,                              RCTYPE_JINJO,           LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_5E_JINJO_YELLOW,      NULL,                                           -4060, 1355, 1092),
    RC(RC_MM_MOLEHILL_BEAK_BUSTER,                      RCTYPE_MOLEHILL,        LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_12C_MOLEHILL,         ABILITY_2_BEAK_BUSTER,                          3943, 2155, -3422),
    RC(RC_MM_MOLEHILL_TALON_TROT,                       RCTYPE_MOLEHILL,        LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_12C_MOLEHILL,         ABILITY_10_TALON_TROT,                          -2550, 2255, -1217),
    RC(RC_MM_MOLEHILL_USE_EGGS,                         RCTYPE_MOLEHILL,        LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_12C_MOLEHILL,         ABILITY_6_EGGS,                                 -5926, 200, 6213),
    RC(RC_MM_MUMBO_TOKEN_BEHIND_PINK_JINJO,             RCTYPE_MUMBO_TOKEN,     LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_2D_MUMBO_TOKEN,       MUMBOTOKEN_04_MM_BEHIND_PINK_JINJO,             5855, 0, 2169),
    RC(RC_MM_MUMBO_TOKEN_BEHIND_RUINS,                  RCTYPE_MUMBO_TOKEN,     LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_2D_MUMBO_TOKEN,       MUMBOTOKEN_02_MM_BEHIND_RUINS,                  -4869, 2255, -2506),
    RC(RC_MM_MUMBO_TOKEN_INSIDE_TICKERS_TOWER,          RCTYPE_MUMBO_TOKEN,     LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_2D_MUMBO_TOKEN,       MUMBOTOKEN_05_MM_INSIDE_TICKERS_TOWER,          -343, 345, -518),
    RC(RC_MM_MUMBO_TOKEN_STUMP_NEAR_CONGA,              RCTYPE_MUMBO_TOKEN,     LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_2D_MUMBO_TOKEN,       MUMBOTOKEN_01_MM_STUMP_NEAR_CONGA,              -4431, 475, 6183),
    RC(RC_MM_MUMBO_TOKEN_UNDER_RAMP_TO_MUMBOS_SKULL,    RCTYPE_MUMBO_TOKEN,     LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_2D_MUMBO_TOKEN,       MUMBOTOKEN_03_MM_UNDER_RAMP_TO_MUMBOS_SKULL,    5150, 2168, -2447),
    RC(RC_MM_NOTE_SOUTH_SLOPE_FIRST_PLATFORM_1,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           4867, 297, 1766),
    RC(RC_MM_NOTE_SOUTH_SLOPE_FIRST_PLATFORM_2,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           4782, 298, 1892),
    RC(RC_MM_NOTE_SOUTH_SLOPE_FIRST_PLATFORM_3,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           4971, 299, 1887),
    RC(RC_MM_NOTE_SOUTH_SLOPE_SECOND_PLATFORM_1,        RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           6430, 500, 1410),
    RC(RC_MM_NOTE_SOUTH_SLOPE_SECOND_PLATFORM_2,        RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           6310, 500, 1536),
    RC(RC_MM_NOTE_SOUTH_SLOPE_SECOND_PLATFORM_3,        RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           6546, 500, 1536),
    RC(RC_MM_NOTE_SOUTH_SLOPE_THIRD_PLATFORM_1,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           3793, 747, 1471),
    RC(RC_MM_NOTE_SOUTH_SLOPE_THIRD_PLATFORM_2,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           3974, 747, 1411),
    RC(RC_MM_NOTE_SOUTH_SLOPE_THIRD_PLATFORM_3,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           3928, 747, 1522),
    RC(RC_MM_NOTE_SOUTH_SLOPE_FOURTH_PLATFORM_1,        RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           5259, 1184, 656),
    RC(RC_MM_NOTE_SOUTH_SLOPE_FOURTH_PLATFORM_2,        RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           5469, 1184, 549),
    RC(RC_MM_NOTE_SOUTH_SLOPE_FOURTH_PLATFORM_3,        RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           5412, 1184, 681),
    RC(RC_MM_NOTE_SOUTH_SLOPE_FIFTH_PLATFORM_1,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           3202, 1247, 925),
    RC(RC_MM_NOTE_SOUTH_SLOPE_FIFTH_PLATFORM_2,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           3126, 1247, 1014),
    RC(RC_MM_NOTE_SOUTH_SLOPE_FIFTH_PLATFORM_3,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           3282, 1247, 1003),
    RC(RC_MM_NOTE_SOUTH_SLOPE_SIXTH_PLATFORM_1,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           5860, 1547, 62),
    RC(RC_MM_NOTE_SOUTH_SLOPE_SIXTH_PLATFORM_2,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           6098, 1547, 67),
    RC(RC_MM_NOTE_SOUTH_SLOPE_SIXTH_PLATFORM_3,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           5990, 1547, 169),
    RC(RC_MM_NOTE_BRIDGE_1,                             RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           2585, -20, 3800),
    RC(RC_MM_NOTE_BRIDGE_2,                             RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           2298, -65, 3800),
    RC(RC_MM_NOTE_BRIDGE_3,                             RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           2012, -90, 3800),
    RC(RC_MM_NOTE_BRIDGE_4,                             RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           1721, -90, 3800),
    RC(RC_MM_NOTE_BRIDGE_5,                             RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           1441, -90, 3800),
    RC(RC_MM_NOTE_BRIDGE_6,                             RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           1149, -62, 3800),
    RC(RC_MM_NOTE_BRIDGE_7,                             RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           854,  -8,  3800),
    RC(RC_MM_NOTE_NORTH_SLOPE_FIRST_PLATFORM_1,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -4497, 393, 2503),
    RC(RC_MM_NOTE_NORTH_SLOPE_FIRST_PLATFORM_2,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -4583, 393, 2600),
    RC(RC_MM_NOTE_NORTH_SLOPE_FIRST_PLATFORM_3,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -4417, 393, 2600),
    RC(RC_MM_NOTE_NORTH_SLOPE_SECOND_PLATFORM_1,        RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -3730, 782, 1838),
    RC(RC_MM_NOTE_NORTH_SLOPE_SECOND_PLATFORM_2,        RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -3776, 782, 1964),
    RC(RC_MM_NOTE_NORTH_SLOPE_SECOND_PLATFORM_3,        RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -3624, 782, 1908),
    RC(RC_MM_NOTE_NORTH_SLOPE_THIRD_PLATFORM_1,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -5185, 837, 1737),
    RC(RC_MM_NOTE_NORTH_SLOPE_THIRD_PLATFORM_2,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -5277, 836, 1850),
    RC(RC_MM_NOTE_NORTH_SLOPE_THIRD_PLATFORM_3,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -5099, 836, 1850),
    RC(RC_MM_NOTE_NORTH_SLOPE_FOURTH_PLATFORM_1,        RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -2962, 1397, 813),
    RC(RC_MM_NOTE_NORTH_SLOPE_FOURTH_PLATFORM_2,        RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -3053, 1397, 905),
    RC(RC_MM_NOTE_NORTH_SLOPE_FOURTH_PLATFORM_3,        RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -2868, 1397, 890),
    RC(RC_MM_NOTE_NORTH_SLOPE_FIFTH_PLATFORM_1,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -5154, 1489, 462),
    RC(RC_MM_NOTE_NORTH_SLOPE_FIFTH_PLATFORM_2,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -5238, 1489, 560),
    RC(RC_MM_NOTE_NORTH_SLOPE_FIFTH_PLATFORM_3,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -5068, 1489, 560),
    RC(RC_MM_NOTE_NORTH_SLOPE_SIXTH_PLATFORM_1,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -4260, 1839, 208),
    RC(RC_MM_NOTE_NORTH_SLOPE_SIXTH_PLATFORM_2,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -4342, 1839, 300),
    RC(RC_MM_NOTE_NORTH_SLOPE_SIXTH_PLATFORM_3,         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -4179, 1839, 300),
    RC(RC_MM_NOTE_NORTH_SLOPE_SEVENTH_PLATFORM_1,       RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -3394, 1910, 171),
    RC(RC_MM_NOTE_NORTH_SLOPE_SEVENTH_PLATFORM_2,       RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -3469, 1910, 263),
    RC(RC_MM_NOTE_NORTH_SLOPE_SEVENTH_PLATFORM_3,       RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -3293, 1910, 247),
    RC(RC_MM_NOTE_NORTH_SLOPE_LOWER_TRAIL_1,            RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -1373, 169, 1957),
    RC(RC_MM_NOTE_NORTH_SLOPE_LOWER_TRAIL_2,            RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -1431, 346, 1784),
    RC(RC_MM_NOTE_NORTH_SLOPE_LOWER_TRAIL_3,            RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -1497, 517, 1621),
    RC(RC_MM_NOTE_NORTH_SLOPE_LOWER_TRAIL_4,            RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -1528, 638, 1424),
    RC(RC_MM_NOTE_NORTH_SLOPE_LOWER_TRAIL_5,            RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -1523, 742, 1179),
    RC(RC_MM_NOTE_NORTH_SLOPE_LOWER_TRAIL_6,            RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -1523, 857, 932),
    RC(RC_MM_NOTE_NORTH_SLOPE_LOWER_TRAIL_7,            RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -1518, 964, 706),
    RC(RC_MM_NOTE_NORTH_SLOPE_LOWER_TRAIL_8,            RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -1413, 1117, 500),
    RC(RC_MM_NOTE_NORTH_SLOPE_LOWER_TRAIL_9,            RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -1284, 1295, 363),
    RC(RC_MM_NOTE_NORTH_SLOPE_HIGHER_TRAIL_1,           RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -1094, 1727, -1922),
    RC(RC_MM_NOTE_NORTH_SLOPE_HIGHER_TRAIL_2,           RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -1309, 1856, -1941),
    RC(RC_MM_NOTE_NORTH_SLOPE_HIGHER_TRAIL_3,           RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -1541, 1987, -1960),
    RC(RC_MM_NOTE_NORTH_SLOPE_HIGHER_TRAIL_4,           RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -1758, 2111, -1974),
    RC(RC_MM_NOTE_RUINS_1,                              RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -3047, 2755, -2669),
    RC(RC_MM_NOTE_RUINS_2,                              RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -3314, 2755, -2802),
    RC(RC_MM_NOTE_RUINS_3,                              RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -3603, 2755, -2923),
    RC(RC_MM_NOTE_RUINS_4,                              RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -3885, 2755, -2901),
    RC(RC_MM_NOTE_RUINS_5,                              RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -4161, 2755, -2858),
    RC(RC_MM_NOTE_RUINS_6,                              RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -4359, 2755, -2682),
    RC(RC_MM_NOTE_RUINS_7,                              RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -4542, 2755, -2495),
    RC(RC_MM_NOTE_RUINS_8,                              RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -4599, 2754, -2261),
    RC(RC_MM_NOTE_RUINS_9,                              RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -4649, 2755, -2009),
    RC(RC_MM_NOTE_RUINS_10,                             RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -4539, 2755, -1746),
    RC(RC_MM_NOTE_RUINS_11,                             RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -4416, 2755, -1509),
    RC(RC_MM_NOTE_RUINS_12,                             RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -4167, 2755, -1383),
    RC(RC_MM_NOTE_RUINS_13,                             RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -3892, 2755, -1284),
    RC(RC_MM_NOTE_RUINS_14,                             RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -3581, 2755, -1359),
    RC(RC_MM_NOTE_UNDERWATER_RIGHT_CAVE_1,              RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           2128, -824, 2181),
    RC(RC_MM_NOTE_UNDERWATER_RIGHT_CAVE_2,              RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           2034, -824, 2158),
    RC(RC_MM_NOTE_UNDERWATER_RIGHT_CAVE_3,              RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           1937, -824, 2136),
    RC(RC_MM_NOTE_UNDERWATER_LEFT_CAVE_1,               RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           1027, -824, 2183),
    RC(RC_MM_NOTE_UNDERWATER_LEFT_CAVE_2,               RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           940, -824, 2223),
    RC(RC_MM_NOTE_UNDERWATER_LEFT_CAVE_3,               RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           850, -824, 2267),
    RC(RC_MM_NOTE_HUT_ROOF_1,                           RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           3378, 2439, -2353),
    RC(RC_MM_NOTE_HUT_ROOF_2,                           RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           4137, 2439, -2649),
    RC(RC_MM_NOTE_HUT_ROOF_3,                           RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           5319, 2439, -1717),
    RC(RC_MM_NOTE_HUT_ROOF_4,                           RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           5025, 2439, -814),
    RC(RC_MM_NOTE_HUT_ROOF_5,                           RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           3847, 2439, -715),
    RC(RC_MM_NOTE_HUT_ROOF_6,                           RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           3255, 2438, -1091),
    RC(RC_MM_NOTE_HUT_BUNDLE_1,                         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           5321, 2280, -1715),
    RC(RC_MM_NOTE_HUT_BUNDLE_2,                         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           5321, 2280, -1715),
    RC(RC_MM_NOTE_HUT_BUNDLE_3,                         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           5321, 2280, -1715),
    RC(RC_MM_NOTE_HUT_BUNDLE_4,                         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           5321, 2280, -1715),
    RC(RC_MM_NOTE_HUT_BUNDLE_5,                         RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           5321, 2280, -1715),
    RC(RC_MM_NOTE_INSIDE_MUMBOS_SKULL_1,                RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -288, 0, -154),
    RC(RC_MM_NOTE_INSIDE_MUMBOS_SKULL_2,                RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -127, 0, -308),
    RC(RC_MM_NOTE_INSIDE_MUMBOS_SKULL_3,                RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           119, 0, -298),
    RC(RC_MM_NOTE_INSIDE_MUMBOS_SKULL_4,                RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           286, 0, -155),
    RC(RC_MM_NOTE_INSIDE_TICKERS_TOWER_1,               RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -74, 817, 19),
    RC(RC_MM_NOTE_INSIDE_TICKERS_TOWER_2,               RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -3, 817, 52),
    RC(RC_MM_NOTE_INSIDE_TICKERS_TOWER_3,               RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           67, 817, 0),
    RC(RC_MM_NOTE_INSIDE_TICKERS_TOWER_4,               RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           60, 817, -87),
    RC(RC_MM_NOTE_INSIDE_TICKERS_TOWER_5,               RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -10, 817, -125),
    RC(RC_MM_NOTE_INSIDE_TICKERS_TOWER_6,               RCTYPE_MUSIC_NOTE,      LEVEL_1_MUMBOS_MOUNTAIN,    ACTOR_51_MUSIC_NOTE,        NULL,                                           -80, 817, -71),
    RC(RC_SM_EMPTY_HONEYCOMB_COLLIWOBBLE,               RCTYPE_EMPTY_HONEYCOMB, LEVEL_B_SPIRAL_MOUNTAIN,    ACTOR_47_EMPTY_HONEYCOMB,   HONEYCOMB_17_SM_COLLIWOBBLE,                    3878, 638, 1281),
    RC(RC_SM_EMPTY_HONEYCOMB_QUARRIES,                  RCTYPE_EMPTY_HONEYCOMB, LEVEL_B_SPIRAL_MOUNTAIN,    ACTOR_47_EMPTY_HONEYCOMB,   HONEYCOMB_18_SM_QUARRIES,                       3948, -108, -1785),
    RC(RC_SM_EMPTY_HONEYCOMB_STUMP,                     RCTYPE_EMPTY_HONEYCOMB, LEVEL_B_SPIRAL_MOUNTAIN,    ACTOR_47_EMPTY_HONEYCOMB,   HONEYCOMB_13_SM_STUMP,                          -1400, 99, 4824),
    RC(RC_SM_EMPTY_HONEYCOMB_TREE,                      RCTYPE_EMPTY_HONEYCOMB, LEVEL_B_SPIRAL_MOUNTAIN,    ACTOR_47_EMPTY_HONEYCOMB,   HONEYCOMB_16_SM_TREE,                           -2752, 1299, 37),
    RC(RC_SM_EMPTY_HONEYCOMB_UNDERWATER,                RCTYPE_EMPTY_HONEYCOMB, LEVEL_B_SPIRAL_MOUNTAIN,    ACTOR_47_EMPTY_HONEYCOMB,   HONEYCOMB_15_SM_UNDERWATER,                     -264, -442, -651),
    RC(RC_SM_EMPTY_HONEYCOMB_WATERFALL,                 RCTYPE_EMPTY_HONEYCOMB, LEVEL_B_SPIRAL_MOUNTAIN,    ACTOR_47_EMPTY_HONEYCOMB,   HONEYCOMB_14_SM_WATERFALL,                      -6725, 1300, -1803),
    RC(RC_SM_MOLEHILL_ATTACK,                           RCTYPE_MOLEHILL,        LEVEL_B_SPIRAL_MOUNTAIN,    ACTOR_12C_MOLEHILL,         ABILITY_4_CLAW_SWIPE,                           4675, 400, 511),
    RC(RC_SM_MOLEHILL_BEAK_BARGE,                       RCTYPE_MOLEHILL,        LEVEL_B_SPIRAL_MOUNTAIN,    ACTOR_12C_MOLEHILL,         ABILITY_0_BARGE,                                3271, -110, -2673),
    RC(RC_SM_MOLEHILL_CAMERA_CONTROL,                   RCTYPE_MOLEHILL,        LEVEL_B_SPIRAL_MOUNTAIN,    ACTOR_12C_MOLEHILL,         ABILITY_3_CAMERA_CONTROL,                       2250, -34, 2774	),
    RC(RC_SM_MOLEHILL_CLIMB,                            RCTYPE_MOLEHILL,        LEVEL_B_SPIRAL_MOUNTAIN,    ACTOR_12C_MOLEHILL,         ABILITY_5_CLIMB,                                -1999, 342, -2545),
    RC(RC_SM_MOLEHILL_DIVE,                             RCTYPE_MOLEHILL,        LEVEL_B_SPIRAL_MOUNTAIN,    ACTOR_12C_MOLEHILL,         ABILITY_F_DIVE,                                 -2336, 302, 973),
    RC(RC_SM_MOLEHILL_JUMP,                             RCTYPE_MOLEHILL,        LEVEL_B_SPIRAL_MOUNTAIN,    ACTOR_12C_MOLEHILL,         ABILITY_A_HOLD_A_JUMP_HIGHER,                   -164, -190, 3313),
};
// clang-format on

RandoCheckId GetCheckByPosition(int32_t posX, int32_t posY, int32_t posZ) {
    for (auto& [randoCheckId, randoStaticCheck] : Checks) {
        if (randoStaticCheck.posX == posX && randoStaticCheck.posY == posY && randoStaticCheck.posZ == posZ) {
            return randoCheckId;
        }
    }

    return RC_UNKNOWN;
}

RandoCheckId GetCheckByJiggyId(int32_t jiggyId) {
    for (auto& [randoCheckId, randoStaticCheck] : Checks) {
        if (randoStaticCheck.randoCheckType != RCTYPE_JIGGY) {
            continue;
        }

        if (randoStaticCheck.collectionId == jiggyId) {
            return randoCheckId;
        }
    }

    return RC_UNKNOWN;
}

} // namespace StaticData
} // namespace Rando