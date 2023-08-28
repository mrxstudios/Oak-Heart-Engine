#include "Raster.h"

Raster::Raster(int width, int height) : width(width), height(height), size(width*height){
	pixels.reserve(size);

	for (size_t i = 0; i < size; i++) {
		pixels.emplace_back(Pixel{ 0 });
	}
}

Pixel& Raster::GetPixel(const int index) {
	return pixels[index];
};

Pixel& Raster::GetPixel(coord c) {
	return pixels[CoordToIndex(c)];
};

Pixel& Raster::operator[] (const int nIndex) {
	return pixels[nIndex];
}

coord Raster::IndexToCoord(int index) {
	return coord{
		index % width,
		index / width
	};
};

int Raster::CoordToIndex(coord c) {
	return (width*c.y) + c.x;
}
