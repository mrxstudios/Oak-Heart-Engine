#include "Physics.h"

Physics::Physics(Context* context) : context(context) {
    moveCount = 0;
};

void Physics::Tick(double deltaTime) {
    Raster& raster = *context->raster;
    raster.ResetTilesAndPixels();

    parseForward = !parseForward;

    for (size_t i = 0; i < raster.tileCount; i++) {
        Tile& tile = raster.GetTile(i);

        if (tile.shouldStep) {
            for (size_t y = tile.tileBounds.y1; y < tile.tileBounds.y2; y++) {
                if (parseForward) {
                    // forward
                    for (size_t x = tile.tileBounds.x1; x < tile.tileBounds.x2; x++) {
                        ParsePixel(raster,tile, x, y);
                    }
                }
                else {
                    // backward
                    for (size_t x = tile.tileBounds.x2 - 1; x != tile.tileBounds.x1 - 1; x--) {
                        ParsePixel(raster, tile, x, y);
                    }
                }
            }
        }
    }
}

void Physics::ParsePixel(Raster& raster, Tile& tile, const int x, const int y) {
    int index = y * context->RASTER_WIDTH + x;
    Pixel& pixel = raster.GetPixel(index);

    if (!pixel.CheckState(PIXEL_UPDATED)) {
        coord c = { x,y };
        if (pixel.CheckState(PIXEL_SAND_EXISTS_DYNAMIC)) {
            ParseSand(raster, tile, pixel, c, index);
        }
        else if (pixel.CheckState(PIXEL_WATER_EXISTS_DYNAMIC)) {
            ParseWater(raster, tile, pixel, c, index);
        }
    }
}

void Physics::ParseColumn(Raster& raster, bounds& b) {
    int width = b.x2 - b.x1;
    for (size_t y = b.y1; y < b.y2; y++) {
        for (size_t x = b.x1; y < b.x2; x++) {
            coord c = { x,y };
            int index = raster.CoordToIndex(c);
            Tile& tile = raster.GetTile(c);
            Pixel& pixel = raster.GetPixel(index);
            ParseSand(raster, tile, pixel, c, index);
        }
    }
}

inline void Physics::ParseSand(Raster& raster, Tile& tile, Pixel& pixel, coord& c, const int index) {
    if (!AtBottomBound(c)) {
        moveCount = 0;

        // Move down
        int bottomIndex = raster.GetBottom(index);
        Pixel& bottomPixel = raster.GetPixel(bottomIndex);
        if (!bottomPixel.CheckState(PIXEL_EXISTS) || bottomPixel.CheckState(PIXEL_TYPE_WATER)) {
            raster.SwapPixels(tile, pixel, c, c.moveDown());
            return;
        }

        coord cdl = c.moveDownLeft();
        coord cdr = c.moveDownRight();
        Pixel& bottomLeftPixel = raster.GetPixel(cdl);
        Pixel& bottomRightPixel = raster.GetPixel(cdr);

        // Move left down
        if ((!bottomLeftPixel.CheckState(PIXEL_EXISTS) || bottomPixel.CheckState(PIXEL_TYPE_WATER)) && !AtLeftBound(c)) {
            AddMove(cdl);
        }

        // Move down right
        if ((!bottomRightPixel.CheckState(PIXEL_EXISTS) || bottomPixel.CheckState(PIXEL_TYPE_WATER)) && !AtRightBound(c)) {
            AddMove(cdr);
        }

        if(moveCount > 0) {
            int randomMove = std::rand() % moveCount;
            raster.SwapPixels(tile, pixel, c, moves[randomMove]);
        }

    }
};

inline void Physics::ParseWater(Raster& raster, Tile& tile, Pixel& pixel, coord& c, const int index) {
    bool atBottomBound = AtBottomBound(c);

    moveCount = 0;

    // Move down
    Pixel& bottomPixel = raster.GetPixel(c.moveDown());

    if (!bottomPixel.CheckState(PIXEL_EXISTS) && !atBottomBound) {
        raster.SwapPixels(tile, pixel, c, c.moveDown());
        return;
    }

    coord cdl = c.moveDownLeft();
    coord cdr = c.moveDownRight();
    Pixel& bottomLeftPixel = raster.GetPixel(cdl);
    Pixel& bottomRightPixel = raster.GetPixel(cdr);

    bool atLeftBound = AtLeftBound(c);
    bool atRightBound = AtRightBound(c);
    bool bottomLeftExists = bottomLeftPixel.CheckState(PIXEL_EXISTS);
    bool bottomRightExists = bottomRightPixel.CheckState(PIXEL_EXISTS);

    bool falling = false;

    // Move left down
    if (!bottomLeftExists && !atLeftBound && !atBottomBound) {
        AddMove(cdl);
        falling = true;
    }

    // Move down right
    if (!bottomRightExists && !atRightBound && !atBottomBound) {
        AddMove(cdr);
        falling = true;
    }

    if (!falling) {
        coord cl = c.moveLeft();
        coord cr = c.moveRight();
        Pixel& leftPixel = raster.GetPixel(cl);
        Pixel& rightPixel = raster.GetPixel(cr);
        bool leftExists = leftPixel.CheckState(PIXEL_EXISTS);
        bool rightExists = rightPixel.CheckState(PIXEL_EXISTS);

        // Move left
        if (
            !leftExists &&
            bottomLeftExists &&
            !atLeftBound
        ) {
            AddMove(cl);
        }

        // Move right
        if (
            !rightExists &&
            bottomRightExists &&
            !atRightBound
        ) {
            AddMove(cr);
        }
    }

    if (moveCount > 0) {
        int randomMove = std::rand() % moveCount;
        raster.SwapPixels(tile, pixel, c, moves[randomMove]);
    }
};

bool Physics::PixelCanSlide(coord c, Pixel& bottomLeft, Pixel& bottomRight) {
    if (
        (bottomLeft.CheckState(PIXEL_EXISTS) || AtLeftBound(c)) &&
        (bottomRight.CheckState(PIXEL_EXISTS) || AtRightBound(c))
    ) {
        return false;
    }
    return true;
}

bool Physics::PixelCanFloat(coord c, Pixel& left, Pixel& right) {
    if (
        (left.CheckState(PIXEL_EXISTS) || AtLeftBound(c)) &&
        (right.CheckState(PIXEL_EXISTS) || AtRightBound(c))
        ) {
        return false;
    }
    return true;
}

void Physics::AddMove(coord c) {
    moves[moveCount] = c;
    moveCount++;
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