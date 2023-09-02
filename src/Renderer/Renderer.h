#pragma once

#include "Engine/Context.h" // Assuming OHE_Context is defined in its own header
#include "Engine/ResourceManager.h" // Assuming ResourceManager is defined in its own header
#include "TextRenderer.h" // Assuming TextRenderer is defined in its own header
#include <SDL3/SDL.h>

class Context;
class ResourceManager;
class TextRenderer;

struct ARGB {
    uint_fast8_t A;
    uint_fast8_t R;
    uint_fast8_t G;
    uint_fast8_t B;
};

class Renderer {
private:
    Context* context;

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
    void DrawDebug();
    void RenderGame();
    void RenderDebug();
    void Render();

    void DrawRectangle(float x, float y, float w, float h, ARGB& color, bool upscale);
    void DrawFilledRectangle(float x, float y, float w, float h, ARGB& color, bool upscale);
};