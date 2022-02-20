#pragma once
#include "../Sprites/SpriteAtlas.h"

class Game;

class UIElement {
public:
    int x, y;

    UIElement() {}
    UIElement(const Sprite& sprite) {
        this->sprite = sprite;
    }

    inline const int Width() const { return sprite.width; }
    inline const int Height() const { return sprite.height; }

    virtual void MouseEnter() {}
    virtual void MouseExit() {}
    virtual void HoldStart(int button) {}
    virtual void HoldEnd(int button) {}
    virtual void MouseDown(int button) {}
    virtual void MouseUp(int button) {}
    virtual bool Click(int button) { return false; }

    virtual void Draw(const Game* game);

    void MouseMove(int x, int y);
    bool MouseButton(Uint32 which, Uint8 state);
    inline const bool MouseOver() const { return mouseOver; }

protected:
    bool mouseOver;
    int holding = -1;
    Sprite sprite;

    SDL_Rect GetSize() const { return { x, y, sprite.width, sprite.height }; }
    SDL_Point GetPosition() const { return { x, y }; }
};
