#pragma once 

#include "UIElement.h"
#include <SDL_FontCache.h>

class UIText : public UIElement {
public:
    UIText(FC_Font* font) : UIElement() {
        this->font = font;
    }
    UIText(FC_Font* font, int x, int y) : UIElement() {
        this->font = font;
        this->x = x;
        this->y = y;
    }

    virtual void Draw(const Game* game) override;
    inline void SetText(const char* str) { text = str; }

private:
    FC_Font* font;
    const char* text;
};
