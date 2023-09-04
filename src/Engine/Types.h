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

struct Point {
	int x;
	int y;
};

enum TextAlignment {
	Left,
	Center,
	Right
};