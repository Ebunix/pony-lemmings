#pragma once
#include "Sprites/SpriteRegistry.h"
#include "Window.h"
#include "UI/UIElement.h"
#include "UI/UIButton.h"
#include "UI/UIAbilityButton.h"
#include "Level/Level.h"
#include <SDL_FontCache.h>
#include "StartInfo.h"

class GameScene;

class Game {
public:
    Game(Window* window, float scale);
    void LoadAssets();

    inline void Scale(float scale) { 
        this->scale = scale;
        SDL_RenderSetScale((SDL_Renderer*)renderer, scale, scale);
    }
    void Render();
    void Update(float delta);

    void DrawSprite(const Sprite& sprite, int x, int y, const SDL_RendererFlip flip = SDL_FLIP_NONE) const;
    void DrawSprite(const Sprite& sprite, float x, float y, const SDL_RendererFlip flip = SDL_FLIP_NONE) const;
    void DrawTexture(SDL_Texture* texture, int x, int y, int w, int h) const;

    void MouseMove(const SDL_MouseMotionEvent& event);
    void MouseButton(const SDL_MouseButtonEvent& button);

    void SwitchScene();

    inline Window* MainWindow() const { return window; }
    inline FC_Font* Font() const { return fontMain; }
    inline FC_Font* FontSmall() const { return fontSmall; }
    inline const SpriteAtlas& Atlas() const { return spriteAtlas; }
    inline const SpriteRegistry& Registry() const { return spriteRegistry; }

    inline void SetCursor(int id) { cursor.currentFrame = id; }
    inline SDL_Point MousePos() const { return { (int)(mouseX / scale), (int)(mouseY / scale)}; }
    void PushDebugText(const char* fmt, ...);

private:
    char debugBuffer[0x1000];
    SDL_Renderer* renderer;
    SpriteRegistry spriteRegistry;
    SpriteAtlas spriteAtlas;
    Window* window = 0;
    GameScene* currentScene = 0;
    FC_Font* fontMain = 0;
    FC_Font* fontSmall = 0;
    Sprite cursor;

    float scale = 4;
    int mouseX, mouseY;
};
