#pragma once
#include "../Sprites/SpriteRegistry.h"
#include "MovementMode.h"

class Game;
class Level;

class Pony {
public:
    int x, y;
    bool dead = false;

    Pony(const Sprite& sprite, Level* level, int x, int y);
    void Update(float delta);
    void Draw(const Game* game, int xOff, int yOff);
    SDL_Rect Area() const { return { x - sprite.width / 2, y - sprite.height, sprite.width, sprite.height }; }
    inline MovementMode GetMovementMode() { return movementMode; }
    bool SetMovementMode(MovementMode mode);
    void Die() { dead = true; }

private:
    Level* level;
    bool movingRight = false;
    bool grounded = false;
    float moveEvery = 1.0f / 15;
    float movementCounter = 0;
    float bomberTimer = 5;
    bool isClimbing = false;

    bool isFloater = false;
    bool isBomber = false;
    bool isClimber = false;

    float fallTime = 0;
    bool isFallingFar = false;
    bool isFallingDeadly = false;

    int stairsLeft = 0;

    Sprite sprite;
    MovementMode movementMode = MovementMode::Walker;

    int CheckGroundBelow();
    void DoMovement(int* newX, int* newY);

    inline float MovementSpeedMultiplier();

    bool DoMovement_Basic(int* newX, int* newY);
    void DoMovement_Walk(int* newX, int* newY);
    void DoMovement_Build(int* newX, int* newY);
};
