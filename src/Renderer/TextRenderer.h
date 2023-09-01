#pragma once

#include <SDL3/SDL.h>
#include "Utility/Profiler.h"
#include <Engine/Context.h>
#include <Engine/ResourceManager.h>

class Context;
class ResourceManager;

struct Point {
	int x;
	int y;
};

enum TextAlignment {
	Left,
	Center,
	Right
};

class TextRenderer {

private:
	Context* context;
	SDL_Texture* fontTexture = nullptr;

	int fontSize = 12;
	int gridSize = 16;
	TextAlignment alignment = TextAlignment::Left;

public:
	TextRenderer(Context* context);

	void LoadFontTexture(const std::string& id, const std::string& filePath, int gridSize);

	void Render(
		const char* text,
		const Point &location,
		const SDL_Color color=SDL_Color{255,255,255,255},
		int fontSize=12,
		TextAlignment alignment=TextAlignment::Left
	);
};