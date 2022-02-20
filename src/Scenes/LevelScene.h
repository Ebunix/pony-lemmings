#pragma once

#include "GameScene.h"
#include "../UI/UIAbilityButton.h"

class LevelScene : public GameScene {
public:
    LevelScene(const Game* game);
    void LoadLevel(const char* file, const SDL_Renderer* renderer);

    virtual void Draw(const Game* game) override;
    virtual void Update(Game* game, float delta) override;

private:
    Level* level;
};
