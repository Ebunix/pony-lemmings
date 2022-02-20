#pragma once
#include "UIElement.h"

class UIButton : public UIElement {
public:
    UIButton(const Sprite& sprite) : UIElement(sprite) {}
    UIButton(const Sprite& sprite, int x, int y) : UIElement(sprite) {
        this->x = x;
        this->y = y;
    }

    virtual void MouseEnter() override {
        sprite.currentFrame = 1;
    }
    virtual void MouseExit() override {
        sprite.currentFrame = 0;
    }
    virtual void HoldStart(int button) override {
        sprite.currentFrame = 2;
    }
    virtual void HoldEnd(int button) override {
        sprite.currentFrame = mouseOver ? 1 : 0;
    }
    virtual bool Click(int button) override {
        return true;
    }

private:
};
