#include "port/Rando/StaticData/StaticData.h"
#include <stdint.h>

#include "prop.h"

namespace CustomObject {
void InitializeSpawnQueue();
void AddToSpawnQueue(actor_e id, int32_t posX, int32_t posY, int32_t posZ);
Actor* SpawnRandoObject(actor_e actorId, int32_t posX, int32_t posY, int32_t posZ);
} // namespace CustomObject