#pragma once
#include "UIButton.h"
#include "../Sprites/SpriteRegistry.h"

class UIAbilityButton : public UIButton {
public: 
    UIAbilityButton(const SpriteAtlas& atlas, const SpriteRegistry& registry, int index);
    ~UIAbilityButton();

    virtual bool Click(int button) override;
    virtual void Draw(const Game* game) override;

    inline void Deselect() { isSelected = false; this->sprite.currentFrame = 0; }
    inline void Select() { isSelected = true; this->sprite.currentFrame = 2; }

private:
    bool isSelected = false;
    Sprite number1;
    Sprite number2;
    Sprite icon;
};
