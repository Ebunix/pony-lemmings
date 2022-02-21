#include "Game.h"
#include "AssetLoader.h"
#include "Scenes/LevelScene.h"

Game::Game(Window* window, float scale) {
    renderer = window->Renderer();
    this->window = window;
    Scale(scale);
    spriteAtlas = SpriteAtlas(renderer, 512);
}

void Game::LoadAssets() {
    fontMain = FC_CreateFont();
    fontSmall = FC_CreateFont();
    FC_LoadFont(fontMain, renderer, "resources/yoster-island-regular.ttf", (int)scale * 2, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
    FC_LoadFont(fontSmall, renderer, "resources/yoster-island-regular.ttf", (int)scale * 1.3f, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
    FC_SetFilterMode(fontSmall, FC_FILTER_NEAREST);


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

    float xs, ys;
    SDL_RenderGetScale(renderer, &xs, &ys);
    SDL_RenderSetScale(renderer, 2, 2);
    FC_Draw(fontMain, renderer, 0, 0, debugBuffer);
    SDL_RenderSetScale(renderer, xs, ys);
    debugBuffer[0] = 0;

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
    SDL_RenderCopyEx(renderer, spriteAtlas.GetTexture(), src, &dst, 0, &center, flip);
}

void Game::DrawSprite(const Sprite& sprite, float x, float y, const SDL_RendererFlip flip) const {
    const SDL_Rect* src = spriteAtlas.SpriteFrame(sprite);
    SDL_FRect fSrc = { (float)src->x, (float)src->y, (float)src->w, (float)src->h };
    const SDL_FPoint center = { 0, 0 };
    SDL_FRect dst = { x, y, (float)src->w, (float)src->h };
    SDL_RenderCopyExF(renderer, spriteAtlas.GetTexture(), src, &dst, 0, &center, flip);
}

void Game::DrawTexture(SDL_Texture* texture, int x, int y, int w, int h) const {
    const SDL_Rect src = { 0, 0, w, h };
    SDL_Rect dst = { x, y, src.w, src.h };
    SDL_RenderCopy(renderer, texture, &src, &dst);
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
        currentScene->MouseButton(this, button.which, button.state);
    }
}

void Game::SwitchScene() {
    if (currentScene) {
        delete currentScene;
    }
    currentScene = new LevelScene(this);
}

void Game::PushDebugText(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    vsprintf(debugBuffer + strlen(debugBuffer), fmt, va);
    va_end(va);
}
