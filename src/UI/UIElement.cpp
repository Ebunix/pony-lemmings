#include "UIElement.h"
#include "../Utils.h"
#include "../Game.h"

void UIElement::MouseMove(int x, int y) {
    SDL_Rect rect = GetSize();
    SDL_Point pt = { x, y };
    bool hover = IsInside(rect, pt);
    if (!mouseOver && hover) {
        MouseEnter();
        mouseOver = true;
    }
    else if (mouseOver && !hover) {
        MouseExit();
        holding = -1;
        mouseOver = false;
    }
}

bool UIElement::MouseButton(Uint32 which, Uint8 state) {
    if (state) {
        MouseDown(which);
        HoldStart(which);
        holding = which;
        return true;
    }
    else {
        MouseUp(which);
        HoldEnd(which);
        if (holding == which) {
            holding = -1;
            return Click(which);
        }
    }
    return false;
}

void UIElement::Draw(const Game* game) {
    game->DrawSprite(sprite, x, y);
}