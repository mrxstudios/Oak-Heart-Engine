#pragma once

#include <vector>
#include "Physics/Tile.h"
#include "Engine/Context.h"
#include "Pixel.h"

class Context;
class Tile;

class Raster {
private:
	Context* context;
public:
	int width;
	int height;
	int size;
	Pixel* pixels;
	Pixel swapPixel;

	std::vector<Tile> tiles;
	int columns;
	int rows;
	int tileCount;

	Raster(Context* context, int width, int height);
	~Raster();

	void GenerateTiles();

	Pixel& GetPixel(const int index);
	Pixel& GetPixel(coord c);
	Tile& GetTileFromRasterIndex(const int index);
	Tile& GetTile(const int index);
	Tile& GetTile(coord& c);

	Pixel& operator[] (const int index);
	
	void SetPixel(coord& c, int value, uint8_t color);
	void WakeNeighbors(const int index);

	inline int GetTileIndex(coord& c);
	inline int GetTileIndex(const int index);
	
	void ResetTilesAndPixels();

	inline coord IndexToCoord(const int index);
	inline int CoordToIndex(coord& c);

	void SwapPixels(Tile& tile1, Tile& tile2, Pixel& pixel1, Pixel& pixel2, const int index1, const int index2);
	inline void SwapPixelValues(Pixel& a, Pixel& b);
	
	int GetTop(const int index);
	int GetTopRight(const int index);
	int GetRight(const int index);
	int GetBottomRight(const int index);
	int GetBottom(const int index);
	int GetBottomLeft(const int index);
	int GetLeft(const int index);
	int GetTopLeft(const int index);
	
	void SetSquareToValue(coord c, int value, int color, unsigned int size);

	inline bool AtBounds(const int index);
	inline bool AtLeftBound(const int index);
	inline bool AtRightBound(const int index);
	inline bool AtTopBound(const int index);
	inline bool AtBottomBound(const int index);

	inline bool AtTileBounds(const int index);
	inline bool AtTileLeftBound(const int index);
	inline bool AtTileRightBound(const int index);
	inline bool AtTileTopBound(const int index);
	inline bool AtTileBottomBound(const int index);
};