#include "GameExtractor.h"

#include <fstream>
#include <filesystem>

#include "ship/Context.h"
#include "spdlog/spdlog.h"
#include <port/Engine.h>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#endif

std::unordered_map<std::string, std::string> mGameList = {
    { "1fe1632098865f639e22c11b9a81ee8f29c75d7a", "Banjo Kazooie (U) (V1.0)" },
    { "ded6ee166e740ad1bc810fd678a84b48e245ab80", "Banjo Kazooie (U) (V1.1)" },
};

bool GameExtractor::SelectGameFromUI() {
    // First try to find a ROM in the standard locations
    this->mGamePath = Ship::Context::LocateFileAcrossAppDirs("baserom.z64", "bk");

    if (this->mGamePath.empty() || !std::filesystem::exists(this->mGamePath)) {
#ifdef _WIN32
        // Open a native file picker dialog
        char filePath[MAX_PATH] = { 0 };
        OPENFILENAMEA ofn = {};
        ofn.lStructSize = sizeof(ofn);
        ofn.lpstrFilter = "N64 ROM Files\0*.z64;*.n64;*.v64\0All Files\0*.*\0";
        ofn.lpstrFile = filePath;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrTitle = "Select Banjo-Kazooie ROM";
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

        if (!GetOpenFileNameA(&ofn)) {
            return false;
        }
        this->mGamePath = filePath;
#else
        SPDLOG_ERROR("No ROM found at standard locations");
        return false;
#endif
    }

    std::ifstream file(this->mGamePath, std::ios::binary);
    if (!file.is_open()) {
        SPDLOG_ERROR("Failed to open ROM: {}", this->mGamePath.string());
        return false;
    }
    this->mGameData = std::vector<uint8_t>(std::istreambuf_iterator(file), {});
    file.close();

    if (this->mGameData.empty()) {
        SPDLOG_ERROR("ROM file is empty: {}", this->mGamePath.string());
        return false;
    }

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

    Companion::Instance = new Companion(this->mGameData, ArchiveType::O2R, false, game_path, game_path);

    try {
        Companion::Instance->Init(ExportType::Binary);
    } catch (const std::exception& e) { return false; }

    return true;
}
