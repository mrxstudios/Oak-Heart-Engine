#include "Context.h"

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
    raster = new Raster(VIEW_WIDTH, VIEW_HEIGHT);
    resourceManager = new ResourceManager(this);
    gameLogic = new GameLogic(this);
    physics = new Physics(this);
    gameRenderer = new Renderer(this);
    textRenderer = new TextRenderer(this);
}

void Context::ApplyResolution() {
    RESOLUTION_MULTIPLIER = 4;
    VIEW_WIDTH = RESOLUTION_WIDTH / RESOLUTION_MULTIPLIER;
    VIEW_HEIGHT = RESOLUTION_HEIGHT / RESOLUTION_MULTIPLIER;
    TOTAL_PIXELS = VIEW_WIDTH * VIEW_HEIGHT;
}

void Context::Destroy() {
    delete inputManager;
    delete gameLogic;
    delete raster;
    delete physics;
    delete resourceManager;
    delete gameRenderer;
    delete textRenderer;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}