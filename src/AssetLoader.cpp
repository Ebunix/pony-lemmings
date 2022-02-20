#include "AssetLoader.h"
#include "Sprites/gifdec.h"

SpriteID LoadAnimatedSprite(SpriteAtlas& atlas, const char* file) {
    gd_GIF* gif = gd_open_gif(file);
    if (!gif) {
        Fatal("Can't load asset %s", file);
    }

    int frameCount = 0;
    uint8_t renderBuffer[gif->width * gif->height * 3];
    std::vector<Uint32*> frames;

    while (gd_get_frame(gif)) {
        gd_render_frame(gif, renderBuffer);
        Uint32* pixels = new Uint32[gif->width * gif->height];

        for (int i = 0; i < gif->width * gif->height; i++) {
            uint32_t r = renderBuffer[i * 3 + 0];
            uint32_t g = renderBuffer[i * 3 + 1];
            uint32_t b = renderBuffer[i * 3 + 2];
            pixels[i] = r | (g << 8) | (b << 16) | ((gd_is_bgcolor(gif, renderBuffer + i * 3) ? 0 : 0xff) << 24);
        }

        frames.push_back(pixels);
    }
    SpriteID id = atlas.AddSprite(frames, gif->width, gif->height);
    gd_close_gif(gif);
    return id;
}

Uint32* LoadSpriteRaw(const char* file, int* width, int* height) {
    gd_GIF* gif = gd_open_gif(file);
    if (!gif) {
        Fatal("Can't load asset %s", file);
    }

    *width = gif->width;
    *height = gif->height;
    uint8_t renderBuffer[gif->width * gif->height * 3];
    Uint32* pixels = new Uint32[gif->width * gif->height];

    if (gd_get_frame(gif)) {
        gd_render_frame(gif, renderBuffer);
        for (int i = 0; i < gif->width * gif->height; i++) {
            uint32_t r = renderBuffer[i * 3 + 0];
            uint32_t g = renderBuffer[i * 3 + 1];
            uint32_t b = renderBuffer[i * 3 + 2];
            pixels[i] = r | (g << 8) | (b << 16) | ((gd_is_bgcolor(gif, renderBuffer + i * 3) ? 0 : 0xff) << 24);
        }
    }
    gd_close_gif(gif);

    return pixels;
}

Uint32* LoadSingleFrameFromGif(const char* file, int frame, int* width, int* height) {
    gd_GIF* gif = gd_open_gif(file);
    if (!gif) {
        Fatal("Can't load asset %s", file);
    }

    *width = gif->width;
    *height = gif->height;
    uint8_t renderBuffer[gif->width * gif->height * 3];
    Uint32* pixels = new Uint32[gif->width * gif->height];

    int currentFrame = 0;
    while (gd_get_frame(gif)) {
        gd_render_frame(gif, renderBuffer);
        if (currentFrame++ == frame) {
            for (int i = 0; i < gif->width * gif->height; i++) {
                uint32_t r = renderBuffer[i * 3 + 0];
                uint32_t g = renderBuffer[i * 3 + 1];
                uint32_t b = renderBuffer[i * 3 + 2];
                pixels[i] = r | (g << 8) | (b << 16) | ((gd_is_bgcolor(gif, renderBuffer + i * 3) ? 0 : 0xff) << 24);
            }
            break;
        }
    }
    gd_close_gif(gif);

    return pixels;
}

SpriteID LoadSprite(SpriteAtlas& atlas, const char* file) {
    const char* ext = strrchr(file, '.');
    if (strcmp(ext, ".gif") == 0) {
        return LoadAnimatedSprite(atlas, file);
    }

    return -1;
}
