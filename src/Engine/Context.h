#pragma once

#include <SDL3/SDL.h>

#include "Engine/Types.h"
#include "Input/InputManager.h"
#include "Engine/ResourceManager.h"
#include "Renderer/Palette.h"
#include "Physics/Raster.h"
#include "Renderer/Renderer.h"
#include "Renderer/TextRenderer.h"
#include "GameLogic/GameLogic.h"
#include "Physics/Physics.h"
#include "Utility/ThreadPool.h"
#include "Debug/Debug.h"


class InputManager;
class Palette;
class ResourceManager;
class Raster;
class GameLogic;
class Physics;
class Renderer;
class TextRenderer;
class ThreadPool;
class Debug;

class Context {
public:
    // Screen resolution
    int RESOLUTION_WIDTH = 800;
    int RESOLUTION_HEIGHT = 600;

    // Canvas resolution
    int CANVAS_WIDTH = 800;
    int CANVAS_HEIGHT = 600;
    int CANVAS_OFFSET_X = 0;
    int CANVAS_OFFSET_Y = 0;

    // View dimensions
    int RASTER_WIDTH = 200;
    int RASTER_HEIGHT = 150;

    int CANVAS_MULTIPLIER = 4;
    int TOTAL_PIXELS = 30000;

    int TILE_SIZE = 64;

    // Window
    SDL_bool fullscreen = SDL_bool::SDL_TRUE;
    SDL_Window* window;

    // Rendering
    SDL_Renderer* renderer;

    // Debugging
    char fpsTextBuffer[100];
    char msTextBuffer[100];
    char pixelCountTextBuffer[100];

    bool debugFlags[4] { false,true,true,false};

    InputManager* inputManager;
    GameLogic* gameLogic;
    Physics* physics;
    Palette* palette;
    Raster* raster;
    ResourceManager* resourceManager;
    Renderer* gameRenderer;
    TextRenderer* textRenderer;
    Debug* debug;

    Context();
    ~Context();

    void Init();
    void ApplyResolution();
    void Destroy();
};