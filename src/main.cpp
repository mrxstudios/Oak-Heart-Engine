#include "Engine/Engine.h"

int main(int argc, char* argv[])
{
    Engine engine;
    engine.Initialize();
    engine.Run();
    engine.Terminate();

    return 0;
}