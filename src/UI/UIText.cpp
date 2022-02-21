#include "UIText.h"
#include "../Game.h"

void UIText::Draw(const Game* game) {
    FC_Draw(game->Font(), game->MainWindow()->Renderer(), (float)x, (float)y, text);
}