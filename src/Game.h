#pragma once
#include "Sprites/SpriteRegistry.h"
#include "Window.h"
#include "UI/UIElement.h"
#include "UI/UIButton.h"
#include "UI/UIAbilityButton.h"
#include "Level/Level.h"

class GameScene;

class Game {
public:
    Game(Window* window);
    void LoadAssets();

    inline void Scale(float scale) { 
        this->scale = scale;
        SDL_RenderSetScale((SDL_Renderer*)renderer, scale, scale);
    }
    void Render();
    void Update(float delta);

    void DrawSprite(const Sprite& sprite, int x, int y, const SDL_RendererFlip flip = SDL_FLIP_NONE) const;
    void DrawSprite(const Sprite& sprite, float x, float y, const SDL_RendererFlip flip = SDL_FLIP_NONE) const;
    void DrawTexture(const SDL_Texture* texture, int x, int y, int w, int h) const;

    void MouseMove(const SDL_MouseMotionEvent& event);
    void MouseButton(const SDL_MouseButtonEvent& button);

    void SwitchScene();

    inline const Window* MainWindow() const { return window; }
    inline const SpriteAtlas& Atlas() const { return spriteAtlas; }
    inline const SpriteRegistry& Registry() const { return spriteRegistry; }

    inline void SetCursor(int id) { cursor.currentFrame = id; }
    inline SDL_Point MousePos() const { return { (int)(mouseX / scale), (int)(mouseY / scale)}; }

private:
    const SDL_Renderer* renderer;
    SpriteRegistry spriteRegistry;
    SpriteAtlas spriteAtlas;
    Window* window = 0;
    GameScene* currentScene = 0;

    Sprite cursor;

    float scale = 4;
    int mouseX, mouseY;
};
