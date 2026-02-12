#include "SpriteFactory.h"
#include "../type/Sprite.h"
#include "spdlog/spdlog.h"
#include <libultraship/libultraship.h>

namespace Factories {
std::shared_ptr<Ship::IResource> ResourceFactoryBinarySpriteV0::ReadResource(std::shared_ptr<Ship::File> file,
                                                                           std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    auto sprite = std::make_shared<Sprite>(initData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    int16_t formatCode = reader->ReadInt16();
    uint32_t positionsSize = reader->ReadUInt32();
    std::vector<std::pair<int16_t, int16_t>> positions;
    for (uint32_t i = 0; i < positionsSize; i++) {
        int16_t x = reader->ReadInt16();
        int16_t y = reader->ReadInt16();
        positions.emplace_back(x, y);
    }
    uint32_t chunkCountsSize = reader->ReadUInt32();
    std::vector<uint16_t> chunkCounts;
    for (uint32_t i = 0; i < chunkCountsSize; i++) {
        uint16_t count = reader->ReadUInt16();
        chunkCounts.push_back(count);
    }

    sprite->frameCount = chunkCountsSize;
    sprite->formatType = formatCode;

    SPDLOG_INFO("=== Loading Sprite: {} ===", initData->Path);
    SPDLOG_INFO("Format code: {}, Frame count: {}, Total positions: {}", formatCode, chunkCountsSize, positionsSize);

    // Build frames with their chunks
    size_t positionIdx = 0;
    for (uint32_t frameIdx = 0; frameIdx < chunkCountsSize; frameIdx++) {
        SpriteFrameData frameData;
        uint16_t chunkCount = chunkCounts[frameIdx];
        
        SPDLOG_INFO("Loading frame {}: {} chunks", frameIdx, chunkCount);
        
        // Initialize frame header
        frameData.frameHeader.unk0 = 0;
        frameData.frameHeader.unk2 = 0;
        frameData.frameHeader.w = 0;
        frameData.frameHeader.h = 0;
        frameData.frameHeader.chunkCnt = chunkCount;
        frameData.frameHeader.unkA = 0;
        frameData.frameHeader.unkC = 0;
        frameData.frameHeader.unkE = 0;
        frameData.frameHeader.unk10 = 0;
        frameData.frameHeader.unk12 = 0;

        // Load palette data for color-indexed formats
        if (formatCode == 0x1 || formatCode == 0x4) { // CI4 or CI8
            std::string tlutPath = initData->Path + "_" + std::to_string(frameIdx) + "_TLUT";
            auto tlutTexture = std::static_pointer_cast<Fast::Texture>(
                Ship::Context::GetInstance()->GetResourceManager()->LoadResourceProcess(tlutPath)
            );
            
            if (tlutTexture && tlutTexture->ImageData) {
                frameData.paletteData.resize(tlutTexture->ImageDataSize);
                std::memcpy(frameData.paletteData.data(), tlutTexture->ImageData, tlutTexture->ImageDataSize);
                SPDLOG_INFO("  Loaded TLUT: {} bytes", tlutTexture->ImageDataSize);
            } else {
                SPDLOG_WARN("Failed to load TLUT for frame {}: {}", frameIdx, tlutPath);
                // Allocate empty palette as fallback
                size_t paletteSize = (formatCode == 0x1) ? 32 : 512; // CI4: 16 colors, CI8: 256 colors
                frameData.paletteData.resize(paletteSize);
            }
        }

        // Load each chunk for this frame
        for (uint16_t chunkIdx = 0; chunkIdx < chunkCount; chunkIdx++) {
            SpriteFrameData::ChunkData chunkData;
            
            std::string chunkPath = initData->Path + "_" + std::to_string(frameIdx) + "_" + std::to_string(chunkIdx);
            auto texture = std::static_pointer_cast<Fast::Texture>(
                Ship::Context::GetInstance()->GetResourceManager()->LoadResourceProcess(chunkPath)
            );

            if (!texture || !texture->ImageData) {
                SPDLOG_ERROR("Failed to load texture chunk: {}", chunkPath);
                continue;
            }

            // Set chunk header with position from positions array
            if (positionIdx < positions.size()) {
                chunkData.header.x = positions[positionIdx].first;
                chunkData.header.y = positions[positionIdx].second;
                positionIdx++;
            } else {
                chunkData.header.x = 0;
                chunkData.header.y = 0;
            }
            chunkData.header.w = texture->Width;
            chunkData.header.h = texture->Height;

            // Copy texture data
            chunkData.textureData.resize(texture->ImageDataSize);
            std::memcpy(chunkData.textureData.data(), texture->ImageData, texture->ImageDataSize);

            SPDLOG_DEBUG("  Chunk {}: pos=({},{}), size={}x{}, data={} bytes", 
                        chunkIdx, chunkData.header.x, chunkData.header.y,
                        chunkData.header.w, chunkData.header.h, chunkData.textureData.size());

            // Update frame dimensions to encompass all chunks
            int16_t chunkRight = chunkData.header.x + chunkData.header.w;
            int16_t chunkBottom = chunkData.header.y + chunkData.header.h;
            if (chunkRight > frameData.frameHeader.w) {
                frameData.frameHeader.w = chunkRight;
            }
            if (chunkBottom > frameData.frameHeader.h) {
                frameData.frameHeader.h = chunkBottom;
            }

            frameData.chunks.push_back(std::move(chunkData));
        }

        SPDLOG_INFO("  Frame {} final size: {}x{} with {} chunks loaded", 
                    frameIdx, frameData.frameHeader.w, frameData.frameHeader.h, frameData.chunks.size());
        sprite->frames.push_back(std::move(frameData));
    }

    SPDLOG_INFO("=== Sprite loading complete: {} frames ===", sprite->frames.size());

    return sprite;
}
} // namespace Factories
