#include "Debug.h"

Debug::Debug(Context* context) : context(context) {
	frameElapsed = 0;
	profiler = &Profiler::GetInstance();
};

void Debug::Tick() {
	ProfilingInfo();
}

void Debug::Render() {
	if (context->debugFlags[1]) context->gameRenderer->DrawText(context->fpsTextBuffer, Point{ context->RESOLUTION_WIDTH - 20,20 }, SDL_Color{ 0,255,0,255 }, 12, TextAlignment::Right);
	if (context->debugFlags[2]) context->gameRenderer->DrawText(context->msTextBuffer, Point{ context->RESOLUTION_WIDTH - 32,34 }, SDL_Color{ 0,255,0,255 }, 12, TextAlignment::Right);
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
	ARGB fillColor{ 255,0,255,0 };
	for (size_t ti = 0; ti < context->raster->tileCount; ti++) {
		Tile& tile = context->raster->GetTile(ti);
		if (tile.awakePixels > 0 || tile.IsDirty()) {
			context->gameRenderer->DrawRectangle(
				tile.updateBounds.x1,
				tile.updateBounds.y1,
				tile.updateBounds.x2 - tile.updateBounds.x1 + 1,
				tile.updateBounds.y2 - tile.updateBounds.y1 + 1,
				fillColor,
				true
			);
		}
		context->gameRenderer->DrawRectangle(
			tile.tileBounds.x1,
			tile.tileBounds.y1,
			tile.w,
			tile.h,
			borderColor,
			true
		);
	}
	for (size_t y = 0; y < context->raster->rows; y++) {
		for (size_t x = 0; x < context->raster->columns; x++) {
			
		}
	}
}