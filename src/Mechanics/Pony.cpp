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
    if (movementMode == MovementMode::Digger) 
        return 0.15f;
    if (movementMode == MovementMode::Basher) 
        return 0.5f;

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
            level->Upload();
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
        case MovementMode::Digger:
            DoMovement_Dig(newX, newY);
            break;
        case MovementMode::Basher:
            DoMovement_Bash(newX, newY);
            break;
    }

}

bool Pony::DoMovement_Basic(int* newX, int* newY) {
    int x = *newX, y = *newY;
    int collisions = 0;
    int firstCollision = 0;
    for (int i = 5; i > 0; i--) {
        if (CollisionTest(level->GetHitmap()->GetCollision(x + (movingRight ? 1 : -1), y - i), CollisionType::Any)) {
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
                if (CollisionTest(level->GetHitmap()->GetCollision(x, y - i), CollisionType::Any)) {
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
        if (!CollisionTest(level->GetHitmap()->GetCollision(tx, y - 1), CollisionType::Any)) {
            level->SetPixel(tx, y - 1, 0x4184abff, CollisionType::Wall);
        }
    }
    level->Upload();
    if (DoMovement_Basic(newX, newY)) {
        SetMovementMode(MovementMode::Walker);
        movingRight = !movingRight;
    }
    *newX = x + (*newX - x) * 2;
}

void Pony::DoMovement_Dig(int* newX, int* newY) {
    int x = *newX, y = *newY;
    
    bool didCollide = false;
    CollisionType collision = level->GetHitmap()->GetCollision(x, y);
    if (CollisionTest(collision, CollisionType::Any)) {
        didCollide = true;
        if (CollisionTest(collision, CollisionType::Permanent)) {
            SetMovementMode(MovementMode::Walker);
            return;
        }
    }
    for (int tx = -6; tx <= 6; tx++) {
        for (int ty = 0; ty < 4; ty++) {
            level->SetPixel(x + tx, y - ty, 0, CollisionType::None);
        }
    }
    level->Upload();

    collision = level->GetHitmap()->GetCollision(x, y + 1);
    if (!CollisionTest(collision, CollisionType::Any)) {
        SetMovementMode(MovementMode::Walker);
    }

    *newY = y + 1;
}

void Pony::DoMovement_Bash(int* newX, int* newY) {
    int x = *newX, y = *newY;
    
    bool didCollide = false;
    for (int tx = 0; tx < 5; tx++) {
        CollisionType collision = level->GetHitmap()->GetCollision(x + tx * (movingRight ? 1 : -1), y);
        if (CollisionTest(collision, CollisionType::Any)) {
            didCollide = true;
            if (CollisionTest(collision, CollisionType::Permanent)) {
                SetMovementMode(MovementMode::Walker);
                return;
            }
        }
    }
    
    int bashX = (int)(sin((bashAngle + (movingRight ? -90 : 90)) * 0.0174533f) * 5); // DEG2RAD
    int bashY = (int)(cos((bashAngle + (movingRight ? -90 : 90)) * 0.0174533f) * 5); // DEG2RAD
    level->BashCircle(x + bashX + (movingRight ? 3 : -3), y + bashY - 6, 3, y);
    level->Upload();

    bashAngle += 360.0f / 12 * (movingRight ? -1 : 1);
    if (bashAngle <= -360 || bashAngle >= 360) {
        level->BashCircle(x + (movingRight ? 3 : -3), y - 6, 3, y);

        bashAngle = 0;
        *newX += movingRight ? 3 : -3;

        bool didCollide = false;
        for (int tx = 0; tx < 10; tx++) {
            if (CollisionTest(level->GetHitmap()->GetCollision(*newX + (movingRight ? 1 : -1) * tx, y - 2), CollisionType::Any)) {
                didCollide = true;
            }
            if (CollisionTest(level->GetHitmap()->GetCollision(*newX + (movingRight ? 1 : -1) * tx, y - 8), CollisionType::Any)) {
                didCollide = true;
            }
        }
        if (!didCollide) {
            SetMovementMode(MovementMode::Walker);
        }
    }
}

int Pony::CheckGroundBelow() {
    int groundY = 0;
    if (isClimber && isClimbing) {
        if (CollisionTest(level->GetHitmap()->GetCollision(x + (movingRight ? 1 : -1), y), CollisionType::Any)) {
            return y;
        }
    }
    int i = 0;
    while (true) {
        if (CollisionTest(level->GetHitmap()->GetCollision(x, y + i), CollisionType::Any)) {
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
                level->GetHitmap()->SetCollision(tx, ty, CollisionType::Blocker);
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