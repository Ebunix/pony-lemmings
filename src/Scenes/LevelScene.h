#pragma once

#include "GameScene.h"
#include "../UI/UIAbilityButton.h"
#include "../UI/UIText.h"

class LevelScene : public GameScene {
public:
    LevelScene(const Game* game);
    void LoadLevel(const char* file, SDL_Renderer* renderer);

    virtual void Draw(const Game* game) override;
    virtual void Update(Game* game, float delta) override;

    virtual bool MouseButton(Game* game, Uint32 which, Uint8 state) override;

private:
    Level* level;
    MovementMode selectedAbility = MovementMode::Walker;
    UIText* labelText;
    Pony* targetPony;
};
