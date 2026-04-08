#ifndef RANDO_LOGIC_H
#define RANDO_LOGIC_H

#include "port/Rando/Rando.h"
#include "port/ShipUtils.h"

namespace Rando {

namespace Logic {
extern std::vector<Rando::StaticData::RandoShuffledPool> shuffledPool;


// Regions
// struct RandoRegion {
//     const char* regionName;
//     int16_t levelId;
//     std::map<RandoCheckId, std::pair<std::function<bool()>, std::string>> checks;
//     std::map<RandoRegionId, std::pair<std::function<bool()>, std::string>> connections;
// };

// extern std::map<RandoRegionId, RandoRegion> Regions;

// #define CHECK(check, condition)                               \
//     {                                                         \
//         check, {                                              \
//             [] { return condition; }, LogicString(#condition) \
//         }                                                     \
//     }

// #define CONNECTION(region, condition)                         \
//     {                                                         \
//         region, {                                             \
//             [] { return condition; }, LogicString(#condition) \
//         }                                                     \
//     }

} // namespace Logic

} // namespace Rando

#endif // RANDO_LOGIC_H