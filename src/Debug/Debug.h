#pragma once

#include "Engine/Context.h"
#include "Physics/Tile.h"
#include "Utility/Profiler.h"

class Context;
class Tile;
class Profiler;

class Debug
{
private:
	Context* context;
	Profiler* profiler;

	std::string ProfilerFrameTag = "FRAME";
	float frameElapsed;

public:
	Debug(Context* context);
	void Tick();
	void Render();

private:
	void RenderTiles();
	void ProfilingInfo();
};

