#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

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

struct SDLdata {
    int RESOLUTION_WIDTH = 400;
    int RESOLUTION_HEIGHT = 300;
    int RESOLUTION_MULTIPLIER = 4;

	int VIEW_WIDTH = 400;
	int VIEW_HEIGHT = 300;

    int PIXEL_LENGTH = RESOLUTION_WIDTH * RESOLUTION_HEIGHT;

    SDL_Window* window;
    SDL_bool fullscreen = SDL_bool::SDL_TRUE;
    SDL_Renderer* renderer;

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

inline bool parse_sand(SDLdata& sdl_data, size_t& index) {
    Pixel& pixel = raster[index];
    Pixel swapPixel = Pixel{};
    if (!pixel.Exists()) return false;
    if (pixel.IsDynamic() && pixel.IsAwake()) {
        if (index < sdl_data.PIXEL_LENGTH - 1 - sdl_data.VIEW_WIDTH) {
            Pixel& pixel_below = raster[index + sdl_data.VIEW_WIDTH];
            if (!pixel_below.Exists()) {
                swap_pixels(pixel, pixel_below);
            }
            else {
                Pixel& pixel_bottom_left = raster[index + sdl_data.VIEW_WIDTH - 1];
                Pixel& pixel_bottom_right = raster[index + sdl_data.VIEW_WIDTH + 1];
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




void DrawPixels(SDLdata& sdl_data) {
    void* pixels = nullptr;
    int pitch = 0;

    SDL_LockTexture(sdl_data.buffer, NULL, &pixels, &pitch);

    ARGB* ARGBptr = reinterpret_cast<ARGB*>(pixels);
    for (size_t i = 0; i < sdl_data.PIXEL_LENGTH; i++) {
        Pixel &pixel = raster[i];
        if (pixel.Exists()) {
            if (pixel.IsAwake()) {
                *ARGBptr = sdl_data.white;
            }
            else {
                *ARGBptr = ARGB{ 255,255,0,0 };
            }
        }
        else {
            *ARGBptr = sdl_data.black;
        }
        ARGBptr++;
    }

    SDL_UnlockTexture(sdl_data.buffer);
}

void DrawDebug(SDLdata& sdl_data) {
    SDL_FRect rect = SDL_FRect{ 100,100,100,100 };
    SDL_SetRenderDrawColor(sdl_data.renderer, 0, 255, 0, 0);
    SDL_RenderRect(sdl_data.renderer, &rect);
}

void RenderGame(SDLdata& sdl_data) {
    DrawPixels(sdl_data);
    SDL_RenderTexture(sdl_data.renderer, sdl_data.buffer, &sdl_data.textureSrcRect, &sdl_data.textureDstRect);
}

void RenderDebug(SDLdata& sdl_data) {
    if (sdl_data.debugFlags[1]) sdl_data.textRenderer->Render(sdl_data.fpsTextBuffer, Point{ sdl_data.RESOLUTION_WIDTH - 20,20 }, SDL_Color{ 0,255,0,255 },12, TextAlignment::Right);
    if (sdl_data.debugFlags[2]) sdl_data.textRenderer->Render(sdl_data.msTextBuffer, Point{sdl_data.RESOLUTION_WIDTH - 32,34}, SDL_Color{ 0,255,0,255 }, 12, TextAlignment::Right);
    if (sdl_data.debugFlags[3]) DrawDebug(sdl_data);
}

void Render(SDLdata& sdl_data) {
    RenderGame(sdl_data);
    RenderDebug(sdl_data);
    SDL_RenderPresent(sdl_data.renderer);
}

void Physics(SDLdata& sdl_data, double elapsed) {

    for (size_t i = sdl_data.PIXEL_LENGTH - 1; i != SIZE_MAX; i--) {
        parse_sand(sdl_data, i);
    }
}

void UI(SDLdata& sdl_data) {
    /*char buffer[50];
    sprintf_s(buffer, "%d fps", sdl_data.fps);*/
}

void Tick(SDLdata& sdl_data, double elapsed) {
    Physics(sdl_data, elapsed);
    //UI(sdl_data);
    Render(sdl_data);
}

void InitGame(SDLdata& sdl_data) {

    std::srand(std::time(nullptr));
    raster = Raster(sdl_data.VIEW_WIDTH, sdl_data.VIEW_HEIGHT);

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

    for (int x = 0; x < sdl_data.VIEW_WIDTH; x++) {
        for (int y = 0; y < sdl_data.VIEW_HEIGHT; y++) {
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

SDLdata& InitSDL() {
    SDLdata sdl_data;

    SDL_Init(SDL_INIT_VIDEO);

    sdl_data.window = SDL_CreateWindow("PixelGame", sdl_data.RESOLUTION_WIDTH, sdl_data.RESOLUTION_HEIGHT, 0);
    sdl_data.renderer = SDL_CreateRenderer(sdl_data.window, NULL, SDL_RENDERER_ACCELERATED);
    
    SDL_SetWindowFullscreen(sdl_data.window, sdl_data.fullscreen);
    SDL_GetCurrentRenderOutputSize(sdl_data.renderer, &sdl_data.RESOLUTION_WIDTH, &sdl_data.RESOLUTION_HEIGHT);
    sdl_data.ApplyResolution();

    sdl_data.buffer = SDL_CreateTexture(sdl_data.renderer,
        SDL_PIXELFORMAT_BGRA8888,
        SDL_TEXTUREACCESS_STREAMING,
        sdl_data.VIEW_WIDTH,
        sdl_data.VIEW_HEIGHT);

    sdl_data.resourceManager = new ResourceManager(sdl_data.renderer);
    
    sdl_data.textRenderer = new TextRenderer(sdl_data.renderer, sdl_data.resourceManager);
    sdl_data.textRenderer->LoadFontTexture("T_Font_ascii", "assets/textures/ascii.png", 16);

    return sdl_data;
}

void DestroySDL(SDLdata& sdl_data) {
    delete sdl_data.resourceManager;
    delete sdl_data.textRenderer;
}

int main(int argc, char* argv[]) {
    SDLdata sdl_data = InitSDL();

    bool quit = false;
    SDL_Event event;

    const int targetFPS = -1;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);

    int frameCount = 0;
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    auto lastDebugTime = std::chrono::high_resolution_clock::now();

    InitGame(sdl_data);

    while (!quit) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastFrameTime);
        auto debugElapsed = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastDebugTime);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                quit = true;
                break;
            case SDL_EVENT_KEY_DOWN:
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) { // Escape
                    quit = true;
                    break;
				}
                if (event.key.keysym.scancode == SDL_SCANCODE_0) { // 0
                    sdl_data.debugFlags[0] = !sdl_data.debugFlags[0];
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_1) { // 1
                    sdl_data.debugFlags[1] = !sdl_data.debugFlags[1];
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_2) { // 2
                    sdl_data.debugFlags[2] = !sdl_data.debugFlags[2];
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_3) { // 3
                    sdl_data.debugFlags[3] = !sdl_data.debugFlags[3];
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_4) { // 4
                    sdl_data.debugFlags[4] = !sdl_data.debugFlags[4];
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_5) { // 5
                    sdl_data.debugFlags[5] = !sdl_data.debugFlags[5];
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_6) { // 6
                    sdl_data.debugFlags[6] = !sdl_data.debugFlags[6];
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_7) { // 7
                    sdl_data.debugFlags[7] = !sdl_data.debugFlags[7];
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_8) { // 8
                    sdl_data.debugFlags[8] = !sdl_data.debugFlags[8];
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_9) { // 9
                    sdl_data.debugFlags[9] = !sdl_data.debugFlags[9];
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) { // Spacebar
                }
                break;
            }

        }
        if (elapsed >= frameDuration) {
            lastFrameTime = currentTime;
            frameCount++;

            if (debugElapsed.count() > 250000) {
                lastDebugTime = currentTime;

                // Calculate frames per second
                float fps = static_cast<float>(frameCount) * 1000000 / debugElapsed.count();
                sprintf_s(sdl_data.fpsTextBuffer, "%.1f fps", fps);

                // Display elapsed milliseconds
                float elapsedMicroseconds = (float)debugElapsed.count() / 1000 / frameCount;
                sprintf_s(sdl_data.msTextBuffer, "%.3f ms", elapsedMicroseconds);

                frameCount = 0;  // Reset frame count
            }

            Tick(sdl_data, elapsed.count());
        }
    }

    DestroySDL(sdl_data);
    SDL_DestroyRenderer(sdl_data.renderer);
    SDL_DestroyWindow(sdl_data.window);
    SDL_Quit();

    return 0;
}