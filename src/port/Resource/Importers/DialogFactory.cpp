#include "DialogFactory.h"
#include <cstring>

#include <libultraship/libultraship.h>
#include <libultraship/bridge/consolevariablebridge.h>
#include <ship/resource/type/Blob.h>

#include "port/UI/cvar_prefixes.h"
#include "port/Controller/ControlSchemes.h"

namespace Factories {
namespace {
void AppendBytes(std::vector<uint8_t>& dst, const char* data, size_t size) {
    const auto base = dst.size();
    dst.resize(base + size);
    std::memcpy(dst.data() + base, data, size);
}

// [port] The game's text names N64 buttons literally ("HOLD Z", "THE TOP C
// BUTTON"), which means nothing on a GamePad. Rewrite them to the pad this build
// actually maps to, under the stock Retro scheme:
//   N64 A/B  -> A/B      (SDL follows Nintendo's printed layout here)
//   N64 Z    -> ZL
//   C buttons-> right stick directions
// Longest-first, so "THE TOP C BUTTON" is matched before a bare "C BUTTON".
//
// Every replacement must be no longer than what it replaces. The game wraps text
// on a fixed width, so a longer string pushes the last words outside the box -
// "C BUTTON" -> "THE RIGHT STICK" nearly doubled it and did exactly that. The
// only exception is Z, which grows by one character; there is no shorter way to
// say ZL, and a single character stays inside the box.
// Kept in step with ControlSchemes: under the Modern layout with free look, the
// right stick is the camera and the C buttons moved onto face buttons, so the
// old "RIGHT STICK UP/DOWN" wording is wrong for C-up and C-down. C-left and
// C-right no longer exist as buttons at all - they rotated the camera, which is
// now the stick itself.
//
//   N64 A -> A      N64 B -> B      Z -> ZL/LT   C-up -> Y      C-down -> X
//   C-left -> ZR/RT (Talon Trot)  C-right -> the right stick while crouched
//   (Wonderwing)                  R -> R    Start -> +/MENU
//
// The trigger and Start names follow the pad the build targets, because those
// are the ones whose labels differ: a Wii U pad prints ZL/ZR and +, an Xbox-
// layout pad prints LT/RT and Menu, and naming a button the player cannot find
// is worse than not rewriting at all. The face buttons need no such split - SDL
// reports A/B/X/Y by the label printed on whichever pad is attached, so "Y" is
// the button marked Y on an Xbox pad and on a Wii U Pro Controller alike.
#ifdef __WIIU__
#define PROMPT_ZTRIGGER "ZL"
#define PROMPT_RTRIGGER "ZR"
#define PROMPT_START "+"
#else
#define PROMPT_ZTRIGGER "LT"
#define PROMPT_RTRIGGER "RT"
#define PROMPT_START "MENU"
#endif
//
// Of these rules only HOLD Z, the four directional C BUTTON forms, bare C BUTTON
// and CONTROL STICK ever match the shipped text; the rest are kept so a text
// revision or another region does not silently go unrewritten.
static const std::pair<const char*, const char*> kButtonPrompts[] = {
    // Order matters. The Z rules run first: rewriting C-left to "RT" before
    // them would let a bare "Z" rule match inside an already-rewritten string.
    { "HOLD Z", "HOLD " PROMPT_ZTRIGGER },
    { "PRESS Z", "PRESS " PROMPT_ZTRIGGER },
    { "THE Z BUTTON", PROMPT_ZTRIGGER },
    { "Z BUTTON", PROMPT_ZTRIGGER },

    { "THE TOP C BUTTON", "Y" },
    { "THE BOTTOM C BUTTON", "X" },
    { "THE LEFT C BUTTON", PROMPT_RTRIGGER },
    { "THE RIGHT C BUTTON", "THE D-PAD RIGHT" },
    { "TOP C BUTTON", "Y" },
    { "BOTTOM C BUTTON", "X" },
    { "LEFT C BUTTON", PROMPT_RTRIGGER },
    { "RIGHT C BUTTON", "D-PAD RIGHT" },
    { "C BUTTONS", "R STICK" },
    { "C BUTTON", "R STICK" },
    { "C-UP", "Y" },
    { "C-DOWN", "X" },
    { "C-LEFT", PROMPT_RTRIGGER },
    { "C-RIGHT", "D-PAD" },

    { "THE START BUTTON", PROMPT_START },
    { "START BUTTON", PROMPT_START },
    // No leading article: the source usually reads "THE CONTROL STICK".
    { "CONTROL STICK", "LEFT STICK" },
};

// [port] Toggle, so the rewriting can be ruled in or out without a rebuild:
// dialogue is rewritten at load, so change it and restart. Off gives the vanilla
// N64 prompts.
#define CVAR_REWRITE_PROMPTS CVAR_ENHANCEMENT("Controls.RewritePrompts")

static void RewriteButtonPrompts(std::string& text) {
    if (CVarGetInteger(CVAR_REWRITE_PROMPTS, 1) == 0) {
        return;
    }
    // [port] The replacements below name Modern's buttons (Y, X, ZL, ZR, the
    // right stick). Under Retro those names are simply wrong - C-up is not Y
    // there - so leave the vanilla N64 text alone and let it describe the
    // bindings the player actually has. Keep this default in step with
    // ControlSchemes.cpp.
    if (CVarGetInteger(CVAR_SETTING("Controls.Scheme"), CONTROL_SCHEME_MODERN) != CONTROL_SCHEME_MODERN) {
        return;
    }
    const size_t before = text.size();
    for (const auto& [from, to] : kButtonPrompts) {
        const size_t fromLen = std::strlen(from);
        size_t pos = 0;
        while ((pos = text.find(from, pos)) != std::string::npos) {
            const size_t grown = text.size() - fromLen + std::strlen(to);
            if (grown > 255) {
                // The length is written as a single byte; leave it alone rather
                // than truncate a line of dialogue.
                break;
            }
            text.replace(pos, fromLen, to);
            pos += std::strlen(to);
        }
    }

    // The length is stored in one byte downstream. Anything at or over the limit
    // would be truncated into a corrupt entry, so report it rather than ship it.
    if (text.size() > 255) {
        SPDLOG_ERROR("[dialog] rewrite overflowed: {} -> {} bytes, truncating", before, text.size());
        text.resize(255);
    }
    // Growth past a character or two means a replacement is wider than the text
    // it replaced, which the fixed-width box cannot absorb.
    if (text.size() > before + 1) {
        SPDLOG_WARN("[dialog] rewrite grew {} -> {} bytes, may overflow the box", before, text.size());
    }
}

std::string ReadSizedString(const std::shared_ptr<Ship::BinaryReader>& reader, uint32_t len) {
    std::string out;
    out.resize(len);
    if (len > 0) {
        reader->Read(out.data(), static_cast<int32_t>(len));
    }
    RewriteButtonPrompts(out);
    return out;
}

std::shared_ptr<Ship::Blob> MakeBlob(const std::shared_ptr<Ship::ResourceInitData>& initData,
                                     std::vector<uint8_t>&& data) {
    auto blob = std::make_shared<Ship::Blob>(initData);
    blob->Data = std::move(data);
    return blob;
}

// Read one language block from Torch's o2r format (u32 counts, u8 cmd, u32 strlen, chars)
// and reconstruct the ROM format (u8 count, u8 cmd, u8 strlen, chars)
std::vector<uint8_t> ReadLangBlock(const std::shared_ptr<Ship::BinaryReader>& reader) {
    std::vector<uint8_t> block;

    // Bottom entries
    const uint32_t bottomCount = reader->ReadUInt32();
    block.push_back(static_cast<uint8_t>(bottomCount));
    for (uint32_t i = 0; i < bottomCount; i++) {
        const uint8_t cmd = reader->ReadUByte();
        const uint32_t len = reader->ReadUInt32();
        const auto str = ReadSizedString(reader, len);
        block.push_back(cmd);
        block.push_back(static_cast<uint8_t>(str.size()));
        AppendBytes(block, str.data(), str.size());
    }

    // Top entries
    const uint32_t topCount = reader->ReadUInt32();
    block.push_back(static_cast<uint8_t>(topCount));
    for (uint32_t i = 0; i < topCount; i++) {
        const uint8_t cmd = reader->ReadUByte();
        const uint32_t len = reader->ReadUInt32();
        const auto str = ReadSizedString(reader, len);
        block.push_back(cmd);
        block.push_back(static_cast<uint8_t>(str.size()));
        AppendBytes(block, str.data(), str.size());
    }

    return block;
}

uint32_t ReadEntryRun(const std::shared_ptr<Ship::BinaryReader>& reader, std::vector<uint8_t>& block) {
    const uint32_t count = reader->ReadUInt32();
    for (uint32_t i = 0; i < count; i++) {
        const uint8_t cmd = reader->ReadUByte();
        const uint32_t len = reader->ReadUInt32();
        const auto str = ReadSizedString(reader, len);
        block.push_back(cmd);
        block.push_back(static_cast<uint8_t>(str.size()));
        AppendBytes(block, str.data(), str.size());
    }
    return count;
}

std::vector<uint8_t> ReadQuizLangBlock(const std::shared_ptr<Ship::BinaryReader>& reader) {
    std::vector<uint8_t> entries;
    const uint32_t textCount = ReadEntryRun(reader, entries);
    const uint32_t optionCount = ReadEntryRun(reader, entries);

    std::vector<uint8_t> block;
    block.push_back(static_cast<uint8_t>(textCount + optionCount));
    block.insert(block.end(), entries.begin(), entries.end());
    return block;
}

std::vector<uint8_t> ReadGruntyLangBlock(const std::shared_ptr<Ship::BinaryReader>& reader) {
    std::vector<uint8_t> entries;
    const uint32_t textCount = ReadEntryRun(reader, entries);

    const uint32_t optionCount = reader->ReadUInt32();
    for (uint32_t i = 0; i < optionCount; i++) {
        const uint8_t cmd = reader->ReadUByte();
        const uint8_t unk0 = reader->ReadUByte();
        const uint8_t unk1 = reader->ReadUByte();
        const uint32_t len = reader->ReadUInt32();
        const auto str = ReadSizedString(reader, len);

        entries.push_back(cmd);
        entries.push_back(static_cast<uint8_t>(str.size() + 2));
        entries.push_back(unk0);
        entries.push_back(unk1);
        AppendBytes(entries, str.data(), str.size());
    }

    std::vector<uint8_t> block;
    block.push_back(static_cast<uint8_t>(textCount + optionCount));
    block.insert(block.end(), entries.begin(), entries.end());
    return block;
}

std::vector<uint8_t> BuildQuestionBlob(uint8_t header1, uint8_t header2,
                                       const std::vector<std::vector<uint8_t>>& blocks) {
    const uint32_t langCount = static_cast<uint32_t>(blocks.size());
    const uint32_t headerSize = 3 + langCount * 2;

    std::vector<uint8_t> out;
    out.push_back(static_cast<uint8_t>(langCount));
    out.push_back(header1);
    out.push_back(header2);

    uint32_t pos = headerSize;
    for (const auto& block : blocks) {
        out.push_back(static_cast<uint8_t>(pos & 0xFF));
        out.push_back(static_cast<uint8_t>((pos >> 8) & 0xFF));
        pos += static_cast<uint32_t>(block.size());
    }

    for (const auto& block : blocks) {
        out.insert(out.end(), block.begin(), block.end());
    }

    return out;
}
} // namespace

std::shared_ptr<Ship::IResource>
ResourceFactoryBinaryBKDialogV0::ReadResource(std::shared_ptr<Ship::File> file,
                                              std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    const uint32_t langCount = reader->ReadUInt32();

    if (langCount == 1) {
        // US/JP: reconstruct as 01 03 00 [data]
        // dialogBin_get reads bytes 1-2 as LE u16 offset (0x03, 0x00 = 3), data at byte 3
        auto block = ReadLangBlock(reader);
        std::vector<uint8_t> out;
        out.push_back(0x01); // header byte 0
        out.push_back(0x03); // byte 1: offset lo = 3
        out.push_back(0x00); // byte 2: offset hi = 0
        out.insert(out.end(), block.begin(), block.end());
        return MakeBlob(initData, std::move(out));
    }

    // PAL (3 languages): reconstruct as [header] [fr_off LE u16] [de_off LE u16] [EN] [FR] [DE]
    // dialogBin_get: ptr+1, skip lang*2, read LE u16 offset, jump to ptr+offset
    //   lang=0: bytes 1-2 = EN offset (= 7, right after header+offsets)
    //   lang=1: bytes 3-4 = FR offset
    //   lang=2: bytes 5-6 = DE offset

    // Read all language blocks from o2r
    std::vector<std::vector<uint8_t>> blocks;
    for (uint32_t i = 0; i < langCount; i++) {
        blocks.push_back(ReadLangBlock(reader));
    }

    // Header is 1 byte + (langCount * 2) bytes for offset table
    const uint32_t headerSize = 1 + langCount * 2;

    // Calculate offsets for each language block
    std::vector<uint16_t> offsets;
    uint32_t pos = headerSize;
    for (uint32_t i = 0; i < langCount; i++) {
        offsets.push_back(static_cast<uint16_t>(pos));
        pos += static_cast<uint32_t>(blocks[i].size());
    }

    // Build the blob
    std::vector<uint8_t> out;
    out.push_back(0x03); // PAL header byte 0

    // Write offset table (LE u16 for each language)
    for (uint32_t i = 0; i < langCount; i++) {
        out.push_back(static_cast<uint8_t>(offsets[i] & 0xFF));
        out.push_back(static_cast<uint8_t>((offsets[i] >> 8) & 0xFF));
    }

    // Append all language blocks
    for (const auto& block : blocks) {
        out.insert(out.end(), block.begin(), block.end());
    }

    return MakeBlob(initData, std::move(out));
}

std::shared_ptr<Ship::IResource>
ResourceFactoryBinaryBKQuizQuestionV0::ReadResource(std::shared_ptr<Ship::File> file,
                                                    std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    const uint32_t langCount = reader->ReadUInt32();

    std::vector<std::vector<uint8_t>> blocks;
    for (uint32_t i = 0; i < langCount; i++) {
        blocks.push_back(ReadQuizLangBlock(reader));
    }

    return MakeBlob(initData, BuildQuestionBlob(0x01, 0x02, blocks));
}

std::shared_ptr<Ship::IResource>
ResourceFactoryBinaryBKGruntyQuestionV0::ReadResource(std::shared_ptr<Ship::File> file,
                                                      std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    const uint32_t langCount = reader->ReadUInt32();

    std::vector<std::vector<uint8_t>> blocks;
    for (uint32_t i = 0; i < langCount; i++) {
        blocks.push_back(ReadGruntyLangBlock(reader));
    }

    return MakeBlob(initData, BuildQuestionBlob(0x03, 0x00, blocks));
}
} // namespace Factories
