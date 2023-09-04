#include "Raster.h"

Raster::Raster(Context* context, int width, int height) : context(context), width(width), height(height), size(width*height){
	pixels = new Pixel[size];

	/*for (size_t i = 0; i < size; i++) {
		pixels[i] = Pixel{ 0 };
	}*/
}

Raster::~Raster() {
	delete pixels;
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

void Raster::SetSquareToValue(coord c, int value, int color, unsigned int size=1)
{
	size -= 1;

	for (int y = c.y - size; y <= c.y + size; y++) {
		for (int x = c.x - size; x <= c.x + size; x++) {
			Pixel& pixel = pixels[y * context->VIEW_WIDTH + x];
			pixel.SetValue(value);
			pixel.SetColor(color);
		}
	}
}
int Raster::CoordToIndex(coord c) {
	return (width*c.y) + c.x;
}
