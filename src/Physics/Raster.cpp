#include "Raster.h"

Raster::Raster(Context* context, int width, int height) : context(context), width(width), height(height), size(width*height){
	GenerateTiles();
	
	pixels = new Pixel[size];

	for (size_t i = 0; i < size; i++) {
		pixels[i] = Pixel{ 0,0 };
	}
}

Raster::~Raster() {
	delete pixels;
}

void Raster::GenerateTiles() {
	columns = std::ceil((float)context->RASTER_WIDTH / context->TILE_SIZE);
	rows = std::ceil((float)context->RASTER_HEIGHT / context->TILE_SIZE);
	tileCount = rows * columns;
	tiles.reserve(tileCount);

	for (size_t y = 0; y < rows; y++) {
		for (size_t x = 0; x < columns; x++) {
			int index = y * columns + x;

			int width = context->TILE_SIZE;
			int height = context->TILE_SIZE;

			if (x == columns - 1) {
				width = context->RASTER_WIDTH - (context->TILE_SIZE * (columns - 1));
			}
			if (y == rows - 1) {
				height = context->RASTER_HEIGHT - (context->TILE_SIZE * (rows - 1));
			}

			int startIndex = index * context->TILE_SIZE * context->TILE_SIZE;

			coord leftCorner = { x * context->TILE_SIZE, y * context->TILE_SIZE };

			tiles.push_back(Tile(x, y, width, height, startIndex, leftCorner));
			Tile& tile = tiles.back();

			/*t */if (index >= columns) tile.neighbourIndices[0] = index - columns;
			/*tr*/if ((index >= columns) && (index % columns < (columns - 1))) tile.neighbourIndices[1] = index - columns + 1;
			/*r */if ((index % columns) < (columns - 1)) tile.neighbourIndices[2] = index + 1;
			/*br*/if ((index < (rows - 1) * columns) && (index % columns < (columns - 1))) tile.neighbourIndices[3] = index + columns + 1;
			/*b */if (index < (rows - 1) * columns) tile.neighbourIndices[4] = index + columns;
			/*bl*/if ((index < (rows - 1) * columns) && (index % columns > 0)) tile.neighbourIndices[5] = index + columns - 1;
			/*l */if ((index % columns) > 0) tile.neighbourIndices[6] = index - 1;
			/*tl*/if (((index % columns) > 0) && (index >= columns)) tile.neighbourIndices[7] = index - columns - 1;
		}
	}
}

void Raster::SetPixel(coord& c, int value, uint8_t color) {
	Tile& tile = GetTile(c);
	int pixelIndex = CoordToIndex(c);
	Pixel& pixel = pixels[pixelIndex];

	if ((value & PIXEL_EXISTS) == PIXEL_EXISTS) {
		if (!pixel.CheckState(PIXEL_EXISTS)) {
			tile.occupiedPixels++;
		}
	}

	pixels[CoordToIndex(c)].SetValueAndColor(value,color);
	tile.UpdateRenderBounds(c);

	tile.shouldRender = true;
	tile.shouldStep = true;
}

Pixel& Raster::GetPixel(const int index) {
	return pixels[index];
}

Pixel& Raster::GetPixel(coord c) {
	return pixels[CoordToIndex(c)];
}

Tile& Raster::GetTile(const int index) {
	return tiles[index];
}

Tile& Raster::GetTileFromRasterIndex(const int index) {
	return tiles[GetTileIndex(index)];
}

Tile& Raster::GetTile(coord& c) {
	return tiles[GetTileIndex(c)];
}

int Raster::GetTileIndex(const int index) {
	int x = (index % context->RASTER_WIDTH) / context->TILE_SIZE;
	int y = (index / context->RASTER_WIDTH) / context->TILE_SIZE;
	return y * columns + x;
}

int Raster::GetTileIndex(coord& c) {
	return (c.y / context->TILE_SIZE) * columns + (c.x / context->TILE_SIZE);
}

void Raster::ResetTilesAndPixels() {
	for (size_t i = 0; i < tileCount; i++) {
 		Tile& tile = tiles[i];
		if (tile.shouldRender) {
			for (size_t x = tile.tileBounds.x1; x < tile.tileBounds.x2; x++) {
				for (size_t y = tile.tileBounds.y1; y < tile.tileBounds.y2; y++) {
					int index = y * context->RASTER_WIDTH + x;
					pixels[index].ClearBit(PIXEL_UPDATED);
				}
			}
		}
		tile.shouldStep = tile.shouldRender;

		// If a neighbouring tile was set to Render previous frame, run physics this frame.
		if (!tile.shouldStep && tile.occupiedPixels > 0) {
			for (size_t j = 0; j < 8; j++) {
				int index = tile.neighbourIndices[j];
				if (index < 0) continue;
				if (tiles[tile.neighbourIndices[j]].shouldRender) {
					tile.shouldStep = true;
					break;
				}
			}
		}
	}
	for (size_t i = 0; i < tileCount; i++) {
		Tile& tile = tiles[i];
		tile.shouldRender = false;
		tile.updateBounds = { 0,0,0,0 };
	}
}

Pixel& Raster::operator[] (const int nIndex) {
	return pixels[nIndex];
}

coord Raster::IndexToCoord(const int index) {
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
			if (x >= 0 && x < context->RASTER_WIDTH && y >= 0 && y < context->RASTER_HEIGHT) {
				coord c2 = { x,y };
				context->raster->SetPixel(c2, value, color);
			}
		}
	}
}
int Raster::CoordToIndex(coord& c) {
	return c.y * context->RASTER_WIDTH + c.x;
}

void Raster::SwapPixels(Tile& tile1, Pixel& pixel1, coord c1, coord c2) {
	Pixel& pixel2 = GetPixel(c2);
	Tile& tile2 = GetTile(c2);

	tile1.UpdateRenderBounds(c1);
	tile2.UpdateRenderBounds(c2);

	pixel1.SetBit(PIXEL_UPDATED);
	pixel2.SetBit(PIXEL_UPDATED);

	tile1.shouldRender = true;
	tile2.shouldRender = true;

	if (tile1.startIndex != tile2.startIndex) {
		if (pixel2.CheckState(PIXEL_EMPTY)) {
			tile1.occupiedPixels--;
			tile2.occupiedPixels++;
		}
	}

	SwapPixelValues(pixel1, pixel2);
};

void Raster::SwapPixelValues(Pixel& a, Pixel& b) {
	swapPixel = a;
	a = b;
	b = swapPixel;
}

int Raster::GetTop(const int index) {
	return index - context->RASTER_WIDTH;
};
int Raster::GetTopRight(const int index) {
	return index - context->RASTER_WIDTH + 1;
};
int Raster::GetRight(const int index) {
	return index + 1;
};
int Raster::GetBottomRight(const int index) {
	return index + context->RASTER_WIDTH + 1;
};
int Raster::GetBottom(const int index) {
	return index + context->RASTER_WIDTH;
};
int Raster::GetBottomLeft(const int index) {
	return index + context->RASTER_WIDTH - 1;
};
int Raster::GetLeft(const int index) {
	return index - 1;
};
int Raster::GetTopLeft(const int index) {
	return index - context->RASTER_WIDTH - 1;
};

bool Raster::AtBounds(const int index) {
	return AtTopBound(index) || AtRightBound(index) || AtBottomBound(index) || AtLeftBound(index);
}

bool Raster::AtTopBound(const int index) {
	return index < context->RASTER_WIDTH;
}

bool Raster::AtRightBound(const int index) {
	return (index % context->RASTER_WIDTH) == (context->RASTER_WIDTH - 1);
}

bool Raster::AtBottomBound(const int index) {
	return index >= (context->TOTAL_PIXELS - context->RASTER_WIDTH);
}

bool Raster::AtLeftBound(const int index) {
	return (index % context->RASTER_WIDTH) == 0;
}

bool Raster::AtTileBounds(const int index) {
	return AtTileTopBound(index) || AtTileRightBound(index) || AtTileBottomBound(index) || AtTileLeftBound(index);
}

bool Raster::AtTileTopBound(const int index) {
	return ((index / context->RASTER_WIDTH) % context->TILE_SIZE) == 0;
}

bool Raster::AtTileRightBound(const int index) {
	return (index % context->TILE_SIZE) == (context->TILE_SIZE - 1);
}

bool Raster::AtTileBottomBound(const int index) {
	return ((index / context->RASTER_WIDTH) % context->TILE_SIZE) == (context->TILE_SIZE - 1);
}

bool Raster::AtTileLeftBound(const int index) {
	return (index % context->TILE_SIZE) == 0;
}


