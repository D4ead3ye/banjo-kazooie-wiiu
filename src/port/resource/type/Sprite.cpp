#include "Sprite.h"
#include <cstring>
#include "spdlog/spdlog.h"

namespace Factories {

void Sprite::BuildSpriteStructure() {
    SPDLOG_INFO("=== Building Sprite Structure ===");
    SPDLOG_INFO("Frame count: {}, Format type: {}", frameCount, formatType);
    
    // Calculate total size needed for contiguous buffer
    size_t totalSize = sizeof(BKSprite) + frameCount * sizeof(BKSpriteFrame*);
    std::vector<size_t> frameSizes;
    std::vector<size_t> frameOffsets;
    
    for (int frameIdx = 0; frameIdx < frameCount; frameIdx++) {
        const auto& frameData = frames[frameIdx];
        
        size_t frameSize = sizeof(BKSpriteFrame);
        
        if (!frameData.paletteData.empty()) {
            frameSize = Align8(frameSize);
            frameSize += frameData.paletteData.size();
        }
        
        for (const auto& chunk : frameData.chunks) {
            frameSize += sizeof(BKSpriteTextureBlock);
            frameSize = Align8(frameSize);
            frameSize += chunk.textureData.size();
        }
        
        frameSizes.push_back(frameSize);
        frameOffsets.push_back(totalSize);
        totalSize += frameSize;
    }
    
    // Allocate single contiguous buffer for entire sprite
    mSpriteHeader = std::make_unique<uint8_t[]>(totalSize);
    uint8_t* basePtr = mSpriteHeader.get();
    
    // Setup BKSprite header
    BKSprite* sprite = reinterpret_cast<BKSprite*>(basePtr);
    sprite->frameCnt = frameCount;
    sprite->type = formatType;
    sprite->unk4 = 0;
    sprite->unk6 = 0;
    sprite->unk8 = 0;
    sprite->unkA = 0;
    sprite->unkC.bit31 = 0;
    sprite->unkC.bit27 = 0;
    sprite->unkC.bit24 = 0;
    sprite->unkC.bit22 = 0;
    sprite->unkC.pad_bit20 = 0;
    
    // Build each frame in the contiguous buffer
    for (int frameIdx = 0; frameIdx < frameCount; frameIdx++) {
        const auto& frameData = frames[frameIdx];
        size_t frameOffset = frameOffsets[frameIdx];
        uint8_t* framePtr = basePtr + frameOffset;
        size_t offset = 0;
        
        SPDLOG_INFO("--- Frame {} at offset {} ---", frameIdx, frameOffset);
        SPDLOG_INFO("  Chunks: {}, Frame size: {}x{}", frameData.chunks.size(), 
                    frameData.frameHeader.w, frameData.frameHeader.h);
        
        // Set frame pointer in BKSprite
        sprite->frames[frameIdx] = reinterpret_cast<BKSpriteFrame*>(framePtr);
        
        // Write frame header
        BKSpriteFrame* frame = reinterpret_cast<BKSpriteFrame*>(framePtr);
        *frame = frameData.frameHeader;
        offset += sizeof(BKSpriteFrame);
        
        // Write palette
        if (!frameData.paletteData.empty()) {
            offset = Align8(offset);
            std::memcpy(framePtr + offset, frameData.paletteData.data(), frameData.paletteData.size());
            SPDLOG_DEBUG("  Palette: {} bytes at offset {}", frameData.paletteData.size(), offset);
            offset += frameData.paletteData.size();
        }
        
        // Write chunks
        for (size_t chunkIdx = 0; chunkIdx < frameData.chunks.size(); chunkIdx++) {
            const auto& chunkData = frameData.chunks[chunkIdx];
            
            size_t chunkHeaderOffset = offset;
            
            // Write chunk header
            BKSpriteTextureBlock* chunk = reinterpret_cast<BKSpriteTextureBlock*>(framePtr + offset);
            *chunk = chunkData.header;
            offset += sizeof(BKSpriteTextureBlock);
            
            // Write texture data
            offset = Align8(offset);
            std::memcpy(framePtr + offset, chunkData.textureData.data(), chunkData.textureData.size());
            SPDLOG_INFO("  Chunk {}: header at offset {} (frame+{}), pos=({}, {}), size={}x{}, data at offset {}, {} bytes", 
                        chunkIdx, chunkHeaderOffset, 
                        (chunkIdx == 0) ? "1" : std::to_string(chunkHeaderOffset - sizeof(BKSpriteFrame)),
                        chunkData.header.x, chunkData.header.y,
                        chunkData.header.w, chunkData.header.h,
                        offset, chunkData.textureData.size());
            offset += chunkData.textureData.size();
        }
        
        SPDLOG_DEBUG("  Frame buffer size: {} bytes, pointer: {}", 
                    frameSizes[frameIdx], static_cast<void*>(sprite->frames[frameIdx]));
    }
    
    SPDLOG_INFO("=== Sprite structure complete (total {} bytes) ===", totalSize);
    
    // Verify
    SPDLOG_INFO("Verification: frameCnt={}, type=0x{:X}", sprite->frameCnt, sprite->type);
    if (frameCount > 0) {
        BKSpriteFrame* verifyFrame = sprite->frames[0];
        SPDLOG_INFO("Frame 0: w={}, h={}, chunkCnt={}, frame pointer={}", 
                    verifyFrame->w, verifyFrame->h, verifyFrame->chunkCnt, 
                    static_cast<void*>(verifyFrame));
        
        if (verifyFrame->chunkCnt > 0) {
            BKSpriteTextureBlock* verifyChunk = reinterpret_cast<BKSpriteTextureBlock*>(verifyFrame + 1);
            SPDLOG_INFO("First chunk at frame+1: pointer={}, pos=({}, {}), size={}x{}", 
                        static_cast<void*>(verifyChunk),
                        verifyChunk->x, verifyChunk->y, verifyChunk->w, verifyChunk->h);
            SPDLOG_INFO("Expected offset from frame: {} bytes, actual: {} bytes",
                        sizeof(BKSpriteFrame),
                        reinterpret_cast<uint8_t*>(verifyChunk) - reinterpret_cast<uint8_t*>(verifyFrame));
        }
    }
}

BKSprite* Sprite::GetPointer() {
    if (!mSpriteHeader) {
        SPDLOG_INFO("Building sprite structure on first GetPointer() call");
        BuildSpriteStructure();
    }
    BKSprite* sprite = reinterpret_cast<BKSprite*>(mSpriteHeader.get());
    SPDLOG_INFO("GetPointer() returning sprite at {} with {} frames", 
                static_cast<void*>(sprite), sprite->frameCnt);
    if (sprite->frameCnt > 0) {
        SPDLOG_INFO("  Frame[0] pointer: {}", static_cast<void*>(sprite->frames[0]));
    }
    return sprite;
}

size_t Sprite::GetPointerSize() {
    // Size was calculated during BuildSpriteStructure
    // For now, we don't store it separately, but GetPointer ensures it's built
    if (!mSpriteHeader) {
        BuildSpriteStructure();
    }
    // Return a nominal value since the actual size is managed internally
    return sizeof(BKSprite) + frameCount * sizeof(BKSpriteFrame*);
}

}