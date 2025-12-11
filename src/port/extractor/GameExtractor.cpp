#include "GameExtractor.h"

#include <fstream>

#include "Context.h"
#include "spdlog/spdlog.h"
#include <port/Engine.h>

std::unordered_map<std::string, std::string> mGameList = {
    { "1fe1632098865f639e22c11b9a81ee8f29c75d7a", "Banjo Kazooie (U) (V1.0)" },
    { "ded6ee166e740ad1bc810fd678a84b48e245ab80", "Banjo Kazooie (U) (V1.1)" },
};

bool GameExtractor::SelectGameFromUI() {
    this->mGamePath = Ship::Context::GetPathRelativeToAppDirectory("baserom.z64");

    std::ifstream file(this->mGamePath, std::ios::binary);
    this->mGameData = std::vector<uint8_t>( std::istreambuf_iterator( file ), {} );
    file.close();
    return true;
}

std::optional<std::string> GameExtractor::ValidateChecksum() const {
    const auto rom = new N64::Cartridge(this->mGameData);
    rom->Initialize();
    auto hash = rom->GetHash();
    
    if (mGameList.find(hash) == mGameList.end()) {
        return std::nullopt;
    }

    return mGameList[hash];
}

bool GameExtractor::GenerateOTR() const {
    const std::string assets_path = Ship::Context::GetAppBundlePath();
    const std::string game_path = Ship::Context::GetAppDirectoryPath();

    Companion::Instance = new Companion(this->mGameData, ArchiveType::O2R, false, assets_path, game_path);

    try {
        Companion::Instance->Init(ExportType::Binary);
    } catch (const std::exception& e) {
        return false;
    }

    return true;
}
