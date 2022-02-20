#pragma once
#include <SDL2/SDL.h>
#include <unistd.h>

static void _SDLFatal(const char* src, int line, const char* fmt, ...) {
    char buffer[1000];
    va_list va;
    va_start(va, fmt);
    vsprintf(buffer, fmt, va);
    va_end(va);
    printf("%s:%d - Fatal SDL Error: '%s' (%s)\n", src, line, buffer, SDL_GetError());
    exit(-1);
}

static void _Fatal(const char* src, int line, const char* fmt, ...) {
    char buffer[1000];
    va_list va;
    va_start(va, fmt);
    vsprintf(buffer, fmt, va);
    va_end(va);
    printf("%s:%d - Fatal Error: '%s'\n", src, line, buffer);
    exit(-1);
}

#define SDLFatal(msg, ...) _SDLFatal(__FILE__, __LINE__, msg, ##__VA_ARGS__)
#define Fatal(msg, ...) _Fatal(__FILE__, __LINE__, msg, ##__VA_ARGS__)
