#include "MiscBehavior.h"
#include <libultraship/bridge/consolevariablebridge.h>
#include "port/enhancements/events/hooks/Events.h"
#include "port/ui/Notification.h"

#include "port/save/Types.h"

#include "port/Rando/Logic/Logic.h"
// #include "port/Rando/Spoiler/Spoiler.h"

#include "spdlog/spdlog.h"

extern "C" {
enum map_e gsworld_getMap(void);
}

void Rando::MiscBehavior::OnFileLoad() {
    REGISTER_LISTENER(OnGameLoad, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnGameLoad* ev = (OnGameLoad*)event;
        selectedFileNum = ev->fileNum;
    });

    REGISTER_LISTENER(OnSaveLoad, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnSaveLoad* ev = (OnSaveLoad*)event;
        SaveData* saveData = (SaveData*)ev->saveData;

        Rando::Logic::shuffledPool.clear();

        if (saveData->magic != 0) {
            if (saveData->shipSaveData.fileType == FILE_TYPE_SAVE_RANDO) {
                Rando::Logic::GeneratePoolFromSaveData(saveData);
            }
            return;
        }
        
        if (CVarGetInteger("gRandoSettings.Enable", 0)) {
            Rando::Logic::GenerateShufflePool();
            Rando::Logic::InitializeSaveData(saveData);
            saveData->shipSaveData.fileType = FILE_TYPE_SAVE_RANDO;
        }
    });

    REGISTER_LISTENER(OnSaveFileLoad, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnSaveFileLoad* ev = (OnSaveFileLoad*)event;

        selectedFileNum = DEFAULT_FILE_NUM;
    });
}
