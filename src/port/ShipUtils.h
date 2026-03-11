#pragma once

#include "libultraship/libultra/types.h"

#ifdef __cplusplus
extern "C" {
#endif

uint64_t GetUnixTimestamp();
bool Ship_IsCStringEmpty(const char* str);
int port_checkHeap(const char* label);

// [port] SPDLOG level wrappers callable from C
void BK_LOG_INFO(const char* fmt, ...);
void BK_LOG_WARN(const char* fmt, ...);
void BK_LOG_ERROR(const char* fmt, ...);

// [port] Update window title with current map ID for debugging
void port_setMapDebugTitle(int map_id);

#ifdef __cplusplus
}
#endif
