#include "Physics.h"

Physics::Physics(Context* context) : context(context) {
    SetupTiles();
};

void Physics::Tick(double deltaTime) {
    CleanTiles();
    for (size_t i = context->TOTAL_PIXELS - 1; i != SIZE_MAX; i--) {
    //for (size_t i = 0; i < context->TOTAL_PIXELS; i++) {
        ParseSand(i);
    }
}

void Physics::SetupTiles() {
    int desiredTileSize = 40;
    

    int horizontalDivider = 
    tileColumns = context->VIEW_WIDTH / desiredTileSize;
    tileRows = context->VIEW_HEIGHT / desiredTileSize;

    tileWidth = context->VIEW_WIDTH / tileColumns;
    tileHeight = context->VIEW_HEIGHT / tileRows;
    tileCount = tileColumns * tileRows;

    tiles = new Tile[tileCount];
}

void Physics::CleanTiles() {
    for (size_t i = 0; i < tileCount; i++) {
        tiles[i].MarkClean();
    }
}

inline bool Physics::ParseSand(size_t index) {
    Pixel& pixel = context->raster->GetPixel(index);
    if (pixel.CheckState(PIXEL_EXISTS_DYNAMIC)) {
        if (!AtBottomBound(index)) {
            Pixel& pixel_below = context->raster->GetPixel(index + context->VIEW_WIDTH);
            if (!pixel_below.CheckState(PIXEL_EXISTS)) {
                MarkTileDirty(index);
                MarkTileDirty(index + context->VIEW_WIDTH);
                SwapPixels(pixel, pixel_below);
            }
            else {
                Pixel& pixel_bottom_left = context->raster->GetPixel(index + context->VIEW_WIDTH - 1);
                Pixel& pixel_bottom_right = context->raster->GetPixel(index + context->VIEW_WIDTH + 1);
                if (pixel_bottom_left.CheckState(PIXEL_EXISTS) && pixel_bottom_right.CheckState(PIXEL_EXISTS)) {
                    pixel.SetAwake(false);
                }
                else {
                    if (!pixel_bottom_left.CheckState(PIXEL_EXISTS) && !pixel_bottom_right.CheckState(PIXEL_EXISTS) && !AtLeftBound(index) && !AtRightBound(index)) {
                        if (std::rand() % 2 == 0) {
                            MarkTileDirty(index);
                            MarkTileDirty(index + context->VIEW_WIDTH - 1);
                            SwapPixels(pixel, pixel_bottom_left);
                        }
                        else {
                            MarkTileDirty(index);
                            MarkTileDirty(index + context->VIEW_WIDTH + 1);
                            SwapPixels(pixel, pixel_bottom_right);
                        }
                    }
                    else {
                        if (!pixel_bottom_left.CheckState(PIXEL_EXISTS) && !AtLeftBound(index)) {
                            MarkTileDirty(index);
                            MarkTileDirty(index + context->VIEW_WIDTH - 1);
                            SwapPixels(pixel, pixel_bottom_left);
                        }
                        else if (!pixel_bottom_right.CheckState(PIXEL_EXISTS) && !AtRightBound(index)) {
                            MarkTileDirty(index);
                            MarkTileDirty(index + context->VIEW_WIDTH + 1);
                            SwapPixels(pixel, pixel_bottom_right);
                        }
                        else {
                            pixel.SetAwake(false);
                        }
                    }
                }
            }
        }
        else {
            pixel.SetAwake(false);
        }
    }
    return false;
};

void Physics::SwapPixels(Pixel& a, Pixel& b) {
    swapPixel = a;
    a = b;
    b = swapPixel;
}

bool Physics::AtBounds(size_t index) {
    return  AtLeftBound(index) || AtRightBound(index) || AtTopBound(index) || AtBottomBound(index);
}

bool Physics::AtLeftBound(size_t index) {
    return (index % context->VIEW_WIDTH) == 0;
}

bool Physics::AtRightBound(size_t index) {
    return (index % (context->VIEW_WIDTH)) == context->VIEW_WIDTH - 1;
}

bool Physics::AtTopBound(size_t index) {
    return (index < context->VIEW_WIDTH);
}

bool Physics::AtBottomBound(size_t index) {
    return (index >= (context->TOTAL_PIXELS - context->VIEW_WIDTH));
}

int Physics::GetTileIndex(int pixelIndex) {
    int x = (pixelIndex % context->VIEW_WIDTH) / tileWidth;
    int y = (pixelIndex / context->VIEW_WIDTH) / tileHeight;
    return (y * tileColumns) + x;
}

void Physics::MarkTileDirty(int pixelIndex) {
    tiles[GetTileIndex(pixelIndex)].MarkDirty();
}