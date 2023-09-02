#pragma once

#include "Engine/Context.h"
#include "Physics/Tile.h"
#include "Debug/Debug.h"


class Context;
class Tile;
class Debug;

class Physics
{
private:
	Context* context;
	Pixel swapPixel;
	Tile* tiles;

	int tileWidth;
	int tileHeight;
	int tileColumns;
	int tileRows;
	int tileCount;

public:
	Physics(Context* context);

	void Tick(double deltaTime);

	friend class Debug;

private:
	inline bool ParseSand(size_t index);
	inline void SwapPixels(Pixel& a, Pixel& b);
	
	void SetupTiles();
	void CleanTiles();
	inline int GetTileIndex(int pixelIndex);
	inline void MarkTileDirty(int pixelIndex);
	
};