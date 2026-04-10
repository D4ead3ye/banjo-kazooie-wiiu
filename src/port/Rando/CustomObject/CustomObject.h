#include <stdint.h>
#include "port/Rando/Types.h"

#include "prop.h"

namespace CustomObject {
Actor* SpawnRandoActor(actor_e actorId, int32_t position[3]);
bool CheckSpawnQueue(RandoCheckId randoCheckId);
void InitializeSpawnQueue();

void AddToSpawnQueue(RandoCheckId randoCheckId, int32_t posX, int32_t posY, int32_t posZ);
} // namespace CustomObject