#include "Game.h"
#include "AssetLoader.h"
#include "Scenes/LevelScene.h"

Game::Game(Window* window) {
    renderer = window->Renderer();
    this->window = window;
    spriteAtlas = SpriteAtlas(renderer, 512);
}

void Game::LoadAssets() {
    RegisterSprites(spriteAtlas, spriteRegistry);
    spriteAtlas.Build();

    cursor = Sprite(spriteAtlas, spriteRegistry.cursors);

    SDL_ShowCursor(0);
}

void Game::Render() {
    SDL_Renderer* r = (SDL_Renderer*)renderer;
    SDL_Texture* t = (SDL_Texture*)spriteAtlas.GetTexture();
    SDL_RenderClear(r);

    if (currentScene) {
        currentScene->Draw(this);
    }

    DrawSprite(cursor, mouseX / scale - 7, mouseY / scale - 7);

    SDL_RenderPresent(r);
}

void Game::Update(float delta) {
    window->Update();
    SetCursor(0);
    
    if (currentScene) {
        currentScene->Update(this, delta);
    }
}

void Game::DrawSprite(const Sprite& sprite, int x, int y, const SDL_RendererFlip flip) const {
    const SDL_Rect* src = spriteAtlas.SpriteFrame(sprite);
    const SDL_Point center = { 0, 0 };
    SDL_Rect dst = { x, y, src->w, src->h };
    SDL_RenderCopyEx((SDL_Renderer*)renderer, (SDL_Texture*)spriteAtlas.GetTexture(), src, &dst, 0, &center, flip);
}

void Game::DrawSprite(const Sprite& sprite, float x, float y, const SDL_RendererFlip flip) const {
    const SDL_Rect* src = spriteAtlas.SpriteFrame(sprite);
    SDL_FRect fSrc = { (float)src->x, (float)src->y, (float)src->w, (float)src->h };
    const SDL_FPoint center = { 0, 0 };
    SDL_FRect dst = { x, y, (float)src->w, (float)src->h };
    SDL_RenderCopyExF((SDL_Renderer*)renderer, (SDL_Texture*)spriteAtlas.GetTexture(), src, &dst, 0, &center, flip);
}

void Game::DrawTexture(const SDL_Texture* texture, int x, int y, int w, int h) const {
    const SDL_Rect src = { 0, 0, w, h };
    SDL_Rect dst = { x, y, src.w, src.h };
    SDL_RenderCopy((SDL_Renderer*)renderer, (SDL_Texture*)texture, &src, &dst);
}

void Game::MouseMove(const SDL_MouseMotionEvent& event) {
    mouseX = event.x;
    mouseY = event.y;
    if (currentScene) {
        currentScene->MouseMove((int)(event.x / scale), (int)(event.y / scale));
    }
}

void Game::MouseButton(const SDL_MouseButtonEvent& button) {
    if (currentScene) {
        currentScene->MouseButton(button.which, button.state);
    }
}

void Game::SwitchScene() {
    if (currentScene) {
        delete currentScene;
    }
    currentScene = new LevelScene(this);
}
