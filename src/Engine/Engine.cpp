#include "Engine.h"

Engine::Engine()
    : frameCount(0)
{
    // Initialization logic can go here
}

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
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);

    lastFrameTime = std::chrono::high_resolution_clock::now();
    lastDebugTime = std::chrono::high_resolution_clock::now();

    while (!quit)
    {
        context->inputManager->PollEvents();

        auto currentTime = std::chrono::high_resolution_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastFrameTime).count();
        auto debugElapsed = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastDebugTime);

        if (context->inputManager->IsQuitRequested() || context->inputManager->IsPressed_Escape()) {
            quit = true;
            break;
        }

        if (context->inputManager->IsPressed_1()) context->debugFlags[1] = !context->debugFlags[1];
        if (context->inputManager->IsPressed_2()) context->debugFlags[2] = !context->debugFlags[2];
        if (context->inputManager->IsPressed_3()) context->debugFlags[3] = !context->debugFlags[3];


        if (deltaTime >= frameDuration.count())
        {
            lastFrameTime = currentTime;

            frameCount++;

            if (debugElapsed.count() > 250000) {
                lastDebugTime = currentTime;

                // Calculate frames per second
                float fps = static_cast<float>(frameCount) * 1000000 / debugElapsed.count();
                sprintf_s(context->fpsTextBuffer, "%.1f fps", fps);

                // Display elapsed milliseconds
                float elapsedMicroseconds = (float)debugElapsed.count() / 1000 / frameCount;
                sprintf_s(context->msTextBuffer, "%.3f ms", elapsedMicroseconds);

                frameCount = 0;  // Reset frame count
            }

            Tick(deltaTime);
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
    Render();
}

void Engine::Render()
{
    RenderGame();
    //RenderUI();
    RenderDebug();
}

void Engine::RenderGame()
{
    context->gameRenderer->Render();
}

void Engine::RenderDebug()
{
    // Debug rendering logic
}
