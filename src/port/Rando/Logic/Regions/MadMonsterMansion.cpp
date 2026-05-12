#include "src/port/ShipInit.hpp"
#include "src/port/Rando/Logic/Logic.h"

using namespace Rando::Logic;

// clang-format off
static RegisterShipInitFunc initFunc([]() {
    Regions[RR_MUMBOS_MOUNTAIN] = RandoRegion{ .regionName = "Mumbo's Mountain", .levelId = LEVEL_1_MUMBOS_MOUNTAIN,
        .checks = {

        },
    };


}, {});
// clang-format on