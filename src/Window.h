#pragma once
#include "SDL.h"

class Window {
public:
    Window(int width, int height, bool fullscreen);
    ~Window();

    inline SDL_Renderer* Renderer() const { return renderer; }

    inline const float Width() const { return width; }
    inline const float Height() const { return height; }

    inline const void Update() {
        SDL_GetWindowSize(window, &width, &height);
    }

    void Open();
    void Close();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    int width, height;

};

