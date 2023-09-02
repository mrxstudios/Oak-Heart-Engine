#pragma once

#include "Engine/Context.h"

class Context;

class Tile
{
public:
	int x;
	int y;
	bool dirty;

	Tile();
	//Tile(int x, int y);

	void MarkDirty();
	void MarkClean();
	bool IsDirty();
};

