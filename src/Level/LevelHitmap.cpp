#include "LevelHitmap.h"

LevelHitmap::LevelHitmap(int width, int height) {
    int neededBytes = (width * height);
    this->width = width;
    this->height = height;

    collisions = new uint8_t[neededBytes];
}

LevelHitmap::~LevelHitmap() {
    delete[] collisions;
}

bool LevelHitmap::GetCollision(int x, int y, CollisionType type) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    int index = x + y * width;
    return (collisions[index] & (uint8_t)type) != 0;
}

void LevelHitmap::SetCollision(int x, int y, CollisionType type) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    int index = x + y * width;
    collisions[index] = (uint8_t)(type);
}
