#pragma once

#include <stdint.h>

struct ARGB {
	uint_fast8_t A;
	uint_fast8_t R;
	uint_fast8_t G;
	uint_fast8_t B;

	void darken() {
		R = R >> 1;
		G = G >> 1;
		B = B >> 1;
	}
};

struct coord {
	int x;
	int y;

	inline coord moveUp() const { return coord{ x, y - 1 }; }
	inline coord moveDown() const { return coord{ x, y + 1 }; }
	inline coord moveRight() const { return coord{ x + 1, y }; }
	inline coord moveLeft() const { return coord{ x - 1, y }; }
	inline coord moveUpRight() const { return coord{ x + 1, y - 1 }; }
	inline coord moveDownRight() const { return coord{ x + 1, y + 1 }; }
	inline coord moveDownLeft() const { return coord{ x - 1, y + 1 }; }
	inline coord moveUpLeft() const { return coord{ x - 1, y - 1 }; }
};

struct bounds {
	int x1;
	int y1;
	int x2;
	int y2;
};

struct neighbours {
	int t	= -1;	// top
	int tr	= -1;	// top-right
	int r	= -1;	// right
	int br	= -1;	// bottom-right
	int b	= -1;	// bottom
	int bl	= -1;	// bottom-left
	int l	= -1;	// left
	int tl	= -1;	// top-left
};

struct Point {
	int x;
	int y;
};

enum TextAlignment {
	Left,
	Center,
	Right
};