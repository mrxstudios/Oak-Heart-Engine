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
};

struct bounds {
	int x1;
	int y1;
	int x2;
	int y2;
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