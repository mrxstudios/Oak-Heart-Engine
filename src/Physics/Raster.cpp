#include "Raster.h"

Raster::Raster(Context* context, int width, int height) : context(context), width(width), height(height), size(width*height){
	GenerateTiles();
	
	pixels = new Pixel[size];

	for (size_t i = 0; i < size; i++) {
		pixels[i] = Pixel{ 0 };
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
		}
	}
}

void Raster::SetPixel(coord& c, int value, uint8_t color) {
	Tile& tile = GetTile(c);
	int pixelIndex = CoordToIndex(c);
	Pixel& pixel = pixels[pixelIndex];

	if ((value & PIXEL_EXISTS) == PIXEL_EXISTS) {
		if (pixel.CheckState(PIXEL_EMPTY)) {
			tile.occupiedPixels++;
		}
	}
	else {
		if (!pixel.CheckState(PIXEL_EMPTY)) {
			tile.occupiedPixels--;
		}
	}
	if ((value & PIXEL_AWAKE) == PIXEL_AWAKE) {
		if (!pixel.CheckState(PIXEL_AWAKE)) {
			tile.AddAwake();
			pixel.SetBit(PIXEL_AWAKE);
			//WakeNeighbors(pixelIndex);
		}
	}
	else {
		if (pixel.CheckState(PIXEL_AWAKE)) {
			pixel.ClearBit(PIXEL_AWAKE);
			tile.RemoveAwake();
		}
	}

	pixels[CoordToIndex(c)].SetValueAndColor(value,color);
	tile.UpdateRenderBounds(c);

	tile.MarkDirty();
}

void Raster::WakeNeighbors(const int index) {
	bool top = AtTopBound(index);
	bool right = AtRightBound(index);
	bool bottom = AtBottomBound(index);
	bool left = AtLeftBound(index);
	bool topright = top || right;
	bool bottomright = bottom || right;
	bool bottomleft = bottom || left;
	bool topleft = top || left;

	if (!top) {
		int tempIndex = GetTop(index);
		Pixel& temp = pixels[tempIndex];
		if (!temp.IsWakable()) {
			temp.SetBit(PIXEL_AWAKE_UPDATED);
			GetTileFromRasterIndex(tempIndex).AddAwake();
		}
	}
	if (!topright) {
		int tempIndex = GetTopRight(index);
		Pixel& temp = pixels[tempIndex];
		if (!temp.IsWakable()) {
			temp.SetBit(PIXEL_AWAKE_UPDATED);
			GetTileFromRasterIndex(tempIndex).AddAwake();
		}
	}
	if (!right) {
		int tempIndex = GetRight(index);
		Pixel& temp = pixels[tempIndex];
		if (!temp.IsWakable()) {
			temp.SetBit(PIXEL_AWAKE_UPDATED);
			GetTileFromRasterIndex(tempIndex).AddAwake();
		}
	}
	if (!bottomright) {
		int tempIndex = GetBottomRight(index);
		Pixel& temp = pixels[tempIndex];
		if (!temp.IsWakable()) {
			temp.SetBit(PIXEL_AWAKE_UPDATED);
			GetTileFromRasterIndex(tempIndex).AddAwake();
		}
	}
	if (!bottom) {
		int tempIndex = GetBottom(index);
		Pixel& temp = pixels[tempIndex];
		if (!temp.IsWakable()) {
			temp.SetBit(PIXEL_AWAKE_UPDATED);
			GetTileFromRasterIndex(tempIndex).AddAwake();
		}
	}
	if (!bottomleft) {
		int tempIndex = GetBottomLeft(index);
		Pixel& temp = pixels[tempIndex];
		if (!temp.IsWakable()) {
			temp.SetBit(PIXEL_AWAKE_UPDATED);
			GetTileFromRasterIndex(tempIndex).AddAwake();
		}
	}
	if (!left) {
		int tempIndex = GetLeft(index);
		Pixel& temp = pixels[tempIndex];
		if (!temp.IsWakable()) {
			temp.SetBit(PIXEL_AWAKE_UPDATED);
			GetTileFromRasterIndex(tempIndex).AddAwake();
		}
	}
	if (!topleft) {
		int tempIndex = GetTopLeft(index);
		Pixel& temp = pixels[tempIndex];
		if (!temp.IsWakable()) {
			temp.SetBit(PIXEL_AWAKE_UPDATED);
			GetTileFromRasterIndex(tempIndex).AddAwake();
		}
	}
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

void Raster::MarkTileDirty(coord& c) {
	GetTile(c).MarkDirty();
}

void Raster::ResetPixelUpdatedState() {
	for (size_t i = 0; i < tileCount; i++) {
		Tile& tile = tiles[i];
		if (tile.awakePixels > 0) {
			for (size_t x = tile.tileBounds.x1; x < tile.tileBounds.x2; x++) {
				for (size_t y = tile.tileBounds.y1; y < tile.tileBounds.y2; y++) {
					int index = y * context->RASTER_WIDTH + x;
					pixels[index].ClearBit(PIXEL_UPDATED);
				}
			}
		}
	}
}


void Raster::CleanTiles() {
	for (size_t i = 0; i < tileCount; i++) {
		Tile& tile = tiles[i];
		tile.dirty = false;
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

void Raster::SwapPixels(Tile& tile1, Tile& tile2, Pixel& pixel1, Pixel& pixel2, const int index1, const int index2) {
	coord c1 = { index1 % context->RASTER_WIDTH, index1 / context->RASTER_WIDTH};
	tile1.UpdateRenderBounds(c1);
	
	coord c2 = { index2 % context->RASTER_WIDTH, index2 / context->RASTER_WIDTH };
	tile2.UpdateRenderBounds(c2);

	pixel1.SetBit(PIXEL_UPDATED);
	pixel2.SetBit(PIXEL_UPDATED);

	//WakeNeighbors(index1);
	//WakeNeighbors(index2);

	if (tile1.startIndex != tile2.startIndex) {
		if (pixel2.CheckState(PIXEL_EMPTY)) {
			tile1.occupiedPixels--;
			tile2.occupiedPixels++;
			tile1.RemoveAwake();
			tile2.AddAwake();
		}
		else if (!pixel2.CheckState(PIXEL_AWAKE)) {
			tile2.AddAwake();
		}
	}

	SwapPixelValues(pixels[index1], pixels[index2]);
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
	return index % context->RASTER_WIDTH < context->RASTER_WIDTH - 1;
}

bool Raster::AtBottomBound(const int index) {
	return index < context->TOTAL_PIXELS - context->RASTER_WIDTH;
}

bool Raster::AtLeftBound(const int index) {
	return index % context->RASTER_WIDTH > 0;
}


