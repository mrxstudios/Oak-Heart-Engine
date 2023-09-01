/*#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "Input/InputManager.h"
#include "Renderer/Raster.h"
#include "Renderer/TextRenderer.h"
#include "Utility/Profiler.h"*/

#include <SDL3/SDL_main.h>
#include "Engine/Engine.h"


int main(int argc, char* argv[])
{
    Engine engine;
    engine.Initialize();
    engine.Run();
    engine.Terminate();

    return 0;
}