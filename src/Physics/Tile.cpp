#include "Tile.h"

Tile::Tile(int x, int y, int w, int h, int startIndex, coord& leftCorner) :
x(x), y(y), w(w), h(h), startIndex(startIndex), leftCorner(leftCorner)
{
	updateBounds = { 0,0,0,0 };
	tileBounds = {
		leftCorner.x,
		leftCorner.y,
		leftCorner.x + w,
		leftCorner.y + h
	};
	totalPixels = w * h;
	occupiedPixels = 0;
	shouldRender = true;
	shouldStep = false;

	for (size_t i = 0; i < 8; i++) {
		neighbourIndices[i] = -1;
	}
};

void Tile::UpdateRenderBounds(coord& c) {
	if (updateBounds.x1 == 0 && updateBounds.x2 == 0 && updateBounds.y1 == 0 && updateBounds.y2 == 0) {
		updateBounds = { c.x,c.y,c.x,c.y };
		return;
	}
	if (c.x < updateBounds.x1) updateBounds.x1 = c.x;
	if (c.x > updateBounds.x2) updateBounds.x2 = c.x;
	if (c.y < updateBounds.y1) updateBounds.y1 = c.y;
	if (c.y > updateBounds.y2) updateBounds.y2 = c.y;
}