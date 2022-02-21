#include "LevelScene.h"
#include "../AssetLoader.h"
#include "../Utils.h"
#include "../UI/UIText.h"

LevelScene::LevelScene(const Game* game) : GameScene(game) {
    LoadLevel("resources/level001.gif", game->MainWindow()->Renderer());
    for (int i = 0; i < 8; i++) {
        AddUIElement(new UIAbilityButton(game->Atlas(), game->Registry(), &selectedAbility, i));
    }
    labelText = new UIText(game->Font(), 3, 160);
    level->AddPony(Sprite(game->Atlas(), game->Registry().pony, 10), 240, 30);
    level->AddPony(Sprite(game->Atlas(), game->Registry().pony, 10), 230, 30);
}

void LevelScene::LoadLevel(const char* file, SDL_Renderer* renderer) {
    int width, height;

    Uint32* bitmap = LoadSpriteRaw("resources/level001.gif", &width, &height);
    level = new Level(width, height, renderer);
    level->PlaceSprite(0, 0, bitmap, width, height);
    delete[] bitmap;

    Uint32* block = LoadSpriteRaw("resources/block.gif", &width, &height);
    for (int i = 0; i < 5; i++)
        level->PlaceSprite(140 + 24 * i, 64, block, 24, 24);
    for (int i = 1; i < 3; i++)
        level->PlaceSprite(140, 64 - 24 * i, block, 24, 24);
    delete[] block;

    level->Upload();
}

void LevelScene::Draw(const Game* game) {
    level->Draw(game);
    if (targetPony) {
        labelText->Draw(game);
    }
    GameScene::Draw(game);
}

void LevelScene::Update(Game* game, float delta) {
    GameScene::Update(game, delta);
    level->Update(game, delta);

    targetPony = 0;
    int hoveredPonies = 0;
    SDL_Point mousePos = game->MousePos();
    SDL_Point levelMousePos = level->AdjustMousePos(mousePos);
    for (auto p : level->Ponies()) {
        if (p->dead) continue;

        SDL_Rect rect = p->Area();
        if (IsInside(rect, levelMousePos)) {
            if (!targetPony) {
                targetPony = p;
                labelText->SetText(MovementModeText(targetPony->GetMovementMode()));
            }
            hoveredPonies++;
            game->PushDebugText("Pony under cursor is %s\n", MovementModeText(p->GetMovementMode()));
            game->SetCursor(1);
        }
    }

    game->PushDebugText("Ponies on screen: %d\n", level->Ponies().size());
    game->PushDebugText("Ponies under cursor: %d\n", hoveredPonies);
}

bool LevelScene::MouseButton(Game* game, Uint32 which, Uint8 state) {
    if (GameScene::MouseButton(game, which, state)) {
        return true;
    }

    SDL_Point mousePos = game->MousePos();
    SDL_Point levelMousePos = level->AdjustMousePos(mousePos);
    for (auto p : level->Ponies()) {
        if (p->dead) continue;

        SDL_Rect rect = p->Area();
        if (IsInside(rect, levelMousePos)) {
            p->SetMovementMode(selectedAbility);
            return true;
        }
    }

    return false;
}