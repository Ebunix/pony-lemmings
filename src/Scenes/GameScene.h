#pragma once

#include "../Game.h"

class GameScene {
public:
    GameScene(const Game* game) {}
    ~GameScene() {
        for (auto elem : uiElements) {
            delete elem;
        }
    }

    virtual void Draw(const Game* game);
    virtual void Update(Game* game, float delta) {}

    virtual void MouseMove(int x, int y);
    virtual void MouseButton(Uint32 which, Uint8 state);

    void AddUIElement(UIElement* elem) { uiElements.push_back(elem); }

private:
    std::vector<UIElement*> uiElements;
};