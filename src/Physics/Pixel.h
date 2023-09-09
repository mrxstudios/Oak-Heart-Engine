#pragma once

#include "Engine/Types.h"
#include "Renderer/Palette.h"
#include <iostream>

#define PIXEL_EMPTY 0
#define PIXEL_EXISTS 1
#define PIXEL_AWAKE 2
#define PIXEL_EXISTS_AWAKE 3
#define PIXEL_UPDATED 4
#define PIXEL_AWAKE_UPDATED 6
#define PIXEL_DYNAMIC 8
#define PIXEL_EXISTS_DYNAMIC 9
#define PIXEL_EXISTS_AWAKE_DYNAMIC 11

struct Pixel {
	uint8_t bitmask1 = 0;
	uint8_t colorId = 0;

	/*
	* 00000000
	*     duae
	* e: Exists
	* a: Is Awake
	* u: Updated
	* d: Is Dynamic
	*/

	inline void SetExists(bool state) {
		state ? SetBit(PIXEL_EXISTS) : ClearBit(PIXEL_EXISTS);
	}

	inline void SetAwake(bool state) {
		state ? SetBit(PIXEL_AWAKE) : ClearBit(PIXEL_AWAKE);
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

	inline bool IsNotSwappable() {
		return ((CheckState(PIXEL_EXISTS) && !CheckState(PIXEL_AWAKE)));
	}

	inline bool IsWakable() {
		return (!CheckState(PIXEL_AWAKE_UPDATED) && CheckState(PIXEL_EXISTS_DYNAMIC));
	}
};