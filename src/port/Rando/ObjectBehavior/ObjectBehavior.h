#ifndef RANDO_OBJECT_BEHAVIOR_H
#define RANDO_OBJECT_BEHAVIOR_H

#include "port/Rando/Rando.h"

bool ShouldOverrideSpawn(RandoCheckId randoCheckId);

namespace Rando {

namespace ObjectBehavior {

void Init();
void InitJiggyBehavior();
void InitMolehillBehavior();
void InitPropBehavior();

} // namespace ObjectBehavior

} // namespace Rando

#endif // RANDO_OBJECT_BEHAVIOR_H
