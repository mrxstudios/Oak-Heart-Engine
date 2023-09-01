#pragma once

#include "Engine/Context.h"

class Context;

class Physics
{
private:
	Context* context;
	Pixel swapPixel;

public:
	Physics(Context* context);

	void Tick(double deltaTime);

private:
	inline bool ParseSand(size_t index);
	inline void SwapPixels(Pixel& a, Pixel& b);
};