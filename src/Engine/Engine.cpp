#include "Engine.h"

Engine::Engine() {};

Engine::~Engine()
{
    // Cleanup logic can go here
}

void Engine::Initialize()
{
    std::srand(std::time(nullptr));

    context = new Context();
    context->Init();

    context->textRenderer->LoadFontTexture("T_Font_ascii", "assets/textures/ascii.png", 16);

    context->gameLogic->InitGame();
}

void Engine::Run()
{
    bool quit = false;
    const int targetFPS = -1;
    const std::chrono::milliseconds targetFrameTime = std::chrono::milliseconds(1000 / targetFPS);

    lastFrameTime = std::chrono::high_resolution_clock::now();

    while (!quit)
    {
#if defined(_NSHIPPING) 
        PROFILE_SCOPE_RATE("FRAME", 1000); 
#endif

        auto frameStart = std::chrono::high_resolution_clock::now();
        context->inputManager->PollEvents();

        if (context->inputManager->IsQuitRequested() || context->inputManager->IsPressed_Escape()) {
            quit = true;
            break;
        }

        if (context->inputManager->IsPressed_1()) context->debugFlags[1] = !context->debugFlags[1];
        if (context->inputManager->IsPressed_2()) context->debugFlags[2] = !context->debugFlags[2];
        if (context->inputManager->IsPressed_3()) context->debugFlags[3] = !context->debugFlags[3];

        double deltaTime = (frameStart - lastFrameTime).count();
        Tick(deltaTime);

        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);

        if (frameDuration < targetFrameTime) {
            std::this_thread::sleep_for(targetFrameTime - frameDuration);
        }
    }
}

void Engine::Terminate()
{
    context->Destroy();
}

void Engine::Tick(double deltaTime)
{
    context->gameLogic->Tick(deltaTime);
    context->physics->Tick(deltaTime);
    context->debug->Tick();
    Render();
}

void Engine::Render()
{
    context->gameRenderer->Render();
}