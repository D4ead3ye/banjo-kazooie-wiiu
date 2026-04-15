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

DEFINE_EVENT(OnPropSpawn,
	int32_t propId;
	int32_t posX;
	int32_t posY;
	int32_t posZ;
)

DEFINE_EVENT(OnBundleSpawn,
	int32_t bundle_id;
	f32 bundleYaw;
	void* bundleInfo;
	f32 posX;
	f32 posY;
	f32 posZ;
	Actor** result; 
)

DEFINE_EVENT(OnJiggySpawn,
	int32_t jiggyId;
	f32 posX;
	f32 posY;
	f32 posZ;
)

DEFINE_EVENT(OnActorSaveState)

DEFINE_EVENT(OnActorCollision,
	Prop* propId;
)
// clang-format on