#ifndef CUSTOM_OBJECT_H
#define CUSTOM_OBJECT_H

#include <stdint.h>
#include "port/Rando/Types.h"

#include "prop.h"

typedef struct {
    RandoCheckId randoCheckId;
    int32_t location[3];
} CustomActor;

CustomActor CreateCustomActor(RandoCheckId randoCheckId, int32_t position[3]);

class CustomObject {
public:
    static bool CheckSpawnQueue(RandoCheckId randoCheckId);
    static Actor* SetCustomActorParameters(Actor* actor, RandoCheckId randoCheckId);
    static Actor* SpawnCustomActor(actor_e actorId, int32_t position[3]);
    static void AddToCustomActorMap(RandoCheckId randoCheckId, Actor* actor);
    static void AddToSpawnQueue(RandoCheckId randoCheckId, int32_t position[3]);
    static void InitializeSpawnQueue();
    static void ObjectCollected(Prop* prop);
};

#endif // CUSTOM_OBJECT_H