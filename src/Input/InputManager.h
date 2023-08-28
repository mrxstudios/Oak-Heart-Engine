#pragma once

#include <SDL3/SDL.h>

class InputManager {
public:
	InputManager();

	void HandleInput();

	bool IsKeyDown(SDL_Scancode key) const;

private:
	bool keys[SDL_NUM_SCANCODES];
};