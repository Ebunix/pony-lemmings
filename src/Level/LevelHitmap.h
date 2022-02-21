#pragma once
#include <stdint.h>

enum class CollisionType {
    None = 0,
    Wall = 1 << 0,
    Blocker = 1 << 1,
    Permanent = 1 << 2,
    NotBlocker = Wall | Permanent,
    Any = Wall | Blocker | Permanent
};

class LevelHitmap {
public:
    LevelHitmap(int width, int height);
    ~LevelHitmap();

    bool GetCollision(int x, int y, CollisionType type) const;
    void SetCollision(int x, int y, CollisionType type);
private:
    uint8_t* collisions;
    int width;
    int height;
};
