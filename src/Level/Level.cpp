#include "Level.h" 
#include "../Game.h"
#include "../Input.h"

Level::Level(int width, int height, const SDL_Renderer* renderer) {
    hitmap = new LevelHitmap(width, height);
    bitmap = new Uint32[width * height];
    this->width = width;
    this->height = height;
    this->texture = SDL_CreateTexture((SDL_Renderer*)renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
}

void Level::PlaceSprite(int x, int y, Uint32* pixels, int w, int h) {
    for (int sy = 0; sy < h; sy++) {
        memcpy(bitmap + (sy + y) * width + x, pixels + sy * w, sizeof(Uint32) * w);
    }
    for (int i = 0; i < w * h; i++) {
        uint8_t alpha = pixels[i] >> 24;
        hitmap->SetCollision(i % width, i / width, alpha > 0x80);
    }
}

void Level::AddPony(const Sprite& sprite, int x, int y) {
    Pony* pony = new Pony(sprite, hitmap, x, y);
    ponies.push_back(pony);
}

void Level::Upload() {
    SDL_Rect texRect = { 0, 0, width, height };
    SDL_UpdateTexture((SDL_Texture*)texture, &texRect, bitmap, width * sizeof(Uint32));
}

void Level::Draw(const Game* game) {
    game->DrawTexture(texture, x, y, width, height);
    for (Pony* pony : ponies) {
        pony->Draw(game, x, y);
    }
}

void Level::Update(const Game* game, float delta) {

    for (Pony* pony : ponies) {
        pony->Update(delta);
    }

    if (Input::GetButton(InputButton::Left)) {
        x += 200 * delta;
    }
    if (Input::GetButton(InputButton::Right)) {
        x -= 200 * delta;
    }
    if (Input::GetButton(InputButton::Up)) {
        y += 200 * delta;
    }
    if (Input::GetButton(InputButton::Down)) {
        y -= 200 * delta;
    }
}

