#include <libultraship.h>

#include <Fast3D/interpreter.h>
#include "Engine.h"

extern "C"
void Graphics_PushFrame(Gfx* data) {
    GameEngine::ProcessGfxCommands(data);
}

void push_frame() {

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

    while (WindowIsRunning()) {
        push_frame();
    }
    GameEngine::Instance->Destroy();
    return 0;
}
