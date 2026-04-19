#pragma once

#include "port/enhancements/events/hooks/EventSystem.h"
#include <stdarg.h>

#include "enums.h"
#include "prop.h"

// clang-format off
DEFINE_EVENT(OnSaveLoad,
	void* saveData;
)

DEFINE_EVENT(OnActorSpawn,
	int32_t actorId;
	int32_t posX;
	int32_t posY;
	int32_t posZ;
	Actor* result;
)

DEFINE_EVENT(OnBundleSpawn,
	int32_t bundle_id;
	f32 bundleYaw;
	void* bundleInfo;
	int32_t curCount;
	f32 posX;
	f32 posY;
	f32 posZ;
	Actor** result; 
)

DEFINE_EVENT(OnActorSaveState)

DEFINE_EVENT(OnActorCollision,
	Prop* propId;
)
// clang-format on