#include "LevelHitmap.h"

LevelHitmap::LevelHitmap(int width, int height) {
    int neededBytes = (width * height) / 8 + 1;
    this->width = width;
    this->height = height;

    collisions = new uint8_t[neededBytes];
}

LevelHitmap::~LevelHitmap() {
    delete[] collisions;
}

bool LevelHitmap::GetCollision(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    int index = x + y * width;
    int i = index / 8;
    int m = index % 8;
    return (collisions[i] & (1 << m)) != 0;
}

void LevelHitmap::SetCollision(int x, int y, bool collide) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    int index = x + y * width;
    int i = index / 8;
    int m = index % 8;
    if (collide)
        collisions[i] |= (1 << m);
    else
        collisions[i] &= ~(1 << m);
}
