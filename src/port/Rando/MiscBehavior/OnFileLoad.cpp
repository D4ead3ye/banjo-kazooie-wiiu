#include "MiscBehavior.h"
#include <libultraship/bridge/consolevariablebridge.h>
#include "port/enhancements/events/hooks/Events.h"
#include "port/ui/Notification.h"

#include "port/save/Types.h"

// #include "port/Rando/Logic/Logic.h"
// #include "port/Rando/Spoiler/Spoiler.h"

extern "C" {
bool gameFile_isNotEmpty(s32 gamenum);
}

void Test() {
    int hi = 0;
}

void Rando::MiscBehavior::OnFileLoad() {
    REGISTER_LISTENER(OnSaveCreate, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnSaveCreate* ev = (OnSaveCreate*)event;
        // selectedFileNum = ev->fileNum;
        SaveData* saveData = (SaveData*)ev->saveData;

        saveData->shipSaveData.fileType = FILE_TYPE_SAVE_RANDO;

        // if (gameFile_isNotEmpty(selectedFileNum)) {
        //     return;
        // }
        // 
        // if (!IS_RANDO) {
        //     if (!CVarGetInteger("gRandoSettings.Enabled", 0)) {
        //         saveData->shipSaveData.fileType = FILE_TYPE_SAVE_RANDO;
        //     }
        // }
        Test();
        
    });
}
