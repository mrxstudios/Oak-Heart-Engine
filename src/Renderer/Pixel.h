#pragma once

#include <iostream>

#define PIXEL_EXISTS 0
#define PIXEL_DYNAMIC 1
#define PIXEL_AWAKE 2

struct Pixel {
	uint32_t bitmask = 7;

	/*
	* 0000000 00000000 00000000 00000000
	*                                ase
	* e: Exists
	* s: Is Dynamic
	* a: Is Awake
	*/

	inline bool Exists() {
		return BitIsSet(PIXEL_EXISTS);
	}

	inline bool IsDynamic() {
		return BitIsSet(PIXEL_DYNAMIC);
	}

	inline bool IsAwake() {
		return BitIsSet(PIXEL_AWAKE);
	}

	inline void SetExists(bool state) {
		state ? SetBit(PIXEL_EXISTS) : ClearBit(PIXEL_EXISTS);
	}

	inline void SetDynamic(bool state) {
		state ? SetBit(PIXEL_DYNAMIC) : ClearBit(PIXEL_DYNAMIC);
	}
	inline void SetAwake(bool state) {
		state ? SetBit(PIXEL_AWAKE) : ClearBit(PIXEL_AWAKE);
	}

	inline void SetValue(int value) {
		bitmask = value;
	}

	inline void SetBit(int n) {
		bitmask |= 1UL << n;
	}
	inline void ClearBit(int n) {
		bitmask &= ~(1UL << n);
	}

	inline bool BitIsSet(int n) {
		return bitmask & (1 << n);
	}
};