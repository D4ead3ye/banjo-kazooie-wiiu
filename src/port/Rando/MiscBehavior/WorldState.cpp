#include "MiscBehavior.h"
#include "port/enhancements/events/hooks/Events.h"

extern "C" {
void mapSpecificFlags_set(s32 i, s32 val);

enum map_e gsworld_getMap(void);
enum level_e map_getLevel(enum map_e map);
}

void Rando::MiscBehavior::InitWorldStateBehavior() {
    REGISTER_LISTENER(OnActorSpawn, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnActorSpawn* ev = (OnActorSpawn*)event;
		
		if (!IS_RANDO) {
			return;
		}

        level_e currentLevel = map_getLevel(gsworld_getMap());

		switch (currentLevel) {
            case LEVEL_1_MUMBOS_MOUNTAIN:
                if (RANDO_SAVE_CHECKS[RC_MM_JIGGY_CHIMPY].obtained) {
                    if (ev->actorId == ACTOR_F_CHIMPY) {
                        event->Cancelled = true;
                        ev->result = NULL;
                    }
                }
                mapSpecificFlags_set(MM_SPECIFIC_FLAG_0_CHIMPY_STUMP_RAISED, RANDO_SAVE_CHECKS[RC_MM_JIGGY_CHIMPY].obtained);
                mapSpecificFlags_set(MM_SPECIFIC_FLAG_2_ORANGE_HAS_BEEN_RETURNED, RANDO_SAVE_CHECKS[RC_MM_JIGGY_CHIMPY].obtained);
                mapSpecificFlags_set(MM_SPECIFIC_FLAG_3_CHIMPY_HAS_LEFT, RANDO_SAVE_CHECKS[RC_MM_JIGGY_CHIMPY].obtained);
                break;
            default:
                break;
        }
	})
}