#pragma once

#include "Engine/Types.h"
#include "Renderer/Palette.h"
#include <iostream>

#define PIXEL_EMPTY 0
#define PIXEL_EXISTS 1
#define PIXEL_UPDATED 2
#define PIXEL_EXISTS_UPDATED 3
#define PIXEL_DYNAMIC 4
#define PIXEL_EXISTS_DYNAMIC 5
#define PIXEL_UPDATED_DYNAMIC 6
#define PIXEL_EXISTS_UPDATED_DYNAMIC 7
#define PIXEL_TYPE_SAND 16
#define PIXEL_TYPE_WATER 32


struct Pixel {
	uint8_t bitmask1 = 0;
	uint8_t colorId = 0;

	/*
	* 00000000
	* tttt due
	* e: Exists
	* u: Updated
	* d: Is Dynamic
	* t: type
	*/

	inline void SetExists(bool state) {
		state ? SetBit(PIXEL_EXISTS) : ClearBit(PIXEL_EXISTS);
	}

	inline void SetUpdated(bool state) {
		state ? SetBit(PIXEL_UPDATED) : ClearBit(PIXEL_UPDATED);
	}

	inline void SetDynamic(bool state) {
		state ? SetBit(PIXEL_DYNAMIC) : ClearBit(PIXEL_DYNAMIC);
	}

	inline void SetValueAndColor(int value, uint8_t color) {
		bitmask1 = value;
		colorId = color;
	}

	inline void SetValue(int value) {
		bitmask1 = value;
	}

	inline uint8_t GetColor() {
		return colorId;
	}

	inline void SetColor(uint8_t id) {
		colorId = id;
	}

	inline void SetBit(int n) {
		bitmask1 |= n;
	}
	inline void ClearBit(int n) {
		bitmask1 &= ~(n);
	}

	inline bool CheckState(int n) {
		return (bitmask1 & n) == n;
	}
};