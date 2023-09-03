#include "Renderer.h"

Renderer::Renderer(Context* context) : context(context) {
	textureSrcRect = SDL_FRect{ 0,0,(float)context->VIEW_WIDTH,(float)context->VIEW_HEIGHT };
	textureDstRect = SDL_FRect{ 0,0,(float)context->RESOLUTION_WIDTH,(float)context->RESOLUTION_HEIGHT };

    buffer = SDL_CreateTexture(context->renderer,
        SDL_PIXELFORMAT_BGRA8888,
        SDL_TEXTUREACCESS_STREAMING,
        context->VIEW_WIDTH,
        context->VIEW_HEIGHT);

    white = { 255,255,255,255 };
    black = { 255,0,0,0 };

    textRenderer = new TextRenderer(context);
    textRenderer->LoadFontTexture("T_Font_ascii", "assets/textures/ascii.png", 16);
}

Renderer::~Renderer() {
    delete textRenderer;
    SDL_DestroyTexture(buffer);
}

void Renderer::DrawText(const char* text,
    const Point& location,
    const SDL_Color color = SDL_Color{ 255,255,255,255 },
    int fontSize = 12,
    TextAlignment alignment = TextAlignment::Left) 
{
    textRenderer->Render(text, location, color, fontSize, alignment);
}

void Renderer::DrawPixels() {
    void* pixels = nullptr;
    int pitch = 0;

    SDL_LockTexture(buffer, NULL, &pixels, &pitch);

    ARGB* ARGBptr = reinterpret_cast<ARGB*>(pixels);
    for (size_t i = 0; i < context->TOTAL_PIXELS; i++) {
        Pixel& pixel = context->raster->GetPixel(i);
        if (pixel.Exists()) {
            if (pixel.IsAwake()) {
                *ARGBptr = white;
            }
            else {
                *ARGBptr = ARGB{ 255,255,0,0 };
            }
        }
        else {
            *ARGBptr = black;
        }
        ARGBptr++;
    }

    SDL_UnlockTexture(buffer);
}


void Renderer::DrawDebug() {
    SDL_FRect rect = SDL_FRect{ 100,100,100,100 };
    SDL_SetRenderDrawColor(context->renderer, 0, 255, 0, 0);
    SDL_RenderRect(context->renderer, &rect);
}

void Renderer::RenderGame() {
    DrawPixels();
    SDL_RenderTexture(context->renderer, buffer, &textureSrcRect, &textureDstRect);
}

void Renderer::Render() {
    RenderGame();

#if defined(_NSHIPPING)
    context->debug->Render();
#endif

    SDL_RenderPresent(context->renderer);
}

void Renderer::DrawRectangle(float x, float y, float w, float h, ARGB& color, bool upscale) {
    SDL_FRect rect = SDL_FRect{ x,y,w,h };
    if (upscale) {
        rect.x = x * context->RESOLUTION_MULTIPLIER;
        rect.y = y * context->RESOLUTION_MULTIPLIER;
        rect.y = y * context->RESOLUTION_MULTIPLIER;
        rect.w = w * context->RESOLUTION_MULTIPLIER;
        rect.h = h * context->RESOLUTION_MULTIPLIER;
    }
    SDL_SetRenderDrawBlendMode(context->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(context->renderer, color.R, color.G, color.B, color.A);
    SDL_RenderRect(context->renderer, &rect);
}

void Renderer::DrawFilledRectangle(float x, float y, float w, float h, ARGB& color, bool upscale) {
    SDL_FRect rect = SDL_FRect{ x,y,w,h };
    if (upscale) {
        rect.x = x * context->RESOLUTION_MULTIPLIER;
        rect.y = y * context->RESOLUTION_MULTIPLIER;
        rect.y = y * context->RESOLUTION_MULTIPLIER;
        rect.w = w * context->RESOLUTION_MULTIPLIER;
        rect.h = h * context->RESOLUTION_MULTIPLIER;
    }
    SDL_SetRenderDrawBlendMode(context->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(context->renderer, color.R, color.G, color.B, color.A);
    SDL_RenderFillRect(context->renderer, &rect);
}