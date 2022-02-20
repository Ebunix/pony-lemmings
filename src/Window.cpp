#include "Window.h"

Window::Window(int width, int height, bool fullscreen) {
    
    this->width = width;
    this->height = height;
    Uint32 flags = SDL_WINDOW_RESIZABLE;
    if (fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    printf("Creating window of size %dx%d\n", width, height);
    SDL_CreateWindowAndRenderer(width, height, flags, &window, &renderer);
    if (!window) {
        SDLFatal("Failed to create window");
    }
    if (!renderer) {
        SDLFatal("Failed to create renderer");
    }

    SDL_SetWindowTitle(window, "Ponies");
}

Window::~Window() {
    Close();
    SDL_DestroyWindow(window);
}

void Window::Open() {
    SDL_ShowWindow(window);
}

void Window::Close() {
    SDL_HideWindow(window);
}