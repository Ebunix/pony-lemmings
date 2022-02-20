#pragma once 
#include "SDL.h"

static bool IsInside(const SDL_Rect& rect, const SDL_Point& point) {
    return rect.x < point.x && rect.x + rect.w >= point.x && rect.y < point.y && rect.y + rect.h >= point.y;
}