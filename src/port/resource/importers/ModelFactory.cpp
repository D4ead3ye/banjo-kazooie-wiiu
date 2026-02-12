#include "ModelFactory.h"
#include "../type/Model.h"
#include "spdlog/spdlog.h"
#include <libultraship/libultraship.h>

namespace Factories {

// Helper to align offsets to 8-byte boundaries (N64 requirement)
inline size_t Align8(size_t offset) {
    return (offset + 7) & ~7;
}

std::shared_ptr<Ship::IResource> ResourceFactoryBinaryModelV0::ReadResource(std::shared_ptr<Ship::File> file,
                                                                           std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    auto model = std::make_shared<Model>(initData);
    auto resourceMgr = Ship::Context::GetInstance()->GetResourceManager();

    SPDLOG_INFO("=== Loading Model: {} ===", initData->Path);

    // Build a contiguous buffer with all model data
    std::vector<uint8_t> buffer;
    
    // Reserve space for the header
    buffer.resize(sizeof(BKModelBin));
    BKModelBin* header = reinterpret_cast<BKModelBin*>(buffer.data());
    memset(header, 0, sizeof(BKModelBin));
    
    size_t currentOffset = sizeof(BKModelBin);

    // Track loaded sub-resources and their data
    std::vector<uint8_t> geoLayoutData;
    std::vector<uint8_t> textureListData;
    std::vector<uint8_t> gfxListData;
    std::vector<uint8_t> vtxListData;
    std::vector<uint8_t> unk14Data;
    std::vector<uint8_t> animationListData;
    std::vector<uint8_t> collisionListData;
    std::vector<uint8_t> unk20Data;
    std::vector<uint8_t> effectsListData;
    std::vector<uint8_t> unk28Data;
    std::vector<uint8_t> animatedTextureData;

    // Try to load GeoLayout
    {
        std::string geoPath = initData->Path + "_GEO";
        auto geoResource = resourceMgr->LoadResourceProcess(geoPath);
        if (geoResource) {
            SPDLOG_INFO("  Loaded GeoLayout: {}", geoPath);
            // TODO: Extract geo data when GeoLayout resource type is implemented
            // For now, store the offset where it would go
            currentOffset = Align8(currentOffset);
            header->geo_list_offset_4 = static_cast<int32_t>(currentOffset);
            // currentOffset += geoLayoutData.size();
        }
    }

    // Try to load Vertex List
    {
        std::string vtxPath = initData->Path + "_VTX";
        auto vtxResource = resourceMgr->LoadResourceProcess(vtxPath);
        if (vtxResource) {
            SPDLOG_INFO("  Loaded VTX: {}", vtxPath);
            // TODO: Extract vertex data when VTX resource type is implemented
            currentOffset = Align8(currentOffset);
            header->vtx_list_offset_10 = static_cast<int32_t>(currentOffset);
            // currentOffset += vtxListData.size();
        }
    }

    // Try to load GFX display lists
    {
        uint32_t gfxIdx = 0;
        bool foundAny = false;
        while (true) {
            std::string gfxPath = initData->Path + "_GFX_" + std::to_string(gfxIdx);
            auto gfxResource = resourceMgr->LoadResourceProcess(gfxPath);
            if (!gfxResource) {
                break; // No more GFX resources
            }
            if (!foundAny) {
                currentOffset = Align8(currentOffset);
                header->gfx_list_offset_C = static_cast<int32_t>(currentOffset);
                foundAny = true;
            }
            SPDLOG_INFO("  Loaded GFX_{}: {}", gfxIdx, gfxPath);
            // TODO: Extract GFX data when GFX resource type is implemented
            gfxIdx++;
        }
    }

    // Try to load Textures
    {
        uint32_t texIdx = 0;
        bool foundAny = false;
        while (true) {
            std::string texPath = initData->Path + "_TEX_" + std::to_string(texIdx);
            auto texResource = resourceMgr->LoadResourceProcess(texPath);
            if (!texResource) {
                break; // No more texture resources
            }
            if (!foundAny) {
                currentOffset = Align8(currentOffset);
                header->texture_list_offset_8 = static_cast<int16_t>(currentOffset);
                foundAny = true;
            }
            SPDLOG_INFO("  Loaded TEX_{}: {}", texIdx, texPath);
            
            // Also try to load associated TLUT if it's a CI format
            std::string tlutPath = initData->Path + "_TLUT_" + std::to_string(texIdx);
            auto tlutResource = resourceMgr->LoadResourceProcess(tlutPath);
            if (tlutResource) {
                SPDLOG_INFO("    Loaded TLUT_{}: {}", texIdx, tlutPath);
            }
            
            texIdx++;
        }
    }

    // Allocate final buffer
    model->mModelData = std::make_unique<uint8_t[]>(buffer.size());
    model->mModelDataSize = buffer.size();
    std::memcpy(model->mModelData.get(), buffer.data(), buffer.size());

    SPDLOG_INFO("=== Model loading complete: {} bytes ===", buffer.size());

    return model;
}
} // namespace Factories
