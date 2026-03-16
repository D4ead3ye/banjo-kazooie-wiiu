#include "Engine.h"
#include "ship/utils/StringHelper.h"
#include "ship/window/gui/Fonts.h"
#include "ship/window/gui/resource/Font.h"
#include "extractor/GameExtractor.h"
#include <libultraship/controller/controldeck/ControlDeck.h>
#include "ship/controller/controldevice/controller/mapping/ControllerDefaultMappings.h"

#include <fast/Fast3dWindow.h>
#include "fast/resource/ResourceType.h"
#include <fast/resource/factory/DisplayListFactory.h>
#include <fast/resource/factory/TextureFactory.h>
#include <fast/resource/factory/MatrixFactory.h>
#include <fast/resource/factory/VertexFactory.h>
#include <ship/resource/factory/BlobFactory.h>
#include "resource/importers/AnimFactory.h"
#include "resource/importers/DemoInputFactory.h"
#include "resource/importers/DialogFactory.h"
#include "resource/importers/MapFactory.h"
#include "resource/importers/ModelFactory.h"
#include "resource/importers/SpriteFactory.h"
#include "audio/GameAudio.h"
#include "ui/LighthouseGui.hpp"
// #include "port/patches/DisplayListPatch.h"
// #include "port/mods/PortEnhancements.h"

#include <fast/interpreter.h>
#include <libultraship/bridge/gfxbridge.h>
#include <filesystem>
#include <fstream>
#include <libultraship/libultraship.h>

#ifdef __SWITCH__
#include <port/switch/SwitchImpl.h>
#endif

namespace fs = std::filesystem;

extern "C" {
bool prevAltAssets = false;
// bool gEnableGammaBoost = true;
}

std::vector<uint8_t*> MemoryPool;
GameEngine* GameEngine::Instance;

GameEngine::GameEngine() {
    // Initialize context properties early to recognize paths properly for non-portable builds
    this->context = Ship::Context::CreateUninitializedInstance("Lighthouse", "ship", "lighthouse.cfg.json");

#ifdef __SWITCH__
    Ship::Switch::Init(Ship::PreInitPhase);
    Ship::Switch::Init(Ship::PostInitPhase);
#endif

    std::vector<std::string> archiveFiles;
    const std::string main_path = Ship::Context::GetPathRelativeToAppDirectory("bk.o2r");
    const std::string assets_path = Ship::Context::LocateFileAcrossAppDirs("lighthouse.o2r");

#ifdef _WIN32
    AllocConsole();
#endif

    if (std::filesystem::exists(main_path)) {
        archiveFiles.push_back(main_path);
    } else {
        if (ShowYesNoBox("Lighthouse - Asset Extraction",
                         "Please provide a Banjo-Kazooie ROM.\n\nSupported Versions:\nUS 1.0\nUS 1.1\n\nAssets will be "
                         "extracted into an O2R file.") == IDYES) {
            if (!GenAssetFile()) {
                ShowMessage("Error", "An error occured, no O2R file was generated.\n\nExiting...");
                exit(1);
            } else {
                archiveFiles.push_back(main_path);
            }

            if (ShowYesNoBox("Extraction Complete", "ROM Extracted. Extract another?") == IDYES) {
                if (!GenAssetFile()) {
                    ShowMessage("Error", "An error occured, no O2R file was generated.");
                }
            }
        } else {
            exit(1);
        }
    }

    if (std::filesystem::exists(assets_path)) {
        archiveFiles.push_back(assets_path);
    }

    if (const std::string patches_path = Ship::Context::GetPathRelativeToAppDirectory("mods"); !patches_path.empty()) {
        if (!std::filesystem::exists(patches_path)) {
            std::filesystem::create_directories(patches_path);
        }

        if (std::filesystem::is_directory(patches_path)) {
            for (const auto& p : std::filesystem::recursive_directory_iterator(patches_path)) {
                const auto ext = p.path().extension().string();
                if (StringHelper::IEquals(ext, ".otr") || StringHelper::IEquals(ext, ".o2r")) {
                    archiveFiles.push_back(p.path().generic_string());
                }

                if (StringHelper::IEquals(ext, ".zip")) {
                    SPDLOG_WARN("Zip files should be only used for development purposes, not for distribution");
                    archiveFiles.push_back(p.path().generic_string());
                }
            }
        }
    }

    this->context->InitConfiguration();    // without this line InitConsoleVariables fails at Config::Reload()
    this->context->InitConsoleVariables(); // without this line the controldeck constructor failes in
                                           // ShipDeviceIndexMappingManager::UpdateControllerNamesFromConfig()

    auto controlDeck = std::make_shared<LUS::ControlDeck>(std::vector<CONTROLLERBUTTONS_T>());

    this->context->InitResourceManager(archiveFiles, {}, 3); // without this line InitWindow fails in Gui::Init()
    this->context->InitConsole(); // without this line the GuiWindow constructor fails in ConsoleWindow::InitElement()

    auto window = std::make_shared<Fast::Fast3dWindow>(std::vector<std::shared_ptr<Ship::GuiWindow>>({}));

    // [port] Truncate log file before LUS init so each run starts fresh
    {
        auto logPath = Ship::Context::GetPathRelativeToAppDirectory("logs/Lighthouse.log");
        std::filesystem::create_directories(std::filesystem::path(logPath).parent_path());
        std::ofstream(logPath, std::ios::trunc).close();
    }

    auto audioChannelsSetting = Ship::Context::GetInstance()->GetConfig()->GetCurrentAudioChannelsSetting();
    this->context->Init(archiveFiles, {}, 3, { 32000, 1024, 1680, audioChannelsSetting }, window, controlDeck);

#ifndef __SWITCH__
    Ship::Context::GetInstance()->GetLogger()->set_level(
        (spdlog::level::level_enum)CVarGetInteger("gDeveloperTools.LogLevel", 1));
    Ship::Context::GetInstance()->GetLogger()->set_pattern("[%H:%M:%S.%e] [%s:%#] [%l] %v");
#endif

    auto loader = context->GetResourceManager()->GetResourceLoader();
    loader->RegisterResourceFactory(std::make_shared<Factories::ResourceFactoryBinarySpriteV0>(),
                                    RESOURCE_FORMAT_BINARY, "Sprite",
                                    static_cast<uint32_t>(Torch::ResourceType::BKSprite), 0);
    loader->RegisterResourceFactory(std::make_shared<Factories::ResourceFactoryBinaryModelV0>(), RESOURCE_FORMAT_BINARY,
                                    "Model", static_cast<uint32_t>(Torch::ResourceType::BKModel), 0);
    loader->RegisterResourceFactory(std::make_shared<Factories::ResourceFactoryBinaryBKAnimationV0>(),
                                    RESOURCE_FORMAT_BINARY, "BKAnimation",
                                    static_cast<uint32_t>(Torch::ResourceType::BKAnimation), 0);
    loader->RegisterResourceFactory(std::make_shared<Factories::ResourceFactoryBinaryBKDialogV0>(),
                                    RESOURCE_FORMAT_BINARY, "BKDialog",
                                    static_cast<uint32_t>(Torch::ResourceType::BKDialog), 0);
    loader->RegisterResourceFactory(std::make_shared<Factories::ResourceFactoryBinaryBKQuizQuestionV0>(),
                                    RESOURCE_FORMAT_BINARY, "BKQuizQuestion",
                                    static_cast<uint32_t>(Torch::ResourceType::BKQuizQuestion), 0);
    loader->RegisterResourceFactory(std::make_shared<Factories::ResourceFactoryBinaryBKGruntyQuestionV0>(),
                                    RESOURCE_FORMAT_BINARY, "BKGruntyQuestion",
                                    static_cast<uint32_t>(Torch::ResourceType::BKGruntyQuestion), 0);
    loader->RegisterResourceFactory(std::make_shared<Factories::ResourceFactoryBinaryBKDemoInputV0>(),
                                    RESOURCE_FORMAT_BINARY, "BKDemoInput",
                                    static_cast<uint32_t>(Torch::ResourceType::BKDemoInput), 0);
    loader->RegisterResourceFactory(std::make_shared<Factories::ResourceFactoryBinaryBKMapV0>(), RESOURCE_FORMAT_BINARY,
                                    "BKMap", static_cast<uint32_t>(Torch::ResourceType::BKMap), 0);
    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryAnimV0>(), RESOURCE_FORMAT_BINARY,
    //                                 "Animation", static_cast<uint32_t>(SF64::ResourceType::AnimData), 0);
    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinarySkeletonV0>(),
    // RESOURCE_FORMAT_BINARY,
    //                                 "Skeleton", static_cast<uint32_t>(SF64::ResourceType::Skeleton), 0);
    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryLimbV0>(), RESOURCE_FORMAT_BINARY,
    //                                 "Limb", static_cast<uint32_t>(SF64::ResourceType::Limb), 0);
    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryMessageV0>(), RESOURCE_FORMAT_BINARY,
    //                                 "Message", static_cast<uint32_t>(SF64::ResourceType::Message), 0);
    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryMessageLookupV0>(),
    //                                 RESOURCE_FORMAT_BINARY, "MessageTable",
    //                                 static_cast<uint32_t>(SF64::ResourceType::MessageTable), 0);
    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryEnvSettingsV0>(),
    //                                 RESOURCE_FORMAT_BINARY, "EnvSettings",
    //                                 static_cast<uint32_t>(SF64::ResourceType::Environment), 0);
    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryObjectInitV0>(),
    // RESOURCE_FORMAT_BINARY,
    //                                 "ObjectInit", static_cast<uint32_t>(SF64::ResourceType::ObjectInit), 0);
    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryHitboxV0>(), RESOURCE_FORMAT_BINARY,
    //                                 "Hitbox", static_cast<uint32_t>(SF64::ResourceType::Hitbox), 0);
    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryScriptV0>(), RESOURCE_FORMAT_BINARY,
    //                                 "Script", static_cast<uint32_t>(SF64::ResourceType::Script), 0);
    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryScriptCMDV0>(),
    // RESOURCE_FORMAT_BINARY,
    //                                 "ScriptCMD", static_cast<uint32_t>(SF64::ResourceType::ScriptCmd), 0);
    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryColPolyV0>(), RESOURCE_FORMAT_BINARY,
    //                                 "ColPoly", static_cast<uint32_t>(SF64::ResourceType::ColPoly), 0);
    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryVec3fV0>(), RESOURCE_FORMAT_BINARY,
    //                                 "Vec3f", static_cast<uint32_t>(SF64::ResourceType::Vec3f), 0);
    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryVec3sV0>(), RESOURCE_FORMAT_BINARY,
    //                                 "Vec3s", static_cast<uint32_t>(SF64::ResourceType::Vec3s), 0);
    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryGenericArrayV0>(),
    //                                 RESOURCE_FORMAT_BINARY, "GenericArray",
    //                                 static_cast<uint32_t>(SF64::ResourceType::GenericArray), 0);
    loader->RegisterResourceFactory(std::make_shared<Fast::ResourceFactoryBinaryTextureV0>(), RESOURCE_FORMAT_BINARY,
                                    "Texture", static_cast<uint32_t>(Fast::ResourceType::Texture), 0);
    loader->RegisterResourceFactory(std::make_shared<Fast::ResourceFactoryBinaryTextureV1>(), RESOURCE_FORMAT_BINARY,
                                    "Texture", static_cast<uint32_t>(Fast::ResourceType::Texture), 1);

    loader->RegisterResourceFactory(std::make_shared<Fast::ResourceFactoryBinaryVertexV0>(), RESOURCE_FORMAT_BINARY,
                                    "Vertex", static_cast<uint32_t>(Fast::ResourceType::Vertex), 0);
    loader->RegisterResourceFactory(std::make_shared<Fast::ResourceFactoryXMLVertexV0>(), RESOURCE_FORMAT_XML, "Vertex",
                                    static_cast<uint32_t>(Fast::ResourceType::Vertex), 0);

    loader->RegisterResourceFactory(std::make_shared<Fast::ResourceFactoryBinaryDisplayListV0>(),
                                    RESOURCE_FORMAT_BINARY, "DisplayList",
                                    static_cast<uint32_t>(Fast::ResourceType::DisplayList), 0);
    loader->RegisterResourceFactory(std::make_shared<Fast::ResourceFactoryXMLDisplayListV0>(), RESOURCE_FORMAT_XML,
                                    "DisplayList", static_cast<uint32_t>(Fast::ResourceType::DisplayList), 0);

    loader->RegisterResourceFactory(std::make_shared<Fast::ResourceFactoryBinaryMatrixV0>(), RESOURCE_FORMAT_BINARY,
                                    "Matrix", static_cast<uint32_t>(Fast::ResourceType::Matrix), 0);

    loader->RegisterResourceFactory(std::make_shared<Ship::ResourceFactoryBinaryBlobV0>(), RESOURCE_FORMAT_BINARY,
                                    "Blob", static_cast<uint32_t>(Ship::ResourceType::Blob), 0);

    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryAudioTableV0>(),
    // RESOURCE_FORMAT_BINARY,
    //                                 "AudioTable", static_cast<uint32_t>(SF64::ResourceType::AudioTable), 0);

    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryAdpcmBookV0>(),
    // RESOURCE_FORMAT_BINARY,
    //                                 "AdpcmBook", static_cast<uint32_t>(SF64::ResourceType::AdpcmBook), 0);

    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryDrumV0>(), RESOURCE_FORMAT_BINARY,
    //                                 "Drum", static_cast<uint32_t>(SF64::ResourceType::Drum), 0);

    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryEnvelopeV0>(),
    // RESOURCE_FORMAT_BINARY,
    //                                 "Envelope", static_cast<uint32_t>(SF64::ResourceType::Envelope), 0);

    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryInstrumentV0>(),
    // RESOURCE_FORMAT_BINARY,
    //                                 "Instrument", static_cast<uint32_t>(SF64::ResourceType::Instrument), 0);

    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinaryAdpcmLoopV0>(),
    // RESOURCE_FORMAT_BINARY,
    //                                 "AdpcmLoop", static_cast<uint32_t>(SF64::ResourceType::AdpcmLoop), 0);

    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinarySampleV1>(), RESOURCE_FORMAT_BINARY,
    //                                 "Sample", static_cast<uint32_t>(SF64::ResourceType::Sample), 1);

    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryXMLSampleV0>(), RESOURCE_FORMAT_XML,
    //                                 "Sample", static_cast<uint32_t>(SF64::ResourceType::Sample), 0);

    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryBinarySoundFontV0>(),
    // RESOURCE_FORMAT_BINARY,
    //                                 "SoundFont", static_cast<uint32_t>(SF64::ResourceType::SoundFont), 0);

    // loader->RegisterResourceFactory(std::make_shared<SF64::ResourceFactoryXMLSoundFontV0>(), RESOURCE_FORMAT_XML,
    //                                 "SoundFont", static_cast<uint32_t>(SF64::ResourceType::SoundFont), 0);

    prevAltAssets = CVarGetInteger("gEnhancements.Mods.AlternateAssets", 0);
    context->GetResourceManager()->SetAltAssetsEnabled(prevAltAssets);

    fontMono = CreateFontWithSize(16.0f, "fonts/Inconsolata-Regular.ttf");
    fontMonoLarger = CreateFontWithSize(20.0f, "fonts/Inconsolata-Regular.ttf");
    fontMonoLargest = CreateFontWithSize(24.0f, "fonts/Inconsolata-Regular.ttf");
    fontStandard = CreateFontWithSize(16.0f, "fonts/Montserrat-Regular.ttf");
    fontStandardLarger = CreateFontWithSize(20.0f, "fonts/Montserrat-Regular.ttf");
    fontStandardLargest = CreateFontWithSize(24.0f, "fonts/Montserrat-Regular.ttf");
    ImGui::GetIO().FontDefault = fontMono;
}

bool GameEngine::GenAssetFile(bool exitOnFail) {
    auto extractor = new GameExtractor();

    if (!extractor->SelectGameFromUI()) {
        ShowMessage("Error", "No ROM selected.\n\nExiting...");
        if (exitOnFail) {
            exit(1);
        } else {
            return false;
        }
    }

    auto game = extractor->ValidateChecksum();
    if (!game.has_value()) {
        ShowMessage("Unsupported ROM",
                    "The provided ROM is not supported.\n\nCheck the readme for a list of supported versions.");
        if (exitOnFail) {
            exit(1);
        } else {
            return false;
        }
    }

    ShowMessage(("Lighthouse - Extraction - Found " + game.value()).c_str(),
                "The extraction process will now begin.\n\nThis may take a few minutes.", SDL_MESSAGEBOX_INFORMATION);

    return extractor->GenerateOTR();
}

void GameEngine::Create() {
    const auto instance = Instance = new GameEngine();
    // instance->AudioInit();
    // DisplayListPatch::Run();
    // [port] BK renders at 292x216, not the standard 320x240.
    GfxSetNativeDimensions(292, 216);
    LighthouseGui::SetupGuiElements();
#if defined(__SWITCH__) || defined(__WIIU__)
    CVarRegisterInteger("gControlNav", 1); // always enable controller nav on switch/wii u
    osSetTime(0);
#endif
    // PortEnhancements_Init();
}

void GameEngine::Destroy() {
    // PortEnhancements_Exit();
    // AudioExit();
    for (auto ptr : MemoryPool) {
        free(ptr);
    }
    MemoryPool.clear();
#ifdef __SWITCH__
    Ship::Switch::Exit();
#endif
}

void GameEngine::StartFrame() const {
    using Ship::KbScancode;
    const int32_t dwScancode = this->context->GetWindow()->GetLastScancode();
    this->context->GetWindow()->SetLastScancode(-1);

    switch (dwScancode) {
        case KbScancode::LUS_KB_TAB: {
            // Toggle HD Assets
            CVarSetInteger("gEnhancements.Mods.AlternateAssets",
                           !CVarGetInteger("gEnhancements.Mods.AlternateAssets", 0));
            break;
        }
        case KbScancode::LUS_KB_F4: {
            // gNextGameState = GSTATE_BOOT;
            break;
        }
        default:
            break;
    }
}

#if 0
// Values for 44100 hz
#define SAMPLES_HIGH 752
#define SAMPLES_LOW 720
#else
// Values for 32000 hz
#define SAMPLES_HIGH 560
#define SAMPLES_LOW 528

#endif

// [port] BK runs at 30fps on N64 (2 vertical interrupts per game frame).
// gVIsPerFrame=2 means original_fps = 60/2 = 30, which correctly paces game logic.
#define gVIsPerFrame 2

void GameEngine::HandleAudioThread() {
    static unsigned short samples_high = SAMPLES_HIGH;
    static unsigned short samples_low = SAMPLES_LOW;
    static int countermin = 0;
    static int frames = 0;
#ifdef PIPE_DEBUG
    std::ofstream outfile("audio.bin", std::ios::binary | std::ios::app);
#endif
    while (audio.running) {
        {
            std::unique_lock<std::mutex> Lock(audio.mutex);
            while (!audio.processing && audio.running) {
                audio.cv_to_thread.wait(Lock);
            }
            if (!audio.running) {
                break;
            }
        }

        // gVIsPerFrame = 2;

#define AUDIO_FRAMES_PER_UPDATE (gVIsPerFrame > 0 ? gVIsPerFrame : 1)
#define MAX_AUDIO_FRAMES_PER_UPDATE 5 // Compile-time constant with max value of gVIsPerFrame

        std::unique_lock<std::mutex> Lock(audio.mutex);
        int samples_left = AudioPlayerBuffered();
        u32 num_audio_samples = samples_left < AudioPlayerGetDesiredBuffered() ? (((samples_high))) : (((samples_low)));

        frames++;

        if (frames > 60) {
            countermin++;
        }

        // const int32_t num_audio_channels = GetNumAudioChannels();

        // s16 audio_buffer[SAMPLES_HIGH * MAX_NUM_AUDIO_CHANNELS * MAX_AUDIO_FRAMES_PER_UPDATE] = { 0 };
        // for (int i = 0; i < AUDIO_FRAMES_PER_UPDATE; i++) {
        //     AudioThread_CreateNextAudioBuffer(audio_buffer + i * (num_audio_samples * num_audio_channels),
        //                                       num_audio_samples);
        // }
#ifdef PIPE_DEBUG
        if (outfile.is_open()) {
            outfile.write(reinterpret_cast<char*>(audio_buffer),
                          num_audio_samples * (sizeof(int16_t) * num_audio_channels * AUDIO_FRAMES_PER_UPDATE));
        }
#endif
        // AudioPlayerPlayFrame((u8*) audio_buffer,
        //                      num_audio_samples * (sizeof(int16_t) * num_audio_channels * AUDIO_FRAMES_PER_UPDATE));

        audio.processing = false;
        audio.cv_from_thread.notify_one();
    }
#ifdef PIPE_DEBUG
    outfile.close();
#endif
}

void GameEngine::StartAudioFrame() {
    {
        std::unique_lock<std::mutex> Lock(audio.mutex);
        audio.processing = true;
    }
    audio.cv_to_thread.notify_one();
}

void GameEngine::EndAudioFrame() {
    {
        std::unique_lock<std::mutex> Lock(audio.mutex);
        while (audio.processing) {
            audio.cv_from_thread.wait(Lock);
        }
    }
}

void GameEngine::AudioInit() {
    if (!audio.running) {
        audio.running = true;
        audio.thread = std::thread(HandleAudioThread);
    }
}

void GameEngine::AudioExit() {
    {
        std::unique_lock lock(audio.mutex);
        audio.running = false;
    }
    audio.cv_to_thread.notify_all();
    // Wait until the audio thread quit
    audio.thread.join();
}

// [port] GPU→CPU framebuffer readback — defined in Game.cpp
void Framebuffer_ReadbackGPU_FromBackbuffer(Fast::Interpreter* interpreter);
extern "C" int port_isViBlack(void);

void GameEngine::RunCommands(Gfx* Commands, const std::vector<std::unordered_map<Mtx*, MtxF>>& mtx_replacements) {
    auto wnd = std::dynamic_pointer_cast<Fast::Fast3dWindow>(Ship::Context::GetInstance()->GetWindow());

    if (wnd == nullptr) {
        return;
    }

    auto interpreter = wnd->GetInterpreterWeak().lock().get();

    // Process window events for resize, mouse, keyboard events
    wnd->HandleEvents();

    interpreter->mInterpolationIndex = 0;

    // [port] Expand DrawAndRunGraphicsCommands so we can read the backbuffer between
    // Run() (frame rendered) and EndFrame() (buffer swap). On N64, CPU/RDP shared
    // physical memory so gFramebuffers always had valid pixel data after rendering.
    auto wndBase = Ship::Context::GetInstance()->GetWindow();
    for (const auto& m : mtx_replacements) {
        if (wndBase->IsFrameReady()) {
            auto gui = wndBase->GetGui();
            wndBase->GetMouseStateManager()->StartFrame();
            gui->StartDraw();
            interpreter->StartFrame();
            interpreter->Run(Commands, m);
            // Read backbuffer into gFramebuffers before the swap destroys it
            Framebuffer_ReadbackGPU_FromBackbuffer(interpreter);
            // [port] Emulate N64 osViBlack: after readback captured the world,
            // clear the game framebuffer to black so the player sees nothing.
            // On N64, osViBlack blanked TV output but the RDP still rendered.
            if (port_isViBlack()) {
                int gameFb = interpreter->mRendersToFb ? interpreter->mGameFb : 0;
                interpreter->mRapi->StartDrawToFramebuffer(gameFb, 1);
                interpreter->mRapi->ClearFramebuffer(true, false);
            }
            gui->EndDraw();
            interpreter->EndFrame();
        }
        interpreter->mInterpolationIndex++;
    }

    bool curAltAssets = CVarGetInteger("gEnhancements.Mods.AlternateAssets", 0);
    if (prevAltAssets != curAltAssets) {
        prevAltAssets = curAltAssets;
        Ship::Context::GetInstance()->GetResourceManager()->SetAltAssetsEnabled(curAltAssets);
        gfx_texture_cache_clear();
    }
}

void GameEngine::ProcessGfxCommands(Gfx* commands) {
    auto wnd = std::dynamic_pointer_cast<Fast::Fast3dWindow>(Ship::Context::GetInstance()->GetWindow());

    if (wnd == nullptr) {
        return;
    }

    // if(gEnableGammaBoost) {
    //     wnd->EnableSRGBMode();
    // }
    wnd->SetRendererUCode(UcodeHandlers::ucode_f3dex);

    std::vector<std::unordered_map<Mtx*, MtxF>> mtx_replacements;
    int target_fps = GameEngine::Instance->GetInterpolationFPS();
    static int last_fps;
    static int last_update_rate;
    static int time;
    int fps = target_fps;
    int original_fps = 60 / gVIsPerFrame;

    if (target_fps == 20 || original_fps > target_fps) {
        fps = original_fps;
    }

    if (last_fps != fps || last_update_rate != gVIsPerFrame) {
        time = 0;
    }

    // time_base = fps * original_fps (one second)
    int next_original_frame = fps;

    while (time + original_fps <= next_original_frame) {
        time += original_fps;
        if (time != next_original_frame) {
            // mtx_replacements.push_back(FrameInterpolation_Interpolate((float) time / next_original_frame));
        } else {
            mtx_replacements.emplace_back();
        }
    }

    time -= fps;

    if (wnd != nullptr) {
        wnd->SetTargetFps(fps);
        wnd->SetMaximumFrameLatency(
            2); // [port] Hardcoded: CVarGetInteger crashes due to heap corruption in debug builds
    }

    // When the gfx debugger is active, only run with the final mtx
    if (GfxDebuggerIsDebugging()) {
        mtx_replacements.clear();
        mtx_replacements.emplace_back();
    }

    RunCommands(commands, mtx_replacements);

    last_fps = fps;
    last_update_rate = gVIsPerFrame;
}

uint32_t GameEngine::GetInterpolationFPS() {
    if (CVarGetInteger("gMatchRefreshRate", 0)) {
        return Ship::Context::GetInstance()->GetWindow()->GetCurrentRefreshRate();

    } else if (CVarGetInteger("gVsyncEnabled", 1) ||
               !Ship::Context::GetInstance()->GetWindow()->CanDisableVerticalSync()) {
        return std::min<uint32_t>(Ship::Context::GetInstance()->GetWindow()->GetCurrentRefreshRate(),
                                  CVarGetInteger("gInterpolationFPS", 60));
    }

    return CVarGetInteger("gInterpolationFPS", 60);
}

uint32_t GameEngine::GetInterpolationFrameCount() {
    return ceil((float)GetInterpolationFPS() / (60.0f / gVIsPerFrame));
}

extern "C" uint32_t GameEngine_GetInterpolationFrameCount() {
    return GameEngine::GetInterpolationFrameCount();
}

void GameEngine::ShowMessage(const char* title, const char* message, SDL_MessageBoxFlags type) {
#if defined(__SWITCH__)
    SPDLOG_ERROR(message);
#else
    SDL_ShowSimpleMessageBox(type, title, message, nullptr);
    SPDLOG_ERROR(message);
#endif
}

int GameEngine::ShowYesNoBox(const char* title, const char* box) {
    int ret;
#ifdef _WIN32
    ret = MessageBoxA(nullptr, box, title, MB_YESNO | MB_ICONQUESTION);
#elif defined(__SWITCH__)
    SPDLOG_ERROR(box);
    return IDYES;
#else
    SDL_MessageBoxData boxData = { 0 };
    SDL_MessageBoxButtonData buttons[2] = { { 0 } };

    buttons[0].buttonid = IDYES;
    buttons[0].text = "Yes";
    buttons[0].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
    buttons[1].buttonid = IDNO;
    buttons[1].text = "No";
    buttons[1].flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
    boxData.numbuttons = 2;
    boxData.flags = SDL_MESSAGEBOX_INFORMATION;
    boxData.message = box;
    boxData.title = title;
    boxData.buttons = buttons;
    SDL_ShowMessageBox(&boxData, &ret);
#endif
    return ret;
}

ImFont* GameEngine::CreateFontWithSize(float size, std::string fontPath) {
    auto mImGuiIo = &ImGui::GetIO();
    ImFont* font;
    if (fontPath == "") {
        ImFontConfig fontCfg = ImFontConfig();
        fontCfg.OversampleH = fontCfg.OversampleV = 1;
        fontCfg.PixelSnapH = true;
        fontCfg.SizePixels = size;
        font = mImGuiIo->Fonts->AddFontDefault(&fontCfg);
    } else {
        auto initData = std::make_shared<Ship::ResourceInitData>();
        ImFontConfig config;
        config.FontDataOwnedByAtlas = false;

        initData->Format = RESOURCE_FORMAT_BINARY;
        initData->Type = static_cast<uint32_t>(RESOURCE_TYPE_FONT);
        initData->ResourceVersion = 0;
        initData->Path = fontPath;
        std::shared_ptr<Ship::Font> fontData = std::static_pointer_cast<Ship::Font>(
            Ship::Context::GetInstance()->GetResourceManager()->LoadResource(fontPath, false, initData));
        font = mImGuiIo->Fonts->AddFontFromMemoryTTF(fontData->Data, fontData->DataSize, size, &config);
    }
    // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly
    float iconFontSize = size * 2.0f / 3.0f;
    static const ImWchar sIconsRanges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig iconsConfig;
    iconsConfig.MergeMode = true;
    iconsConfig.PixelSnapH = true;
    iconsConfig.GlyphMinAdvanceX = iconFontSize;
    mImGuiIo->Fonts->AddFontFromMemoryCompressedBase85TTF(fontawesome_compressed_data_base85, iconFontSize,
                                                          &iconsConfig, sIconsRanges);

    return font;
}

bool GameEngine::HasVersion(BKVersion ver) {
    auto versions = Ship::Context::GetInstance()->GetResourceManager()->GetArchiveManager()->GetGameVersions();
    return std::find(versions.begin(), versions.end(), ver) != versions.end();
}

extern "C" bool GameEngine_HasVersion(BKVersion ver) {
    return GameEngine::HasVersion(ver);
}

extern "C" uint32_t GameEngine_GetSampleRate() {
    auto player = Ship::Context::GetInstance()->GetAudio()->GetAudioPlayer();
    if (player == nullptr) {
        return 0;
    }

    if (!player->IsInitialized()) {
        return 0;
    }

    return player->GetSampleRate();
}

#define SAMPLES_PER_FRAME ADPCMFSIZE

extern "C" uint32_t GameEngine_GetSamplesPerFrame() {
    return SAMPLES_PER_FRAME;
}

// End

Fast::Interpreter* GameEngine_GetInterpreter() {
    return std::dynamic_pointer_cast<Fast::Fast3dWindow>(Ship::Context::GetInstance()->GetWindow())
        ->GetInterpreterWeak()
        .lock()
        .get();
}

extern "C" float GameEngine_GetAspectRatio() {
    auto interpreter = GameEngine_GetInterpreter();
    return interpreter->mCurDimensions.aspect_ratio;
}

extern "C" uint32_t GameEngine_GetGameVersion() {
    return 0x00000001;
}

static const char* sOtrSignature = "__OTR__";

extern "C" uint8_t GameEngine_OTRSigCheck(const char* data) {
    if (data == nullptr) {
        return 0;
    }
    return strncmp(data, sOtrSignature, strlen(sOtrSignature)) == 0;
}

extern "C" void GameEngine_GetTextureInfo(const char* path, int32_t* width, int32_t* height, float* scale,
                                          bool* custom) {
    if (GameEngine_OTRSigCheck(path) != 1) {
        *custom = false;
        return;
    }
    std::shared_ptr<Fast::Texture> tex = std::static_pointer_cast<Fast::Texture>(
        Ship::Context::GetInstance()->GetResourceManager()->LoadResourceProcess(path));
    *width = tex->Width;
    *height = tex->Height;
    *scale = tex->VPixelScale;
    *custom = tex->Flags & (1 << 0);
}

// Gets the width of the main ImGui window
extern "C" uint32_t OTRGetCurrentWidth() {
    return GameEngine::Instance->context->GetWindow()->GetWidth();
}

// Gets the height of the main ImGui window
extern "C" uint32_t OTRGetCurrentHeight() {
    return GameEngine::Instance->context->GetWindow()->GetHeight();
}

extern "C" float OTRGetHUDAspectRatio() {
    if (CVarGetInteger("gHUDAspectRatio.Enabled", 0) == 0 || CVarGetInteger("gHUDAspectRatio.X", 0) == 0 ||
        CVarGetInteger("gHUDAspectRatio.Y", 0) == 0) {
        return GameEngine_GetAspectRatio();
    }
    return ((float)CVarGetInteger("gHUDAspectRatio.X", 1) / (float)CVarGetInteger("gHUDAspectRatio.Y", 1));
}

extern "C" float OTRGetDimensionFromLeftEdge(float v) {
    auto interpreter = GameEngine_GetInterpreter();
    return (interpreter->mNativeDimensions.width / 2 -
            interpreter->mNativeDimensions.height / 2 * interpreter->mCurDimensions.aspect_ratio + (v));
}

extern "C" float OTRGetDimensionFromRightEdge(float v) {
    auto interpreter = GameEngine_GetInterpreter();
    return (interpreter->mNativeDimensions.width / 2 +
            interpreter->mNativeDimensions.height / 2 * interpreter->mCurDimensions.aspect_ratio -
            (interpreter->mNativeDimensions.width - v));
}

extern "C" float OTRGetDimensionFromLeftEdgeForcedAspect(float v, float aspectRatio) {
    auto interpreter = GameEngine_GetInterpreter();
    return (interpreter->mNativeDimensions.width / 2 -
            interpreter->mNativeDimensions.height / 2 *
                (aspectRatio > 0 ? aspectRatio : interpreter->mCurDimensions.aspect_ratio) +
            (v));
}

extern "C" float OTRGetDimensionFromRightEdgeForcedAspect(float v, float aspectRatio) {
    auto interpreter = GameEngine_GetInterpreter();
    return (interpreter->mNativeDimensions.width / 2 +
            interpreter->mNativeDimensions.height / 2 *
                (aspectRatio > 0 ? aspectRatio : interpreter->mCurDimensions.aspect_ratio) -
            (interpreter->mNativeDimensions.width - v));
}

extern "C" float OTRGetDimensionFromLeftEdgeOverride(float v) {
    return OTRGetDimensionFromLeftEdgeForcedAspect(v, OTRGetHUDAspectRatio());
}

extern "C" float OTRGetDimensionFromRightEdgeOverride(float v) {
    return OTRGetDimensionFromRightEdgeForcedAspect(v, OTRGetHUDAspectRatio());
}

// Gets the width of the current render target area
extern "C" uint32_t OTRGetGameRenderWidth() {
    auto interpreter = GameEngine_GetInterpreter();
    return interpreter->mCurDimensions.width;
}

// Gets the height of the current render target area
extern "C" uint32_t OTRGetGameRenderHeight() {
    auto interpreter = GameEngine_GetInterpreter();
    return interpreter->mCurDimensions.height;
}

extern "C" int16_t OTRGetRectDimensionFromLeftEdge(float v) {
    return ((int)floorf(OTRGetDimensionFromLeftEdge(v)));
}

extern "C" int16_t OTRGetRectDimensionFromRightEdge(float v) {
    return ((int)ceilf(OTRGetDimensionFromRightEdge(v)));
}

extern "C" int16_t OTRGetRectDimensionFromLeftEdgeForcedAspect(float v, float aspectRatio) {
    return ((int)floorf(OTRGetDimensionFromLeftEdgeForcedAspect(v, aspectRatio)));
}

extern "C" int16_t OTRGetRectDimensionFromRightEdgeForcedAspect(float v, float aspectRatio) {
    return ((int)ceilf(OTRGetDimensionFromRightEdgeForcedAspect(v, aspectRatio)));
}

extern "C" int16_t OTRGetRectDimensionFromLeftEdgeOverride(float v) {
    return OTRGetRectDimensionFromLeftEdgeForcedAspect(v, OTRGetHUDAspectRatio());
}

extern "C" int16_t OTRGetRectDimensionFromRightEdgeOverride(float v) {
    return OTRGetRectDimensionFromRightEdgeForcedAspect(v, OTRGetHUDAspectRatio());
}

extern "C" int32_t OTRConvertHUDXToScreenX(int32_t v) {
    auto interpreter = GameEngine_GetInterpreter();
    float gameAspectRatio = interpreter->mCurDimensions.aspect_ratio;
    int32_t gameHeight = interpreter->mCurDimensions.height;
    int32_t gameWidth = interpreter->mCurDimensions.width;
    float hudAspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
    int32_t hudHeight = gameHeight;
    int32_t hudWidth = hudHeight * hudAspectRatio;
    float hudScreenRatio = (hudWidth / (float)SCREEN_WIDTH);
    float hudCoord = v * hudScreenRatio;
    float gameOffset = (gameWidth - hudWidth) / 2;
    float gameCoord = hudCoord + gameOffset;
    float gameScreenRatio = ((float)SCREEN_WIDTH / gameWidth);
    float screenScaledCoord = gameCoord * gameScreenRatio;
    int32_t screenScaledCoordInt = screenScaledCoord;
    return screenScaledCoordInt;
}

extern "C" void* GameEngine_Malloc(size_t size) {
    MemoryPool.push_back(new uint8_t[size]);
    return (void*)MemoryPool.back();
}

extern "C" void GameEngine_Free(void* ptr) {
    for (auto it = MemoryPool.begin(); it != MemoryPool.end(); ++it) {
        if (*it == ptr) {
            delete[] static_cast<uint8_t*>(ptr); // [port] match new uint8_t[] with delete[]
            MemoryPool.erase(it);
            break;
        }
    }
}
