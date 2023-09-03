#pragma once

#include <iostream>

#define PIXEL_EXISTS 0
#define PIXEL_AWAKE 1
#define PIXEL_UPDATED 2
#define PIXEL_DYNAMIC 3

struct Pixel {
	uint32_t bitmask = 0;

	/*
	* 0000000 00000000 00000000 00000000
	*                               duae
	* e: Exists
	* a: Is Awake
	* u: Updated
	* d: Is Dynamic
	*/

	inline bool Exists() {
		return BitIsSet(PIXEL_EXISTS);
	}

	inline bool IsAwake() {
		return BitIsSet(PIXEL_AWAKE);
	}

	inline bool IsUpdated() {
		return BitIsSet(PIXEL_UPDATED);
	}

	inline bool IsDynamic() {
		return BitIsSet(PIXEL_DYNAMIC);
	}


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