#include "DialogFactory.h"

#include <libultraship/libultraship.h>
#include <ship/resource/type/Blob.h>

namespace Factories {
namespace {
template <typename T> void AppendValue(std::vector<uint8_t>& dst, const T& value) {
    const auto base = dst.size();
    dst.resize(base + sizeof(T));
    std::memcpy(dst.data() + base, &value, sizeof(T));
}

void AppendBytes(std::vector<uint8_t>& dst, const char* data, size_t size) {
    const auto base = dst.size();
    dst.resize(base + size);
    std::memcpy(dst.data() + base, data, size);
}

std::string ReadSizedString(const std::shared_ptr<Ship::BinaryReader>& reader, uint32_t len) {
    std::string out;
    out.resize(len);
    if (len > 0) {
        reader->Read(out.data(), static_cast<int32_t>(len));
    }
    return out;
}

std::shared_ptr<Ship::Blob> MakeBlob(const std::shared_ptr<Ship::ResourceInitData>& initData,
                                     std::vector<uint8_t>&& data) {
    auto blob = std::make_shared<Ship::Blob>(initData);
    blob->Data = std::move(data);
    return blob;
}
} // namespace

std::shared_ptr<Ship::IResource>
ResourceFactoryBinaryBKDialogV0::ReadResource(std::shared_ptr<Ship::File> file,
                                              std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);
    auto out = std::vector<uint8_t>();

    out.push_back(0x01);
    out.push_back(0x03);
    out.push_back(0x00);

    const uint32_t bottomCount = reader->ReadUInt32();
    out.push_back(static_cast<uint8_t>(bottomCount));
    for (uint32_t i = 0; i < bottomCount; i++) {
        const uint8_t cmd = reader->ReadUByte();
        const uint32_t len = reader->ReadUInt32();
        const auto str = ReadSizedString(reader, len);

        out.push_back(cmd);
        out.push_back(static_cast<uint8_t>(len));
        AppendBytes(out, str.data(), str.size());
    }

    const uint32_t topCount = reader->ReadUInt32();
    out.push_back(static_cast<uint8_t>(topCount));
    for (uint32_t i = 0; i < topCount; i++) {
        const uint8_t cmd = reader->ReadUByte();
        const uint32_t len = reader->ReadUInt32();
        const auto str = ReadSizedString(reader, len);

        out.push_back(cmd);
        out.push_back(static_cast<uint8_t>(len));
        AppendBytes(out, str.data(), str.size());
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
    auto out = std::vector<uint8_t>();

    out.push_back(0x01);
    out.push_back(0x01);
    out.push_back(0x02);
    out.push_back(0x05);
    out.push_back(0x00);

    const uint32_t textCount = reader->ReadUInt32();
    const uint32_t optionCount = reader->ReadUInt32();
    const uint32_t totalCount = textCount + optionCount;
    out.push_back(static_cast<uint8_t>(totalCount));

    for (uint32_t i = 0; i < textCount; i++) {
        const uint8_t cmd = reader->ReadUByte();
        const uint32_t len = reader->ReadUInt32();
        const auto str = ReadSizedString(reader, len);

        out.push_back(cmd);
        out.push_back(static_cast<uint8_t>(len));
        AppendBytes(out, str.data(), str.size());
    }

    for (uint32_t i = 0; i < optionCount; i++) {
        const uint8_t cmd = reader->ReadUByte();
        const uint32_t len = reader->ReadUInt32();
        const auto str = ReadSizedString(reader, len);

        out.push_back(cmd);
        out.push_back(static_cast<uint8_t>(len));
        AppendBytes(out, str.data(), str.size());
    }

    return MakeBlob(initData, std::move(out));
}

std::shared_ptr<Ship::IResource>
ResourceFactoryBinaryBKGruntyQuestionV0::ReadResource(std::shared_ptr<Ship::File> file,
                                                      std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);
    auto out = std::vector<uint8_t>();

    out.push_back(0x01);
    out.push_back(0x03);
    out.push_back(0x00);
    out.push_back(0x05);
    out.push_back(0x00);

    const uint32_t textCount = reader->ReadUInt32();
    const uint32_t optionCount = reader->ReadUInt32();
    const uint32_t totalCount = textCount + optionCount;
    out.push_back(static_cast<uint8_t>(totalCount));

    for (uint32_t i = 0; i < textCount; i++) {
        const uint8_t cmd = reader->ReadUByte();
        const uint32_t len = reader->ReadUInt32();
        const auto str = ReadSizedString(reader, len);

        out.push_back(cmd);
        out.push_back(static_cast<uint8_t>(len));
        AppendBytes(out, str.data(), str.size());
    }

    for (uint32_t i = 0; i < optionCount; i++) {
        const uint8_t cmd = reader->ReadUByte();
        const uint8_t unk0 = reader->ReadUByte();
        const uint8_t unk1 = reader->ReadUByte();
        const uint32_t len = reader->ReadUInt32();
        const auto str = ReadSizedString(reader, len);

        out.push_back(cmd);
        out.push_back(static_cast<uint8_t>(len + 2));
        out.push_back(unk0);
        out.push_back(unk1);
        AppendBytes(out, str.data(), str.size());
    }

    return MakeBlob(initData, std::move(out));
}
} // namespace Factories
