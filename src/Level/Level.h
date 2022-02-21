#pragma once  
#include "LevelHitmap.h"
#include "../SDL.h"
#include <vector>
#include "../Mechanics/Pony.h"

class Game;

class Level {
public:
    Level(int width, int height, SDL_Renderer* renderer);
    ~Level() {
        SDL_DestroyTexture(texture);
    }

    void PlaceSprite(int x, int y, Uint32* pixels, int w, int h);
    void Upload();
    void Update(const Game* game, float delta);
    void Draw(const Game* game);
    void AddPony(const Sprite& sprite, int x, int y);
    inline SDL_Point AdjustMousePos(const SDL_Point& mouse) const { return { mouse.x - x, mouse.y - y}; }

    void BombCircle(int x, int y, int radius);
    void SetPixel(int x, int y, Uint32 color, bool collision);

    std::vector<Pony*>& Ponies() { return ponies; }

    inline LevelHitmap* GetHitmap() const { return hitmap; }

private:
    int width, height;
    int x, y;
    LevelHitmap* hitmap;
    Uint32* bitmap;
    SDL_Texture* texture;
    std::vector<Pony*> ponies;
};
