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

public:
	Physics(Context* context);

	void Tick(double deltaTime);
	inline bool ParseSand(Raster& raster, Tile& tile, coord& c);

	friend class Debug;

private:
	void ParseColumn(Raster& raster, bounds& b);
	inline bool SandCanFall(coord c, Pixel& bottomLeft, Pixel& bottom, Pixel& bottomRight);

	inline bool AtBounds(coord& c);
	inline bool AtLeftBound(coord& c);
	inline bool AtRightBound(coord& c);
	inline bool AtTopBound(coord& c);
	inline bool AtBottomBound(coord& c);
};