#pragma once
#include "../SDL.h"
#include <vector>
#include "binpack2d.hpp"

typedef int64_t SpriteID;

struct Sprite;

struct SpriteAtlasEntryPacked {
    SDL_Rect* frames;
    int frameCount;
};

struct SpriteAtlasEntry {
    SpriteID spriteId;
    int frameIndex;
    int frameCount;
    Uint32* pixelData;
};

class SpriteAtlas {
public: 
    SpriteAtlas() {}
    SpriteAtlas(const SDL_Renderer* renderer, int size);
    void Build();
    SDL_Texture* GetTexture() const { return texture; }
    SpriteID AddSprite(const std::vector<Uint32*>& frames, int w, int h);
    const SDL_Rect* SpriteFrame(const Sprite& sprite) const;
    const SpriteAtlasEntryPacked GetSpriteInfo(SpriteID id) const;

private:
    std::vector<SpriteAtlasEntryPacked> sprites;
    SpriteID currentSpriteID = 0;
    BinPack2D::Canvas<SpriteAtlasEntry>* packCanvas;
    SDL_Texture* texture;
    SDL_Rect size;
    bool isBuilt = false;
};

struct Sprite {
    float frameTime;
    int currentFrame;
    int frameCount;
    float currentTime;
    SpriteID id;
    int width;
    int height;
    void Advance(float sec);
    Sprite() {}
    Sprite(const SpriteAtlas& atlas, SpriteID id, int fps = 0) {
        this->frameTime = 1.0f / fps;
        this->id = id;
        this->currentFrame = 0;
        this->currentTime = 0;
        SpriteAtlasEntryPacked sp = atlas.GetSpriteInfo(id);
        this->frameCount = sp.frameCount;
        this->width = sp.frames[0].w;
        this->height = sp.frames[0].h;
    }
};