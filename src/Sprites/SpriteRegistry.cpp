#include "SpriteRegistry.h"
#include "../AssetLoader.h"

void RegisterSprites(SpriteAtlas& atlas, SpriteRegistry& registry) {
    registry.abilityButton = LoadSprite(atlas, "resources/ability.gif");
    registry.abilityIcon = LoadSprite(atlas, "resources/ability-icons.gif");
    registry.number8x8 = LoadSprite(atlas, "resources/numbers8x8.gif");
    registry.cursors = LoadSprite(atlas, "resources/cursors.gif");
    registry.pony = LoadSprite(atlas, "resources/pony.gif");
}
