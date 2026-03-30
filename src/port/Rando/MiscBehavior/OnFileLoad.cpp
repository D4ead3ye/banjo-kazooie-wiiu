#include "MiscBehavior.h"
#include "port/enhancements/events/hooks/Events.h"
#include "port/ui/Notification.h"

// #include "port/Rando/Logic/Logic.h"
// #include "port/Rando/Spoiler/Spoiler.h"

void Rando::MiscBehavior::OnFileLoad() {
    REGISTER_LISTENER(OnGameFileLoad, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnGameFileLoad* ev = (OnGameFileLoad*)event;
        selectedFileNum = ev->fileNum;
    });
}
