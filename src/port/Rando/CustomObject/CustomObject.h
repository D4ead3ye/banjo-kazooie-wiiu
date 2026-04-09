#include <stdint.h>

#include "prop.h"

namespace CustomObject {
bool CheckSpawnQueue(int32_t posX, int32_t posY, int32_t posZ);
void InitializeSpawnQueue();
void AddToSpawnQueue(int32_t posX, int32_t posY, int32_t posZ);
} // namespace CustomObject