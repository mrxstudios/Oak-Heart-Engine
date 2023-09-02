#include "Tile.h"

Tile::Tile() {};

//Tile::Tile(int x, int y) : x(x), y(y) {};

void Tile::MarkClean() {
	dirty = false;
}

void Tile::MarkDirty() {
	dirty = true;
}

bool Tile::IsDirty() {
	return dirty;
}