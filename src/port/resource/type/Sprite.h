#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <libultraship/libultraship.h>
#include <ship/resource/Resource.h>

extern "C" {
#include "structs.h"
}

namespace Factories {

// Helper to align offsets to 8-byte boundaries (N64 requirement)
inline size_t Align8(size_t offset) {
    return (offset + 7) & ~7;
}

// Store frame data with palette and texture chunks
struct SpriteFrameData {
    BKSpriteFrame frameHeader;
    std::vector<uint8_t> paletteData;  // For CI4/CI8 formats
    
    struct ChunkData {
        BKSpriteTextureBlock header;
        std::vector<uint8_t> textureData;
    };
    std::vector<ChunkData> chunks;
};

class Sprite : public Ship::Resource<BKSprite> {
  public:
    using Resource::Resource;

    Sprite() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {}

    BKSprite* GetPointer();
    size_t GetPointerSize();

    // Build the sprite structure with direct pointers
    void BuildSpriteStructure();

    // Modern storage (separate components)
    int16_t frameCount;
    int16_t formatType;
    std::vector<SpriteFrameData> frames;

  private:
    // Single contiguous buffer: BKSprite header + frame pointers + all frame data
    std::unique_ptr<uint8_t[]> mSpriteHeader;
};
}