#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "Input/InputManager.h"
#include "Renderer/Raster.h"
#include "Renderer/TextRenderer.h"
#include "Utility/Profiler.h"


struct ARGB {
    uint_fast8_t A;
    uint_fast8_t R;
    uint_fast8_t G;
    uint_fast8_t B;
};

Raster raster;

struct OHE_Context {
    int RESOLUTION_WIDTH = 400;
    int RESOLUTION_HEIGHT = 300;
    int RESOLUTION_MULTIPLIER = 4;

	int VIEW_WIDTH = 400;
	int VIEW_HEIGHT = 300;

    int PIXEL_LENGTH = RESOLUTION_WIDTH * RESOLUTION_HEIGHT;

    SDL_Window* window;
    SDL_bool fullscreen = SDL_bool::SDL_TRUE;
    SDL_Renderer* renderer;

    InputManager* inputManager;
    ResourceManager* resourceManager;
    TextRenderer* textRenderer;

    // Indices used as numbers 0-9 and mapped to corresponding keyboard buttons
    bool debugFlags[10] = { false,true,true,false,false,false,false,false,false,false };

    char fpsTextBuffer[20];
    char msTextBuffer[20];

    SDL_Texture* buffer;
	SDL_FRect textureSrcRect = SDL_FRect{};
	SDL_FRect textureDstRect = SDL_FRect{};

    ARGB black = { 255,0,0,0 };
    ARGB white = { 255,255,255,255 };


    void ApplyResolution() {
		VIEW_WIDTH = RESOLUTION_WIDTH / RESOLUTION_MULTIPLIER;
		VIEW_HEIGHT = RESOLUTION_HEIGHT / RESOLUTION_MULTIPLIER;
        PIXEL_LENGTH = VIEW_WIDTH * VIEW_HEIGHT;

		textureSrcRect = SDL_FRect{	0,0,(float)VIEW_WIDTH,(float)VIEW_HEIGHT };
		textureDstRect = SDL_FRect{ 0,0,(float)RESOLUTION_WIDTH,(float)RESOLUTION_HEIGHT };
    }
};

SDL_Texture* LoadTexture(const char* filename, SDL_Renderer* renderer) {
    // Initialize the texture pointer
    SDL_Texture* texture = nullptr;

    // Load the image into an SDL_Surface object
    SDL_Surface* surface = IMG_Load(filename);
    if (surface == nullptr) {
        // Handle error
        SDL_Log("Unable to load image %s! SDL_image Error: %s\n", filename, IMG_GetError());
        return nullptr;
    }

    // Create a texture from the surface
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        // Handle error
        SDL_Log("Unable to create texture from %s! SDL Error: %s\n", filename, SDL_GetError());
    }

    // Free the surface, we no longer need it
    SDL_DestroySurface(surface);

    return texture;
}


Pixel swapPixel = Pixel{};

static inline void swap_pixels(Pixel& a, Pixel& b) {
	swapPixel = a;
	a = b;
	b = swapPixel;
}

inline bool parse_sand(OHE_Context& context, size_t& index) {
    Pixel& pixel = raster[index];
    Pixel swapPixel = Pixel{};
    if (!pixel.Exists()) return false;
    if (pixel.IsDynamic() && pixel.IsAwake()) {
        if (index < context.PIXEL_LENGTH - 1 - context.VIEW_WIDTH) {
            Pixel& pixel_below = raster[index + context.VIEW_WIDTH];
            if (!pixel_below.Exists()) {
                swap_pixels(pixel, pixel_below);
            }
            else {
                Pixel& pixel_bottom_left = raster[index + context.VIEW_WIDTH - 1];
                Pixel& pixel_bottom_right = raster[index + context.VIEW_WIDTH + 1];
                if (pixel_bottom_left.Exists() && pixel_bottom_right.Exists()) {
                    pixel.SetAwake(false);
                }
                else {
					if (!pixel_bottom_left.Exists() && !pixel_bottom_right.Exists()) {
						if (std::rand() % 2 == 0) {
							swap_pixels(pixel, pixel_bottom_left);
						}
						else {
                            swap_pixels(pixel, pixel_bottom_right);
						}
					}
					else {
						if (!pixel_bottom_left.Exists()) {
                            swap_pixels(pixel, pixel_bottom_left);
						}
						else if (!pixel_bottom_right.Exists()) {
                            swap_pixels(pixel, pixel_bottom_right);
						}
					}
                }
            }
        }
        else {
            pixel.SetAwake(false);
        }
    }
    return false;
}




void DrawPixels(OHE_Context& context) {
    void* pixels = nullptr;
    int pitch = 0;

    SDL_LockTexture(context.buffer, NULL, &pixels, &pitch);

    ARGB* ARGBptr = reinterpret_cast<ARGB*>(pixels);
    for (size_t i = 0; i < context.PIXEL_LENGTH; i++) {
        Pixel &pixel = raster[i];
        if (pixel.Exists()) {
            if (pixel.IsAwake()) {
                *ARGBptr = context.white;
            }
            else {
                *ARGBptr = ARGB{ 255,255,0,0 };
            }
        }
        else {
            *ARGBptr = context.black;
        }
        ARGBptr++;
    }

    SDL_UnlockTexture(context.buffer);
}

void DrawDebug(OHE_Context& context) {
    SDL_FRect rect = SDL_FRect{ 100,100,100,100 };
    SDL_SetRenderDrawColor(context.renderer, 0, 255, 0, 0);
    SDL_RenderRect(context.renderer, &rect);
}

void RenderGame(OHE_Context& context) {
    DrawPixels(context);
    SDL_RenderTexture(context.renderer, context.buffer, &context.textureSrcRect, &context.textureDstRect);
}

void RenderDebug(OHE_Context& context) {
    if (context.debugFlags[1]) context.textRenderer->Render(context.fpsTextBuffer, Point{ context.RESOLUTION_WIDTH - 20,20 }, SDL_Color{ 0,255,0,255 },12, TextAlignment::Right);
    if (context.debugFlags[2]) context.textRenderer->Render(context.msTextBuffer, Point{context.RESOLUTION_WIDTH - 32,34}, SDL_Color{ 0,255,0,255 }, 12, TextAlignment::Right);
    if (context.debugFlags[3]) DrawDebug(context);
}

void Render(OHE_Context& context) {
    RenderGame(context);
    RenderDebug(context);
    SDL_RenderPresent(context.renderer);
}

void Physics(OHE_Context& context, double elapsed) {

    for (size_t i = context.PIXEL_LENGTH - 1; i != SIZE_MAX; i--) {
        parse_sand(context, i);
    }
}

void UI(OHE_Context& context) {
    /*char buffer[50];
    sprintf_s(buffer, "%d fps", sdl_data.fps);*/
}

void Tick(OHE_Context& context, double elapsed) {
    Physics(context, elapsed);
    //UI(sdl_data);
    Render(context);
}

void InitGame(OHE_Context& context) {

    std::srand(std::time(nullptr));
    raster = Raster(context.VIEW_WIDTH, context.VIEW_HEIGHT);

    /*for (size_t i = 0; i < sdl_data.PIXEL_LENGTH; i++) {
        unsigned char gradient = ((float)i / (float)sdl_data.PIXEL_LENGTH) * 255.f;
        unsigned char inv_gradient = 255 - gradient;
        unsigned char noise = std::rand() % 100;
        raster.emplace_back(Pixel{ std::rand() % 5 == 0 , ARGB{255,255,noise,noise} });
        //raster.emplace_back(Pixel{ std::rand() % 5 == 0 , BGRA{255,255,255,255} });
    }*/

    /*const int centerX = sdl_data.VIEW_WIDTH / 4;
    const int centerY = sdl_data.VIEW_HEIGHT / 4;
    const int radius = 50; // Adjust this radius as needed

    for (int y = 0; y < sdl_data.VIEW_WIDTH; ++y) {
        for (int x = 0; x < sdl_data.VIEW_HEIGHT; ++x) {
            int distance = static_cast<int>(std::sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY)));

            if (distance <= radius) {
                if (std::rand() % 5 == 0) {
                    coord c = coord{ x,y };
                    raster.GetPixel(c).SetValue(7);
                }
            }
        }
    }*/

    const int rectLeft = 200;      // Left boundary of the rectangle
    const int rectRight = 800; // Right boundary of the rectangle

    for (int x = 0; x < context.VIEW_WIDTH; x++) {
        for (int y = 0; y < context.VIEW_HEIGHT; y++) {
            if (x >= rectLeft && x <= rectRight) {
                unsigned char noise = std::rand() % 50;
                if (std::rand() % 5 == 0) {
                    coord c = coord{ x,y };
                    raster.GetPixel(c).SetValue(7);
                }
            }
        }
    }
    /*for (int y = 0; y < sdl_data.VIEW_HEIGHT; ++y) {
        for (int x = 0; x < sdl_data.VIEW_WIDTH; ++x) {
            if (x % 10 > 5) {
                coord c = coord{ x,y };
                raster.GetPixel(c).SetValue(7);
            }
        }
    }*/
}

OHE_Context& InitSDL() {
    OHE_Context context;

    SDL_Init(SDL_INIT_VIDEO);

    context.window = SDL_CreateWindow("PixelGame", context.RESOLUTION_WIDTH, context.RESOLUTION_HEIGHT, 0);
    context.renderer = SDL_CreateRenderer(context.window, NULL, SDL_RENDERER_ACCELERATED);
    
    SDL_SetWindowFullscreen(context.window, context.fullscreen);
    SDL_GetCurrentRenderOutputSize(context.renderer, &context.RESOLUTION_WIDTH, &context.RESOLUTION_HEIGHT);
    context.ApplyResolution();

    context.buffer = SDL_CreateTexture(context.renderer,
        SDL_PIXELFORMAT_BGRA8888,
        SDL_TEXTUREACCESS_STREAMING,
        context.VIEW_WIDTH,
        context.VIEW_HEIGHT);

    context.resourceManager = new ResourceManager(context.renderer);
    
    context.textRenderer = new TextRenderer(context.renderer, context.resourceManager);
    context.textRenderer->LoadFontTexture("T_Font_ascii", "assets/textures/ascii.png", 16);

    return context;
}

void DestroySDL(OHE_Context& context) {
    delete context.resourceManager;
    delete context.textRenderer;
}

int main(int argc, char* argv[]) {
    OHE_Context context = InitSDL();

    context.inputManager = new InputManager();

    bool quit = false;

    const int targetFPS = -1;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);

    int frameCount = 0;
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    auto lastDebugTime = std::chrono::high_resolution_clock::now();

    InitGame(context);

    while (!quit) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastFrameTime);
        auto debugElapsed = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastDebugTime);

        context.inputManager->PollEvents();

        if (context.inputManager->IsQuitRequested() || context.inputManager->IsPressed_Escape()) {
            quit = true;
            break;
        }

        if (context.inputManager->IsPressed_1()) context.debugFlags[1] = !context.debugFlags[1];
        if (context.inputManager->IsPressed_2()) context.debugFlags[2] = !context.debugFlags[2];
        if (context.inputManager->IsPressed_3()) context.debugFlags[3] = !context.debugFlags[3];
        
        if (elapsed >= frameDuration) {
            lastFrameTime = currentTime;
            frameCount++;

            if (debugElapsed.count() > 250000) {
                lastDebugTime = currentTime;

                // Calculate frames per second
                float fps = static_cast<float>(frameCount) * 1000000 / debugElapsed.count();
                sprintf_s(context.fpsTextBuffer, "%.1f fps", fps);

                // Display elapsed milliseconds
                float elapsedMicroseconds = (float)debugElapsed.count() / 1000 / frameCount;
                sprintf_s(context.msTextBuffer, "%.3f ms", elapsedMicroseconds);

                frameCount = 0;  // Reset frame count
            }

            Tick(context, elapsed.count());
        }
    }

    DestroySDL(context);
    SDL_DestroyRenderer(context.renderer);
    SDL_DestroyWindow(context.window);
    SDL_Quit();

    return 0;
}