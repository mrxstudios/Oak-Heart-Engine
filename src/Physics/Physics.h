#pragma once

#include "Engine/Context.h"
#include "Physics/Pixel.h"
#include "Physics/Tile.h"
#include "Debug/Debug.h"
#include "Utility/ThreadPool.h"


class Context;
class Raster;
class Tile;
class Debug;

struct swapMove {
	coord c;
	Vector2D v;
};

class Physics
{
private:
	Context* context;
	Tile* tiles;

	int tileWidth;
	int tileHeight;
	int tileColumns;
	int tileRows;
	int tileCount;

	bool parseForward = true;
	int start, end, step;

	swapMove moves[8];
	int moveCount;

public:
	Physics(Context* context);

	void Tick(double deltaTime);
	inline void ParsePixel(Raster& raster, Tile& tile, const int x, const int y);
	inline void ParseSand(Raster& raster, Tile& tile, Pixel& pixel, coord& c, int index);
	inline void ParseWater(Raster& raster, Tile& tile, Pixel& pixel, coord& c, int index);

	friend class Debug;

private:
	void ParseColumn(Raster& raster, bounds& b);
	inline bool PixelCanSlide(coord c, Pixel& bottomLeft, Pixel& bottomRight);
	inline bool PixelCanFloat(coord c, Pixel& left, Pixel& right);

	inline void AddMove(coord c, Vector2D v);

	inline bool AtBounds(coord& c);
	inline bool AtLeftBound(coord& c);
	inline bool AtRightBound(coord& c);
	inline bool AtTopBound(coord& c);
	inline bool AtBottomBound(coord& c);
};