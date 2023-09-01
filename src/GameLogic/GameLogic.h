#pragma once
#include "Engine/Context.h"

class Context;

class GameLogic {
private:
	Context* context;

public:
	GameLogic(Context* context);
	~GameLogic();

	void InitGame();
	void Tick(double deltaTime);

private:
	void SpawnRectangleOfSand();
};

/*
#include "../Player/Player.h"
#include "../Entity/Entity.h"
#include "../Physics/PhysicsSystem.h"
#include "../Renderer/Renderer.h"
#include "../UI/UIManager.h"
#include "../Event/Event.h"

class GameLogic {
public:
	GameLogic();
	~GameLogic();

	void Update(float deltaTime);
	void HandleEvent(const Event& event);
	void Render(Renderer& renderer);

private:
	Player player;
	std::vector<Entity> entities;
	PhysicsSystem physicsSystem;
	UIManager uiManager;

	void UpdatePlayer(float deltaTime);
	void UpdateEntities(float deltaTime);
	void UpdatePhysics(float deltaTime);
	void UpdateAI(float deltaTime);

	void RenderEntities(Renderer& renderer);
	void RenderUI(Renderer& renderer);

	// Other private methods and
}*/