#include <libultraship.h>

#include <fast/interpreter.h>
#include "Engine.h"

extern "C" {
#include "enums.h"
#include "core1/main.h"
void game_draw(s32 arg0);
}

extern "C"
void Graphics_PushFrame(Gfx* data) {
    GameEngine::ProcessGfxCommands(data);
}

void push_frame() {
    game_draw(0);

    // GameEngine::StartAudioFrame();
    GameEngine::Instance->StartFrame();

    // GameEngine::EndAudioFrame();
}

/* Rename SDL_main to main for SDL compatibility */
#ifdef __GNUC__
#define SDL_main main
#endif

int SDL_main (int argc, char *argv[]) {
    GameEngine::Create();
    core1_init();

    while (WindowIsRunning()) {
        push_frame();
    }
    GameEngine::Instance->Destroy();
    return 0;
}
