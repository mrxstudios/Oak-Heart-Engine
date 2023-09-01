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
}

Renderer::~Renderer() {
    SDL_DestroyTexture(buffer);
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

void Renderer::RenderDebug() {
    if (context->debugFlags[1]) context->textRenderer->Render(context->fpsTextBuffer, Point{ context->RESOLUTION_WIDTH - 20,20 }, SDL_Color{ 0,255,0,255 }, 12, TextAlignment::Right);
    if (context->debugFlags[2]) context->textRenderer->Render(context->msTextBuffer, Point{ context->RESOLUTION_WIDTH - 32,34 }, SDL_Color{ 0,255,0,255 }, 12, TextAlignment::Right);
    if (context->debugFlags[3]) DrawDebug();
}

void Renderer::Render() {
    RenderGame();
    RenderDebug();
    SDL_RenderPresent(context->renderer);
}