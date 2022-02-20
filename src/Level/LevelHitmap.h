#pragma once
#include <stdint.h>

class LevelHitmap {
public:
    LevelHitmap(int width, int height);
    ~LevelHitmap();

    bool GetCollision(int x, int y) const;
    void SetCollision(int x, int y, bool collide);
private:
    uint8_t* collisions;
    int width;
    int height;
};
