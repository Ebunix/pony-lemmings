#include "Window.h"
#include "SDL.h"
#include "StartInfo.h"
#include "AssetLoader.h"
#include "Game.h"
#include "Input.h"
#include <chrono>

StartInfo parseArgs(int argc, char* argv[]) {
    printf("Parsing arguments with argv = [\n");
    for (int i = 0; i < argc; i++) {
        printf("  '%s',\n", argv[i]);
    }
    printf("];\n");

    StartInfo info = { 
        .displayIndex = -1,
        .width = 0,
        .height = 0,
        .gameScale = 4,
        .fullscreen = false
    };

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--display-index") == 0) {
            if (i + 1 >= argc) Fatal("--display-index expects an argument");
            info.displayIndex = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--width") == 0) {
            if (i + 1 >= argc) Fatal("--width expects an argument");
            info.width = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--height") == 0) {
            if (i + 1 >= argc) Fatal("--height expects an argument");
            info.height = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--scale") == 0) {
            if (i + 1 >= argc) Fatal("--scale expects an argument");
            info.gameScale = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--fullscreen") == 0) {
            info.fullscreen = true;
        }
    }
    return info;
}

int main(int argc, char* argv[]) {

    StartInfo info = parseArgs(argc, argv);

    int status;

    status = SDL_Init(SDL_INIT_EVERYTHING);
    if (status) {
        SDLFatal("Init failed");
    }

    SDL_DisplayMode displayMode;
    status = SDL_GetCurrentDisplayMode(info.displayIndex == -1 ? 0 : info.displayIndex, &displayMode);
    if (status) {
        SDLFatal("Couldn't get display size for display %d", info.displayIndex);
    }
    if (info.width) displayMode.w = info.width * info.gameScale;
    if (info.height) displayMode.h = info.height * info.gameScale;

    if (!displayMode.w || !displayMode.h) {
        Fatal("Invalid display resolution %dx%d", displayMode.w, displayMode.h);
    }

    Window window(displayMode.w, displayMode.h, info.fullscreen);
    window.Open();

    Game game(&window);
    game.Scale(displayMode.h / (160.0f + 40));
    game.LoadAssets();

    game.SwitchScene();

    SDL_Event event;
    bool running = true;

    SDL_GL_SetSwapInterval(1);

    auto last = std::chrono::high_resolution_clock::now();

    while (running) {
        Input::Update();
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        Input::SetButtonState(InputButton::Escape, event.key.state != 0);
                        break;
                    case SDLK_SPACE:
                        Input::SetButtonState(InputButton::Space, event.key.state != 0);
                        break;
                    case SDLK_LEFT:
                        Input::SetButtonState(InputButton::Left, event.key.state != 0);
                        break;
                    case SDLK_RIGHT:
                        Input::SetButtonState(InputButton::Right, event.key.state != 0);
                        break;
                    case SDLK_UP:
                        Input::SetButtonState(InputButton::Up, event.key.state != 0);
                        break;
                    case SDLK_DOWN:
                        Input::SetButtonState(InputButton::Down, event.key.state != 0);
                        break;
                    case SDLK_0:
                    case SDLK_1:
                    case SDLK_2:
                    case SDLK_3:
                    case SDLK_4:
                    case SDLK_5:
                    case SDLK_6:
                    case SDLK_7:
                    case SDLK_8:
                    case SDLK_9:
                        Input::SetButtonState((InputButton)((int)InputButton::Number0 + event.key.keysym.sym - SDLK_0), event.key.state != 0);
                        break;
                }
                break;
            case SDL_MOUSEMOTION:
                game.MouseMove(event.motion);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                game.MouseButton(event.button);
                break;
            case SDL_WINDOWEVENT: {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) { 
                    game.Scale(event.window.data2 / (160.0f + 40));
                }
                break;
            }
            case SDL_QUIT:
                running = false;
                break;
            default:
                //printf("Unhandled event %d\n", event.type);
                break;
            }
        }
        auto now = std::chrono::high_resolution_clock::now();
        auto delta = now - last;
        game.Update(delta.count() / 1000000000.0f);
        game.Render();
        last = now;
    }

    return 0;
}