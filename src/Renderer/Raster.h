#pragma once

#include <vector>
#include "Pixel.h"

struct coord {
	int x;
	int y;
};

class Raster {
public:
	int width;
	int height;
	int size;
	Pixel* pixels;

	Raster(int width, int height);
	~Raster();

	Pixel& GetPixel(const int index);
	Pixel& GetPixel(coord c);
	Pixel& operator[] (const int index);

	inline coord IndexToCoord(int index);
	inline int CoordToIndex(coord c);
};