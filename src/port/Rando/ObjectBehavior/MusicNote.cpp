#include "ObjectBehavior.h"
#include "port/Rando/Logic/Logic.h"
#include "port/Rando/CustomObject/CustomObject.h"

#include <libultraship/bridge.h>
#include "port/ui/cvar_prefixes.h"
#include <libultraship/bridge/consolevariablebridge.h>
#include "port/enhancements/events/PortEnhancements.h"
#include "port/enhancements/events/hooks/Events.h"

extern "C" {
enum map_e gsworld_getMap(void);
enum level_e map_getLevel(enum map_e map);

void item_inc(enum item_e item);
}

#define OPTION_ENABLED RANDO_SAVE_OPTIONS[RO_SHUFFLE_MUSIC_NOTES].optionValue

void Rando::ObjectBehavior::InitMusicNoteBehavior() {
    REGISTER_LISTENER(OnSetJiggyList, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnSetJiggyList* ev = (OnSetJiggyList*)event;

        if (!IS_RANDO && !OPTION_ENABLED) {
            return;
        }

        for (auto& pool : Rando::Logic::shuffledPool) {
            if (Rando::StaticData::Checks[pool.shuffledCheckId].worldId != ev->levelId) {
                continue;
            }
        
            if (pool.randoItemId == RI_MUSIC_NOTE) {
                if (pool.obtained) {
                    item_inc(ITEM_C_NOTE);
                }
            }
        }
    })
}
