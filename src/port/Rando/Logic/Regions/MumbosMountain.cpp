#include "src/port/ShipInit.hpp"
#include "src/port/Rando/Logic/Logic.h"

using namespace Rando::Logic;

// clang-format off
static RegisterShipInitFunc initFunc([]() {
    Regions[RR_MUMBOS_MOUNTAIN] = RandoRegion{ .regionName = "Mumbo's Mountain", .levelId = LEVEL_1_MUMBOS_MOUNTAIN,
        .checks = {
            CHECK(RC_MM_EMPTY_HONEYCOMB_HILL,                   true),
            CHECK(RC_MM_EMPTY_HONEYCOMB_JUJU,                   true),
            CHECK(RC_MM_JIGGY_CHIMPY,                           true),
            CHECK(RC_MM_JIGGY_CONGA,                            true),
            CHECK(RC_MM_JIGGY_HILL,                             true),
            CHECK(RC_MM_JIGGY_HUTS,                             true),
            CHECK(RC_MM_JIGGY_JINJO,                            true),
            CHECK(RC_MM_JIGGY_JUJU,                             true),
            CHECK(RC_MM_JIGGY_MUMBOS_SKULL,                     true),
            CHECK(RC_MM_JIGGY_ORANGE_PADS,                      true),
            CHECK(RC_MM_JIGGY_RUINS,                            true),
            CHECK(RC_MM_JIGGY_TICKERS_TOWER,                    true),
            CHECK(RC_MM_JINJO_BLUE,                             true),
            CHECK(RC_MM_JINJO_GREEN,                            true),
            CHECK(RC_MM_JINJO_ORANGE,                           true),
            CHECK(RC_MM_JINJO_PINK,                             true),
            CHECK(RC_MM_JINJO_YELLOW,                           true),
            CHECK(RC_MM_MUMBO_TOKEN_BEHIND_PINK_JINJO,          true),
            CHECK(RC_MM_MUMBO_TOKEN_BEHIND_RUINS,               true),
            CHECK(RC_MM_MUMBO_TOKEN_INSIDE_TICKERS_TOWER,       true),
            CHECK(RC_MM_MUMBO_TOKEN_STUMP_NEAR_CONGA,           true),
            CHECK(RC_MM_MUMBO_TOKEN_UNDER_RAMP_TO_MUMBOS_SKULL, true),
        },
    };
}, {});
// clang-format on