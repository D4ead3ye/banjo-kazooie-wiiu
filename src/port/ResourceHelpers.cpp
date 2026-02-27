
#include "ship/resource/ResourceManager.h"
#include "fast/resource/ResourceType.h"
#include "fast/resource/type/DisplayList.h"
#include "libultraship/bridge/resourcebridge.h"
#include "ship/Context.h"
#include <spdlog/spdlog.h>

extern "C" {
#include "enums.h"
}

extern "C" uint16_t ResourceMgr_LoadTexWidthByName(char* texPath);

extern "C" uint16_t ResourceMgr_LoadTexHeightByName(char* texPath);

std::shared_ptr<Ship::IResource> GetResourceByName(const char* path) {
    return Ship::Context::GetInstance()->GetResourceManager()->LoadResource(path);
}

extern "C" char* ResourceMgr_LoadByAssetId(uint32_t assetId) {
    if (assetId == 1) {
        return nullptr;
    }
    
    SPDLOG_INFO("ResourceMgr_LoadByAssetId({})", assetId);

    std::string path = "asset_table/D_";
    
    if (
        (assetId >= ASSET_6D9_SPRITE_PROPELLOR_TIMER && assetId <= ASSET_71B_SPRITE_SPARKLE_ORANGE_2) ||
        (assetId == ASSET_580_SPRITE_RED_FEATHER)
    ) {
        path += "SPRITE_";
    } else if (assetId <= ASSET_2AB_ANIM_TEEHEE_DIE) {
        path += "ANIM_";
    } else {
        path += "MODEL_";
    }

    path += std::to_string(assetId);

    auto res = ResourceGetDataByName(path.c_str());
    SPDLOG_INFO("ResourceMgr_LoadByAssetId returning {}", static_cast<void*>(res));

    return (char*)res;
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
