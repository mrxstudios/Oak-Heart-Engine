#include "Engine.h"
/*
Engine::Engine() {
	// Initialize engine components, such as rendering, input, etc.
	renderer = std::make_unique<Renderer>();
	inputManager = std::make_unique<InputManager>();
	// ... initialize other components ...
}

Engine::~Engine() {
	// Cleanup engine components
}

void Engine::Run() {
	// Start the game loop
	while (running) {
		// Handle input
		inputManager->HandleInput();

		// Update game logic based on current game state
		if (currentState) {
			currentState->Update();
		}

		// Render the game based on current game state
		if (currentState) {
			//renderer->Render(*currentState);
		}
	}
}

void Engine::SetCurrentState(std::unique_ptr<GameState> newState) {
	currentState = std::move(newState);
}

void Engine::Exit() {
	running = false;
}*/