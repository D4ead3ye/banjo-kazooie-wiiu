#include <libultraship.h>

#include <fast/interpreter.h>
#include "Engine.h"

extern "C" {
#include "enums.h"
#include "core1/main.h"
}

extern "C" void Graphics_PushFrame(Gfx* data) {
    GameEngine::ProcessGfxCommands(data);
}

// [port] BK game logic runs at 30fps (N64: 2 VI per game frame at 60Hz).
// The main loop must tick at exactly 30fps regardless of render rate.
// Without this, gGlobalTimer increments too fast, time_getDelta() returns
// tiny values, and all tick-based timing (animations, cutscenes, AI) breaks.
static constexpr double GAME_LOGIC_FPS = 30.0;
static constexpr double GAME_LOGIC_FRAME_TIME = 1.0 / GAME_LOGIC_FPS;

void push_frame() {
    // GameEngine::StartAudioFrame();
    GameEngine::Instance->StartFrame();
    // GameEngine::EndAudioFrame();
    mainLoop();
}

/* Rename SDL_main to main for SDL compatibility */
#ifdef __GNUC__
#define SDL_main main
#endif

int SDL_main(int argc, char* argv[]) {
    GameEngine::Create(argc, argv);
    core1_init();

    uint64_t prev = SDL_GetPerformanceCounter();
    double accumulator = 0.0;

    while (WindowIsRunning()) {
        uint64_t now = SDL_GetPerformanceCounter();
        double elapsed = (double)(now - prev) / (double)SDL_GetPerformanceFrequency();
        prev = now;

        // Cap accumulated time to prevent spiral-of-death after long stalls
        if (elapsed > 0.25) {
            elapsed = 0.25;
        }
        accumulator += elapsed;

        // Tick game logic at fixed 30fps
        if (accumulator >= GAME_LOGIC_FRAME_TIME) {
            push_frame();
            accumulator -= GAME_LOGIC_FRAME_TIME;
        } else {
            // Yield CPU while waiting for next tick
            double remaining = GAME_LOGIC_FRAME_TIME - accumulator;
            if (remaining > 0.002) {
                SDL_Delay((uint32_t)((remaining - 0.001) * 1000.0));
            }
        }
    }
    GameEngine::Instance->Destroy();
    return 0;
}
