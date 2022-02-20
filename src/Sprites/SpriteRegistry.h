#pragma once
#include "SpriteAtlas.h"

struct SpriteRegistry {
    SpriteID pony;
    SpriteID abilityButton;
    SpriteID abilityIcon;
    SpriteID number8x8;
    SpriteID cursors;
};

void RegisterSprites(SpriteAtlas& atlas, SpriteRegistry& registry);
