#include "StaticData.h"
#include <libultraship/bridge/consolevariablebridge.h>
#include "port/ShipUtils.h"
#include "port/Rando/Rando.h"
// #include "port/Rando/Logic/Logic.h"

#include "enums.h"

namespace Rando {

namespace StaticData {

#define RI(id, article, name, type, actorId)      \
    {                                             \
        id, {                                     \
            id, #id, article, name, type, actorId \
        }                                         \
    }

// clang-format off
std::map<RandoItemId, RandoStaticItem> Items = {
    RI(RI_UNKNOWN,      "",     "Unknown",      RITYPE_UNKNOWN,     ACTOR_1_UNKNOWN),
    RI(RI_JIGGY,        "a",    "Jiggy",        RITYPE_JIGGY,       ACTOR_46_JIGGY),
    RI(RI_JINJO_BLUE,   "a",    "Blue Jinjo",   RITYPE_JINJO,       ACTOR_60_JINJO_BLUE),
    RI(RI_JINJO_GREEN,  "a",    "Green Jinjo",  RITYPE_JINJO,       ACTOR_62_JINJO_GREEN),
    RI(RI_JINJO_ORANGE, "a",    "Orange Jinjo", RITYPE_JINJO,       ACTOR_5F_JINJO_ORANGE),
    RI(RI_JINJO_PINK,   "a",    "Pink Jinjo",   RITYPE_JINJO,       ACTOR_61_JINJO_PINK),
    RI(RI_JINJO_YELLOW, "a",    "Yellow Jinjo", RITYPE_JINJO,       ACTOR_5E_JINJO_YELLOW),
};
// clang-format on

RandoItemId GetRandoItemByActorId(actor_e actorId) {
    
}

// RandoItemId GetShuffledRandoItem(RandoCheckId randoCheckId) {
//     for (auto& entry : Rando::Logic::shuffledPool) {
//         if (entry.randoCheckId == randoCheckId) {
//             return entry.randoItemId;
//         }
//     }
//     return RI_UNKNOWN;
// }

} // namespace StaticData
} // namespace Rando