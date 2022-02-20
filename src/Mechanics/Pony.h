#pragma once
#include "../Level/LevelHitmap.h"
#include "../Sprites/SpriteRegistry.h"
#include "MovementMode.h"

class Game;

class Pony {
public:
    int x, y;

    Pony(const Sprite& sprite, LevelHitmap* level, int x, int y);
    void Update(float delta);
    void Draw(const Game* game, int xOff, int yOff);
    SDL_Rect Area() const { return { x - sprite.width / 2, y - sprite.height, sprite.width, sprite.height }; }

private:
    LevelHitmap* hitmap;
    bool movingRight = false;
    bool grounded = false;
    int lastGrounded = 0;
    float moveEvery = 1.0f / 20;
    float movementCounter = 0;
    Sprite sprite;
    MovementMode movementMode = MovementMode::Walker;

    bool CheckGroundBelow(float delta, int* groundY);
    void DoMovement(float delta, int* newX, int* newY);

    void DoMovement_Walker(float delta, int* newX, int* newY);
};
