#include "SpriteAtlas.h"
#include <stdio.h>

SpriteAtlas::SpriteAtlas(const SDL_Renderer* renderer, int size) {
    this->size.x = 0;
    this->size.y = 0;
    this->size.w = size;
    this->size.h = size;
    texture = SDL_CreateTexture((SDL_Renderer*)renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, size, size);
    if (!texture) {
        SDLFatal("Couldn't create sprite atlas texture");
    }
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    packCanvas = new BinPack2D::Canvas<SpriteAtlasEntry>(size, size);
}

void SpriteAtlas::Build() {

    printf("Building sprite atlas...\n");

    Uint32* data = new Uint32[size.w * size.h];
    memset(data, 0, sizeof(Uint32) * size.w * size.h);

    auto contents = packCanvas->GetContents();
    SpriteID spriteCount = 0;

    for (auto entry : contents) {
        spriteCount = fmax(entry.content.spriteId + 1, spriteCount);
    }

    for (SpriteID i = 0; i < spriteCount; i++) {
        SpriteAtlasEntryPacked sprite = { 0 };
        for (auto entry : contents) {
            if (entry.content.spriteId == i) {
                if (sprite.frameCount == 0) {
                    sprite.frameCount = entry.content.frameCount;
                    sprite.frames = new SDL_Rect[sprite.frameCount];
                }
                sprite.frames[entry.content.frameIndex].x = entry.coord.x;
                sprite.frames[entry.content.frameIndex].y = entry.coord.y;
                sprite.frames[entry.content.frameIndex].w = entry.size.w;
                sprite.frames[entry.content.frameIndex].h = entry.size.h;

                for (int sy = 0; sy < entry.size.h; sy++) {
                    for (int sx = 0; sx < entry.size.w; sx++) {
                        int tx = entry.coord.x + sx;
                        int ty = entry.coord.y + sy;
                        data[tx + ty * size.w] = entry.content.pixelData[sx + sy * entry.size.w];
                    }
                }
            }
        }
        sprites.push_back(sprite);
    }

    SDL_UpdateTexture(texture, &size, data, size.w * 4);

    printf("Building sprite atlas - Done\n");
    isBuilt = true;
}

SpriteID SpriteAtlas::AddSprite(const std::vector<Uint32*>& frames, int w, int h) {

    int count = frames.size();

    for (int i = 0; i < count; i++) {
        SpriteAtlasEntry entry = {
            .spriteId = currentSpriteID,
            .frameIndex = i,
            .frameCount = count,
            .pixelData = frames.at(i)
        };
        BinPack2D::Content<SpriteAtlasEntry> content(entry, BinPack2D::Coord(), BinPack2D::Size(w, h), false);
        bool placed = packCanvas->Place(content);
    }

    return currentSpriteID++;
}

const SDL_Rect* SpriteAtlas::SpriteFrame(const Sprite& sprite) const {
    if (!isBuilt) {
        Fatal("Can't access sprite atlas before it is built!");
    }
    return &sprites.at(sprite.id).frames[sprite.currentFrame];
}

const SpriteAtlasEntryPacked SpriteAtlas::GetSpriteInfo(SpriteID id) const {
    if (!isBuilt) {
        Fatal("Can't access sprite atlas before it is built!");
    }
    return sprites.at(id);
}

void Sprite::Advance(float sec) {
    currentTime += sec;
    while (currentTime > frameTime) {
        currentTime -= frameTime;
        currentFrame = (currentFrame + 1) % frameCount;
    }
}