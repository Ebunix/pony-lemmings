#include "UIAbilityButton.h"
#include "../Game.h"

std::list<UIAbilityButton*> abilityButtons;

UIAbilityButton::UIAbilityButton(const SpriteAtlas& atlas, const SpriteRegistry& registry, int index) 
    : UIButton(Sprite(atlas, registry.abilityButton), index * 26, 160) {
        number1 = Sprite(atlas, registry.number8x8);
        number2 = Sprite(atlas, registry.number8x8);
        icon = Sprite(atlas, registry.abilityIcon);
        icon.currentFrame = index + 1;
        abilityButtons.push_back(this);
    }

UIAbilityButton::~UIAbilityButton() {
    abilityButtons.remove(this);
}

bool UIAbilityButton::Click(int button) {
    if (button == 0) {
        for (auto button : abilityButtons) {
            button->Deselect();
        }
        Select();
        return true;
    }
    return false;
}

void UIAbilityButton::Draw(const Game* game) {
    if (isSelected) {
        this->sprite.currentFrame = 2;
    }
    UIButton::Draw(game);
    game->DrawSprite(number1, x + 6, y + 5);
    game->DrawSprite(number2, x + 13, y + 5);
    game->DrawSprite(icon, x + 4, y + 16);
} 