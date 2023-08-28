#pragma once

#include <SDL3/SDL.h>
#include <Engine/ResourceManager.h>

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
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* fontTexture = nullptr;
	ResourceManager* resourceManager = nullptr;

	int fontSize = 12;
	int gridSize = 16;
	TextAlignment alignment = TextAlignment::Left;

public:
	TextRenderer() {};
	TextRenderer(SDL_Renderer* renderer, ResourceManager* resourceManager);

	void LoadFontTexture(const std::string& id, const std::string& filePath, int gridSize);

	void Render(
		const char* text,
		const Point &location,
		const SDL_Color color=SDL_Color{255,255,255,255},
		int fontSize=12,
		TextAlignment alignment=TextAlignment::Left
	);
};