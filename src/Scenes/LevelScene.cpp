#include "LevelScene.h"
#include "../AssetLoader.h"
#include "../Utils.h"

LevelScene::LevelScene(const Game* game) : GameScene(game) {
    LoadLevel("resources/level001.gif", game->MainWindow()->Renderer());
    for (int i = 0; i < 8; i++) {
        AddUIElement(new UIAbilityButton(game->Atlas(), game->Registry(), i));
    }
}

void LevelScene::LoadLevel(const char* file, const SDL_Renderer* renderer) {
    int width, height;
    Uint32* bitmap = LoadSpriteRaw("resources/level001.gif", &width, &height);
    level = new Level(width, height, renderer);
    level->PlaceSprite(0, 0, bitmap, width, height);
    level->Upload();
    delete[] bitmap;
}

void LevelScene::Draw(const Game* game) {
    level->Draw(game);
    GameScene::Draw(game);
}

float gameTime = 0;
void LevelScene::Update(Game* game, float delta) {
    GameScene::Update(game, delta);
    level->Update(game, delta);
    gameTime += delta;
    if (gameTime > 1.f) {
        gameTime -= 1.f;
        level->AddPony(Sprite(game->Atlas(), game->Registry().pony, 30), 210, 30);
    }

    SDL_Point mouse = game->MousePos();
    for (auto p : level->Ponies()) {
        SDL_Rect rect = p->Area();
        if (IsInside(rect, mouse)) {
            game->SetCursor(1);
        }
    }
}
