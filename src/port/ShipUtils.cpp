#include "ShipUtils.h"
#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <spdlog/spdlog.h>
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _DEBUG
#include <crtdbg.h>
#endif

extern "C" uint64_t GetUnixTimestamp() {
    auto time = std::chrono::system_clock::now();
    auto since_epoch = time.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);
    long now = millis.count();
    return now;
}

extern "C" bool Ship_IsCStringEmpty(const char* str) {
    return str == NULL || str[0] == '\0';
}

extern "C" int port_checkHeap(const char* label) {
#ifdef _DEBUG
    if (!_CrtCheckMemory()) {
        SPDLOG_ERROR("[port] HEAP CORRUPT at: {}", label);
        return 0;
    }
#endif
    return 1;
}

static void bk_log_vfmt(spdlog::level::level_enum level, const char* fmt, va_list args) {
    char buf[512];
    vsnprintf(buf, sizeof(buf), fmt, args);
    spdlog::default_logger_raw()->log(spdlog::source_loc{}, level, buf);
}

extern "C" void BK_LOG_INFO(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    bk_log_vfmt(spdlog::level::info, fmt, args);
    va_end(args);
}

extern "C" void BK_LOG_WARN(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    bk_log_vfmt(spdlog::level::warn, fmt, args);
    va_end(args);
}

extern "C" void BK_LOG_ERROR(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    bk_log_vfmt(spdlog::level::err, fmt, args);
    va_end(args);
}

extern "C" void port_setMapDebugTitle(int map_id) {
    char title[128];
    snprintf(title, sizeof(title), "Lighthouse - Map %d (0x%02X)", map_id, map_id);
    SPDLOG_INFO("[port] {}", title);
#ifdef _WIN32
    // [port] Update the DXGI/SDL window title for quick visual debugging
    HWND hwnd = GetActiveWindow();
    if (hwnd) {
        SetWindowTextA(hwnd, title);
    }
#endif
}
