#pragma once  
#include "LevelHitmap.h"
#include "../SDL.h"
#include <vector>
#include "../Mechanics/Pony.h"

class Game;

class Level {
public:
    Level(int width, int height, const SDL_Renderer* renderer);
    ~Level() {
        SDL_DestroyTexture((SDL_Texture*)texture);
    }

    void PlaceSprite(int x, int y, Uint32* pixels, int w, int h);
    void Upload();
    void Update(const Game* game, float delta);
    void Draw(const Game* game);
    void AddPony(const Sprite& sprite, int x, int y);
    std::vector<Pony*>& Ponies() { return ponies; }

    const LevelHitmap* GetHitmap() const { return hitmap; }

private:
    int width, height;
    int x, y;
    LevelHitmap* hitmap;
    Uint32* bitmap;
    const SDL_Texture* texture;
    std::vector<Pony*> ponies;
};
