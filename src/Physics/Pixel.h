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

#define PIXEL_SAND_EXISTS_DYNAMIC 21
#define PIXEL_WATER_EXISTS_DYNAMIC 37


struct Pixel {
	uint8_t bitmask1 = 0;
	uint8_t vector = 0;
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

	inline bool CheckMaskedValue(int mask, int value) {
		int maskedA = bitmask1 & mask;
		int maskedB = value & mask;
		return maskedA == maskedB;
	}

	inline void SetVector(uint8_t x, uint8_t y) {
		SetVectorX(x);
		SetVectorY(y);
	}

	// Set the lower 4 bits of vector VALUE RANGE [-8,7]
	inline void SetVectorX(int value) {
		vector = (vector & 0xF0) | (value & 0x0F);
	}

	// Set the higher 4 bits of vector VALUE RANGE [-8,7]
	inline void SetVectorY(int value) {
		vector = (vector & 0x0F) | ((value << 4) & 0xF0);
	}

	// Get the lower 4 bits as a signed integer
	inline int getVectorX() {
		int value = vector & 0x0F;
		return (value > 7) ? value - 16 : value;
	}

	// Get the higher 4 bits as a signed integer
	inline int getVectorY() {
		int value = (vector >> 4) & 0x0F;
		return (value > 7) ? value - 16 : value;
	}
};