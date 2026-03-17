
#include "ship/resource/ResourceManager.h"
#include "ship/resource/type/Blob.h"
#include "fast/resource/ResourceType.h"
#include "fast/resource/type/DisplayList.h"
#include "libultraship/bridge/resourcebridge.h"
#include "ship/Context.h"
#include <spdlog/spdlog.h>
#include <algorithm>
#include <cstring>
#include <mutex>
#include <string>
#include <unordered_map>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

extern "C" {
#include "enums.h"
}

extern "C" uint16_t ResourceMgr_LoadTexWidthByName(char* texPath);

extern "C" uint16_t ResourceMgr_LoadTexHeightByName(char* texPath);

namespace {
const std::unordered_map<uint32_t, std::string>& GetAssetSymbolMap() {
    static std::once_flag mapOnce;
    static std::unordered_map<uint32_t, std::string> symbolMap;

    std::call_once(mapOnce, [] {
        // [port] Load the asset ID → o2r path manifest from the archive.
        // Torch writes this as a Blob at "assets/aBKAssetTable".
        // Format: u32 count, then for each entry: u32 assetId, s32 pathLen, char path[pathLen]
        auto res = Ship::Context::GetInstance()->GetResourceManager()->LoadResource("assets/aBKAssetTable");
        if (!res) {
            SPDLOG_ERROR("Failed to load asset manifest from o2r");
            return;
        }

        auto blob = std::dynamic_pointer_cast<Ship::Blob>(res);
        if (!blob || blob->Data.empty()) {
            SPDLOG_ERROR("Asset manifest blob is empty or wrong type");
            return;
        }

        const uint8_t* data = blob->Data.data();
        const size_t dataSize = blob->Data.size();
        size_t pos = 0;

        if (dataSize < 4) {
            SPDLOG_ERROR("Asset manifest too small");
            return;
        }

        uint32_t count;
        std::memcpy(&count, data + pos, 4);
        pos += 4;

        for (uint32_t i = 0; i < count && pos + 8 <= dataSize; i++) {
            uint32_t assetId;
            std::memcpy(&assetId, data + pos, 4);
            pos += 4;

            int32_t pathLen;
            std::memcpy(&pathLen, data + pos, 4);
            pos += 4;

            if (pathLen < 0 || pos + static_cast<size_t>(pathLen) > dataSize) {
                SPDLOG_ERROR("Asset manifest corrupt at entry {}", i);
                break;
            }

            std::string path(reinterpret_cast<const char*>(data + pos), static_cast<size_t>(pathLen));
            pos += static_cast<size_t>(pathLen);

            symbolMap[assetId] = std::move(path);
        }

        SPDLOG_INFO("Loaded asset manifest from o2r with {} entries", symbolMap.size());
    });

    return symbolMap;
}
} // namespace

std::shared_ptr<Ship::IResource> GetResourceByName(const char* path) {
    try {
        return Ship::Context::GetInstance()->GetResourceManager()->LoadResource(path);
    } catch (const std::exception& e) {
        SPDLOG_ERROR("[port] GetResourceByName('{}') exception: {}", path, e.what());
        return nullptr;
    }
}

// [port] Keep shared_ptr references alive so raw pointers from GetResourceRawPointer
// don't become dangling when the resource manager evicts entries from its cache.
static std::unordered_map<uint32_t, std::shared_ptr<Ship::IResource>> sResourceRefCache;

static char* LoadAndRetainResource(const std::string& path, uint32_t assetId) {
    auto res = GetResourceByName(path.c_str());
    if (res && res->GetRawPointer() != nullptr) {
        sResourceRefCache[assetId] = res;
        return reinterpret_cast<char*>(res->GetRawPointer());
    }
    return nullptr;
}

extern "C" char* ResourceMgr_LoadByAssetId(uint32_t assetId) {
    // Return cached resource if already loaded
    if (auto it = sResourceRefCache.find(assetId); it != sResourceRefCache.end()) {
        auto ptr = it->second->GetRawPointer();
        if (ptr != nullptr) {
            return reinterpret_cast<char*>(ptr);
        }
        sResourceRefCache.erase(it);
    }

    const auto& symbolMap = GetAssetSymbolMap();
    if (const auto entry = symbolMap.find(assetId); entry != symbolMap.end()) {
        auto mappedPath = entry->second;
        std::replace(mappedPath.begin(), mappedPath.end(), '\\', '/');

        if (auto result = LoadAndRetainResource(mappedPath, assetId); result != nullptr) {
            SPDLOG_INFO("Loading '{}'", mappedPath);
            return result;
        } else {
            SPDLOG_WARN("[port] ResourceMgr_LoadByAssetId({}) symbol '{}' found but resource data is NULL", assetId,
                        mappedPath);
            return nullptr;
        }
    } else {
        SPDLOG_WARN("[port] ResourceMgr_LoadByAssetId({}) not found in symbol map", assetId);
        return nullptr;
    }

    return nullptr;
}

// [port] Returns the data size of a previously loaded resource (from the ref cache).
// Used by decomp code that depends on assetCacheCurrentSize (e.g. demo_load).
extern "C" size_t ResourceMgr_GetResourceSize(uint32_t assetId) {
    if (auto it = sResourceRefCache.find(assetId); it != sResourceRefCache.end()) {
        return it->second->GetPointerSize();
    }
    return 0;
}

// [port] On N64, sprites and models were raw binary blobs that could be type-punned.
// On PC, they're separate resource types from different importers. Actors with sprite
// assets can be spawned as "model" props (unk8_1=1), causing collision code to call
// marker_loadModelBin which reinterprets sprite data as BKModelBin. This helper lets
// decomp code detect and skip the model path for sprite assets.
extern "C" int ResourceMgr_IsModelAsset(uint32_t assetId) {
    if (auto it = sResourceRefCache.find(assetId); it != sResourceRefCache.end()) {
        return it->second->GetInitData()->Type == 0x424B4D4F; // Torch::ResourceType::BKModel
    }
    return 0;
}

extern "C" char* ResourceMgr_LoadTexOrDListByName(const char* filePath) {
    auto res = GetResourceByName(filePath);

    if (res->GetInitData()->Type == static_cast<uint32_t>(Fast::ResourceType::DisplayList))
        return (char*)&((std::static_pointer_cast<Fast::DisplayList>(res))->Instructions[0]);
    else {
        return (char*)ResourceGetDataByName(filePath);
    }
}

extern "C" char* ResourceMgr_LoadIfDListByName(const char* filePath) {
    auto res = GetResourceByName(filePath);

    if (res->GetInitData()->Type == static_cast<uint32_t>(Fast::ResourceType::DisplayList))
        return (char*)&((std::static_pointer_cast<Fast::DisplayList>(res))->Instructions[0]);

    return nullptr;
}

extern "C" Gfx* ResourceMgr_LoadGfxByName(const char* path) {
    auto res = std::static_pointer_cast<Fast::DisplayList>(GetResourceByName(path));
    return (Gfx*)&res->Instructions[0];
}

extern "C" Vtx* ResourceMgr_LoadVtxByName(char* path) {
    return (Vtx*)ResourceGetDataByName(path);
}

extern "C" Mtx* ResourceMgr_LoadMtxByName(char* path) {
    return (Mtx*)ResourceGetDataByName(path);
}

// Release all retained resource refs so destructors fire before spdlog shutdown
void ResourceHelpers_ClearRefCache() {
    sResourceRefCache.clear();
}
