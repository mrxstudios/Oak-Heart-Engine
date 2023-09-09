#pragma once

#include "Engine/Context.h" // Assuming OHE_Context is defined in its own header
#include "Engine/ResourceManager.h" // Assuming ResourceManager is defined in its own header
#include "TextRenderer.h" // Assuming TextRenderer is defined in its own header
#include "Palette.h"

#include <SDL3/SDL.h>

class Context;
class ResourceManager;
class TextRenderer;

class Renderer {
private:
    Context* context;
    TextRenderer* textRenderer;

    // Colors
    ARGB black;
    ARGB white;

    // Textures
    SDL_Texture* buffer;
    SDL_FRect textureSrcRect;
    SDL_FRect textureDstRect;


public:
    Renderer(Context* context);
    ~Renderer();

    void DrawPixels();
    void DrawPixelsFromTiles();
    void RenderGame();
    void Render();

    void DrawRectangle(float x, float y, float w, float h, ARGB& color, bool upscale);
    void DrawFilledRectangle(float x, float y, float w, float h, ARGB& color, bool upscale);
    void DrawText(const char* text,
        const Point& location,
        const SDL_Color color = SDL_Color{ 255,255,255,255 },
        int fontSize = 12,
        TextAlignment alignment = TextAlignment::Left);
};