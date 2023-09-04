#pragma once

#include <vector>
#include "Engine/Context.h"
#include "Pixel.h"

struct coord {
	int x;
	int y;
};

class Context;

class Raster {
private:
	Context* context;
public:
	int width;
	int height;
	int size;
	Pixel* pixels;

	Raster(Context* context, int width, int height);
	~Raster();

	Pixel& GetPixel(const int index);
	Pixel& GetPixel(coord c);
	Pixel& operator[] (const int index);

	inline coord IndexToCoord(int index);
	inline int CoordToIndex(coord c);
	
	void SetSquareToValue(coord c, int value, int color, unsigned int size);
};