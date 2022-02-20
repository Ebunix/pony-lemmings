#include "Pony.h"
#include "../Game.h"

Pony::Pony(const Sprite& sprite, LevelHitmap* hitmap, int x, int y) {
    this->hitmap = hitmap;
    this->sprite = sprite;
    this->x = x;
    this->y = y;
}

void Pony::Update(float delta) {
    sprite.Advance(delta);
    movementCounter += delta;
    if (movementCounter < moveEvery) {
        return;
    }
    movementCounter -= moveEvery;

    int newX = x, newY = y;
    DoMovement(delta, &newX, &newY);
    x = newX;
    y = newY;
}

void Pony::Draw(const Game* game, int xOff, int yOff) {
    game->DrawSprite(sprite, xOff + x - sprite.width / 2, yOff + y - sprite.height, movingRight ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void Pony::DoMovement(float delta, int* newX, int* newY) {
    grounded = CheckGroundBelow(delta, newY);
    if (!grounded && lastGrounded++ >= 3) {
        return;
    }
    if (grounded) {
        lastGrounded = 0;
    }

    switch (movementMode) {
        case MovementMode::Walker:
            DoMovement_Walker(delta, newX, newY);
    }

}

void Pony::DoMovement_Walker(float delta, int* newX, int* newY) {
    if (movingRight) {
        for (int i = 0; i < 4; i++) {
            if (hitmap->GetCollision(x + 1, y - (3 - i))) {
                if (i == 0) {
                    movingRight = !movingRight;
                }
                else {
                    *newY = y - (3 - i) - 1;
                }
                break;
            }
            if (movingRight)
                *newX = x + 1;
        }
    }
    else {
        for (int i = 0; i < 4; i++) {
            if (hitmap->GetCollision(x - 1, y - (3 - i))) {
                if (i == 0) {
                    movingRight = !movingRight;
                }
                else {
                    *newY = y - (3 - i) - 1;
                }
                break;
            }
        }
        if (!movingRight)
            *newX = x - 1;
    }
}

bool Pony::CheckGroundBelow(float delta, int* groundY) {
    int start = y;
    int end = start + 2;
    for (int i = start; i <= end; i++) {
        if (hitmap->GetCollision(x, i)) {
            *groundY = i;
            return true;
        }
    }
    *groundY = end;
    return false;
}