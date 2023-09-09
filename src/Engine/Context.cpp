#include "Context.h"
#include "Utility/Math.h"

Context::Context() {
    // Initialize the fields here, if necessary
}

Context::~Context() {
    // Destructor logic can go here
}

void Context::Init() {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("PixelGame", RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED);

    // Set fullscreen, get screen size, etc.
    SDL_SetWindowFullscreen(window, fullscreen);
    SDL_GetCurrentRenderOutputSize(renderer, &RESOLUTION_WIDTH, &RESOLUTION_HEIGHT);
    ApplyResolution();

    inputManager = new InputManager();
    palette = new Palette();
    raster = new Raster(this, RASTER_WIDTH, RASTER_HEIGHT);
    resourceManager = new ResourceManager(this);
    gameLogic = new GameLogic(this);
    physics = new Physics(this);
    gameRenderer = new Renderer(this);
    debug = new Debug(this);
}

void Context::ApplyResolution() {
    CANVAS_WIDTH = RESOLUTION_WIDTH;
    CANVAS_HEIGHT = RESOLUTION_HEIGHT;
    //CANVAS_WIDTH = Math::nearestPowerOfTwo(RESOLUTION_WIDTH);
    //CANVAS_HEIGHT = Math::nearestPowerOfTwo(RESOLUTION_HEIGHT);
    CANVAS_OFFSET_X = (RESOLUTION_WIDTH - CANVAS_WIDTH) / 2;
    CANVAS_OFFSET_Y = (RESOLUTION_HEIGHT - CANVAS_HEIGHT) / 2;

    CANVAS_MULTIPLIER = 4;

    RASTER_WIDTH = CANVAS_WIDTH / CANVAS_MULTIPLIER;
    RASTER_HEIGHT = CANVAS_HEIGHT / CANVAS_MULTIPLIER;
    TOTAL_PIXELS = RASTER_WIDTH * RASTER_HEIGHT;
}

void Context::Destroy() {
    delete inputManager;
    delete palette;
    delete gameLogic;
    delete raster;
    delete physics;
    delete resourceManager;
    delete gameRenderer;
    delete debug;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}