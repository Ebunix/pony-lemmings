#include "Pony.h"
#include "../Game.h"
#include "../Level/Level.h"

#define MAX_ELEVATION_STEP 4

Pony::Pony(const Sprite& sprite, Level* level, int x, int y) {
    this->level = level;
    this->sprite = sprite;
    this->x = x;
    this->y = y;
}

float Pony::MovementSpeedMultiplier() {
    if (isClimber && isClimbing)
        return 0.5f;

    if (isFloater && isFallingFar)
        return 0.5f;

    if (movementMode == MovementMode::Builder) 
        return 0.08f;

    return (grounded ? 1 : 1.2f);
}

void Pony::Update(float delta) {
    if (dead) return;

    sprite.Advance(delta);

    if (isBomber) {
        bomberTimer -= delta;
        if (bomberTimer <= 0) {
            Die();
            level->BombCircle(x, y - sprite.height / 2, 12);
            return;
        }
    }

    if (!grounded) {
        fallTime += delta;
        if (fallTime > 0.3f) {
            isFallingFar = true;
        }
        if (fallTime > 0.8f) {
            isFallingDeadly = !isFloater;
        }
    }

    float moveMulti = MovementSpeedMultiplier();
    movementCounter += delta * moveMulti;
    if (movementCounter < moveEvery) {
        return;
    }
    movementCounter -= moveEvery;

    int newX = x, newY = y;
    DoMovement(&newX, &newY);
    x = newX;
    y = newY;
}

void Pony::Draw(const Game* game, int xOff, int yOff) {
    if (dead) return;

    if (isBomber) {
        int timer = (int)(bomberTimer + 1);
        FC_DrawAlign(game->FontSmall(), game->MainWindow()->Renderer(), x, y - 18, FC_ALIGN_CENTER, "%d", timer);
    }
    game->DrawSprite(sprite, xOff + x - sprite.width / 2, yOff + y - sprite.height, movingRight ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void Pony::DoMovement(int* newX, int* newY) {
    bool wasGrounded = grounded;
    int groundDistance = CheckGroundBelow();
    if (groundDistance <= MAX_ELEVATION_STEP) {
        *newY = y + groundDistance;
        grounded = true;
    }
    else {
        *newY = y + 3;
        grounded = false;
    }

    if (!grounded || movementMode == MovementMode::Blocker) {
        return;
    }

    if (!wasGrounded && grounded) {
        if (isFallingDeadly) {
            Die();
            return;
        }
        fallTime = 0;
        isFallingFar = false;
        isFallingDeadly = false;
    }

    switch (movementMode) {
        case MovementMode::Bomber:
        case MovementMode::Walker:
        case MovementMode::Floater:
        case MovementMode::Climber:
            DoMovement_Walk(newX, newY);
            break;
        case MovementMode::Builder:
            DoMovement_Build(newX, newY);
            break;
    }

}

bool Pony::DoMovement_Basic(int* newX, int* newY) {
    int x = *newX, y = *newY;
    int collisions = 0;
    int firstCollision = 0;
    for (int i = 5; i > 0; i--) {
        if (level->GetHitmap()->GetCollision(x + (movingRight ? 1 : -1), y - i)) {
            if (!firstCollision) {
                firstCollision = i;
            }
            collisions++;
        }
    }
    bool hitWall = collisions >= 3;
    if (hitWall) {
        return true;
    }

    *newX = x + (movingRight ? 1 : -1);
    *newY = y - firstCollision;
    return false;
}

void Pony::DoMovement_Walk(int* newX, int* newY) {
    int x = *newX, y = *newY;
    if (DoMovement_Basic(newX, newY)) {
        if (isClimber) {
            isClimbing = false;
            *newY = y - 1;
            for (int i = 1; i < 10; i++) {
                if (level->GetHitmap()->GetCollision(x, y - i)) {
                    movingRight = !movingRight;
                    return;
                }
            }
            isClimbing = true;
        }
        else {
            movingRight = !movingRight;
        }
    }
}

void Pony::DoMovement_Build(int* newX, int* newY) {
    int x = *newX, y = *newY;
    int sign = (movingRight ? 1 : -1);
    for (int tx = x + sign; tx != x + sign * 5; tx += sign) {
        if (!level->GetHitmap()->GetCollision(tx, y - 1)) {
            level->SetPixel(tx, y - 1, 0x4184abff, true);
        }
    }
    level->Upload();
    if (DoMovement_Basic(newX, newY)) {
        SetMovementMode(MovementMode::Walker);
        movingRight = !movingRight;
    }
    *newX = x + (*newX - x) * 2;
}

int Pony::CheckGroundBelow() {
    int groundY = 0;
    if (isClimber && isClimbing) {
        if (level->GetHitmap()->GetCollision(x + (movingRight ? 1 : -1), y)) {
            return y;
        }
    }
    int i = 0;
    while (true) {
        if (level->GetHitmap()->GetCollision(x, y + i)) {
            groundY = y + i;
            break;
        }
        i++;
        if (i > 320)
            break;
    }
    return groundY - y;
}

bool Pony::SetMovementMode(MovementMode mode) {
    if (
        (movementMode == MovementMode::Blocker && mode != MovementMode::Bomber) ||
        isClimber && mode == MovementMode::Climber ||
        isFloater && mode == MovementMode::Floater ||
        isBomber && mode == MovementMode::Bomber
    ) {
        return false;
    }
    if (mode == MovementMode::Blocker) {
        for(int tx = x - 2; tx <= x + 2; tx++)
            for(int ty = y; ty > y - 10; ty--)
                level->GetHitmap()->SetCollision(tx, ty, true);
    }
    if (mode == MovementMode::Floater) {
        isFloater = true;
        return true;
    }
    if (mode == MovementMode::Bomber) {
        isBomber = true;
        return true;
    }
    if (mode == MovementMode::Climber) {
        isClimber = true;
        return true;
    }
    movementMode = mode;
    return true;
}