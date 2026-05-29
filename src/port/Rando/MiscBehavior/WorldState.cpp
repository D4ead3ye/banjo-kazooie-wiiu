#include "MiscBehavior.h"
#include "port/enhancements/events/hooks/Events.h"

void Rando::MiscBehavior::InitWorldStateBehavior() {
    REGISTER_LISTENER(OnSetJiggyList, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnSetJiggyList* ev = (OnSetJiggyList*)event;
		
		if (!IS_RANDO) {
			return;
		}

		switch (ev->levelId) {
			
            default:
                break;
        }
	})
}