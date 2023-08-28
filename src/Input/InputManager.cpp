#include "InputManager.h"

InputManager::InputManager() {
	// Initialize the keys array
	for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
		keys[i] = false;
	}
}

void InputManager::HandleInput() {
	SDL_PumpEvents(); // Update input state
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
		keys[i] = currentKeyStates[i];
	}
}

bool InputManager::IsKeyDown(SDL_Scancode key) const {
	return keys[key];
}