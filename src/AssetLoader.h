#pragma once
#include "Sprites/SpriteAtlas.h"

SpriteID LoadSprite(SpriteAtlas& atlas, const char* file);
Uint32* LoadSpriteRaw(const char* file, int* width, int* height);
