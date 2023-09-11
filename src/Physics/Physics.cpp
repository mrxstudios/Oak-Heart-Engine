#include "Physics.h"

Physics::Physics(Context* context) : context(context) {
    //SetupTiles();
};

void Physics::Tick(double deltaTime) {
    Raster& raster = *context->raster;
    raster.ResetTilesAndPixels();

    for (size_t i = 0; i < raster.tileCount; i++) {
        Tile& tile = raster.GetTile(i);
        if (tile.shouldStep) {
            for (size_t y = tile.tileBounds.y1; y < tile.tileBounds.y2; y++) {
                for (size_t x = tile.tileBounds.x1; x < tile.tileBounds.x2; x++) {
                    coord c = { x,y };
                    ParseSand(raster, tile, c);
                }
            }
        }
    }
}

void Physics::ParseColumn(Raster& raster, bounds& b) {
    int width = b.x2 - b.x1;
    for (size_t y = b.y1; y < b.y2; y++) {
        for (size_t x = b.x1; y < b.x2; x++) {
            coord c = { x,y };
            Tile& tile = raster.GetTile(c);
            ParseSand(raster, tile, c);
        }
    }
}

inline bool Physics::ParseSand(Raster& raster, Tile& tile, coord& c) {
    int index = raster.CoordToIndex(c);
    Pixel& pixel = raster.GetPixel(index);
    if (!pixel.CheckState(PIXEL_UPDATED)) {
        if (pixel.CheckState(PIXEL_EXISTS_DYNAMIC)) {
            if (!AtBottomBound(c)) {
                int bottomIndex = raster.GetBottom(index);
                Pixel& bottomPixel = raster.GetPixel(bottomIndex);

                // Move down
                if (!bottomPixel.CheckState(PIXEL_EXISTS)) {
                    Tile& tile2 = context->raster->GetTileFromRasterIndex(bottomIndex);
                    raster.SwapPixels(tile,tile2,pixel, bottomPixel, index, bottomIndex);
                }
                else {
                    int bottomLeftIndex = raster.GetBottomLeft(index);
                    Pixel& bottomLeftPixel = raster.GetPixel(bottomLeftIndex);
                    int bottomRightIndex = raster.GetBottomRight(index);
                    Pixel& bottomRightPixel = raster.GetPixel(bottomRightIndex);

                    // Can't fall
                    if (SandCanFall(c, bottomLeftPixel, bottomPixel, bottomRightPixel)) {
                        // Move left or right
                        if (
                            !bottomLeftPixel.CheckState(PIXEL_EXISTS) && 
                            !bottomRightPixel.CheckState(PIXEL_EXISTS) && 
                            !AtLeftBound(c) && 
                            !AtRightBound(c)
                        ) {
                            if (std::rand() % 2 == 0) {
                                Tile& tile2 = context->raster->GetTileFromRasterIndex(bottomLeftIndex);
                                raster.SwapPixels(tile,tile2,pixel,bottomLeftPixel,index, bottomLeftIndex);
                            }
                            else {
                                Tile& tile2 = context->raster->GetTileFromRasterIndex(bottomRightIndex);
                                raster.SwapPixels(tile,tile2,pixel,bottomRightPixel,index, bottomRightIndex);
                            }
                        }
                        else {
                            if (!bottomLeftPixel.CheckState(PIXEL_EXISTS)) {
                                Tile& tile2 = context->raster->GetTileFromRasterIndex(bottomLeftIndex);
                                raster.SwapPixels(tile, tile2, pixel, bottomLeftPixel, index, bottomLeftIndex);
                            }
                            else {
                                Tile& tile2 = context->raster->GetTileFromRasterIndex(bottomRightIndex);
                                raster.SwapPixels(tile, tile2, pixel, bottomRightPixel, index, bottomRightIndex);
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
};

bool Physics::SandCanFall(coord c, Pixel& bottomLeft, Pixel& bottom, Pixel& bottomRight) {
    if (
        (bottomLeft.CheckState(PIXEL_EXISTS) || AtLeftBound(c)) &&
        (bottom.CheckState(PIXEL_EXISTS) || AtBottomBound(c)) &&
        (bottomRight.CheckState(PIXEL_EXISTS) || AtRightBound(c))
    ) {
        return false;
    }
    return true;
}

bool Physics::AtBounds(coord& c) {
    return  AtLeftBound(c) || AtRightBound(c) || AtTopBound(c) || AtBottomBound(c);
}

bool Physics::AtLeftBound(coord& c) {
    return c.x == 0;
}

bool Physics::AtRightBound(coord& c) {
    return c.x == context->RASTER_WIDTH - 1;
}

bool Physics::AtTopBound(coord& c) {
    return c.y == 0;
}

bool Physics::AtBottomBound(coord& c) {
    return c.y == context->RASTER_HEIGHT - 1;
}