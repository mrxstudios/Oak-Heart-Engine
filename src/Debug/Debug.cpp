#include "Debug.h"

Debug::Debug(Context* context) : context(context) {
	profiler = &Profiler::GetInstance();
};

void Debug::Tick() {
	ProfilingInfo();
}

void Debug::Render() {
	if (context->debugFlags[1]) context->textRenderer->Render(context->fpsTextBuffer, Point{ context->RESOLUTION_WIDTH - 20,20 }, SDL_Color{ 0,255,0,255 }, 12, TextAlignment::Right);
	if (context->debugFlags[2]) context->textRenderer->Render(context->msTextBuffer, Point{ context->RESOLUTION_WIDTH - 32,34 }, SDL_Color{ 0,255,0,255 }, 12, TextAlignment::Right);
	if (context->debugFlags[3]) RenderTiles();
}

void Debug::ProfilingInfo() {
	// Display framerate and frametime
	float elapsedMilliseconds = (float) profiler->GetElapsedTime(ProfilerFrameTag) / 1000;
	float fps = 1000.f / elapsedMilliseconds;
	if (frameElapsed != elapsedMilliseconds) {
		frameElapsed = elapsedMilliseconds;
		sprintf_s(context->fpsTextBuffer, "%.1f fps", fps);
		sprintf_s(context->msTextBuffer, "%.3f ms", elapsedMilliseconds);
	}
}

void Debug::RenderTiles() {
	ARGB borderColor{ 150,255,255,255 };
	ARGB fillColor{ 50,255,255,255 };
	for (size_t y = 0; y < context->physics->tileRows; y++) {
		for (size_t x = 0; x < context->physics->tileColumns; x++) {
			int index = (context->physics->tileColumns * y) + x;
			if (context->physics->tiles[index].IsDirty()) {
				context->gameRenderer->DrawFilledRectangle(
					x * context->physics->tileWidth,
					y * context->physics->tileHeight,
					context->physics->tileWidth,
					context->physics->tileHeight,
					borderColor,
					true
				);
			}
			context->gameRenderer->DrawRectangle(
				x * context->physics->tileWidth, 
				y * context->physics->tileHeight, 
				context->physics->tileWidth,
				context->physics->tileHeight,
				borderColor,
				true
			);
		}
	}
}