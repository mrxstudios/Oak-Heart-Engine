#pragma once

#include "Engine/Types.h"
#include "Engine/Context.h"
#include "Physics/Pixel.h"

class Context;

struct Tile
{
public:
	int x;
	int y;
	int w;
	int h;

	coord leftCorner;
	bounds tileBounds;
	bounds updateBounds;

	int neighbourIndices[8];

	int startIndex;
	int totalPixels;
	int occupiedPixels = 0;
	int awakePixels = 0;

	bool shouldRender;
	bool shouldStep;
	
	Tile(int x, int y, int w, int h, int startIndex, coord& leftCorner);

	inline int GetPixelIndex(coord& c);
	Pixel& GetPixel(coord& c);
	Pixel& GetPixel(int index);
	Pixel& GetPixelFromRasterSpace(coord& rasterC);

	void UpdateRenderBounds(coord& c);

	void RemoveAwake();
	void AddAwake();
};

