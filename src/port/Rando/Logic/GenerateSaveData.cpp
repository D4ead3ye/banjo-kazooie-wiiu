#include "Logic.h"
#include "port/ui/Notification.h"
#include <libultraship/bridge/consolevariablebridge.h>
#include "port/enhancements/events/hooks/Events.h"

#include "spdlog/spdlog.h"

void Rando::Logic::InitializeSaveData(SaveData* saveData) {
    // RandoSaveCheck - Initialize
    for (auto& [randoCheckId, randoStaticCheck] : Rando::StaticData::Checks) {
        RandoSaveCheck randoSaveCheck = {
            .randoItemId = Rando::StaticData::GetRandoItemByActorId((actor_e)randoStaticCheck.actorId),
            .shuffledCheckId = randoCheckId,
            .randoCollectionId = randoStaticCheck.collectionId,
            .isShuffled = false,
            .obtained = false,
            .skipped = false,
        };

        saveData->shipSaveData.randoSaveData.randoSaveCheck[randoCheckId] = randoSaveCheck;
    }
}

void Rando::Logic::GenerateSaveData(SaveData* saveData) {
    for (auto& object : Rando::Logic::shuffledPool) {
        RandoSaveCheck randoSaveCheck = {
            .randoItemId = object.randoItemId,
            .shuffledCheckId = object.shuffleCheckId,
            .randoCollectionId = object.randoCollectionId,
            .isShuffled = object.isShuffled,
            .obtained = object.obtained,
            .skipped = object.skipped,
        };

        saveData->shipSaveData.randoSaveData.randoSaveCheck[object.randoCheckId] = randoSaveCheck;
    }
}