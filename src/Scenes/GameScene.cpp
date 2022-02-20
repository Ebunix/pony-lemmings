#include "GameScene.h"

void GameScene::Draw(const Game* game) {
    for (auto ui : uiElements) {
        ui->Draw(game);
    }
}

void GameScene::MouseMove(int x, int y) {
    for (auto elem : uiElements) {
        elem->MouseMove(x, y);
    }
}

void GameScene::MouseButton(Uint32 which, Uint8 state) {
    static UIElement* lastClickTarget = 0;
    for (auto elem : uiElements) {
        if (elem->MouseOver()) {
            if (elem->MouseButton(which, state)) {
                break;
            }
        }
    }
}
