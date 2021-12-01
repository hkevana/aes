#include "FFA.h"
#include <stdio.h>
#include <cstdlib>

// Finite Field Arithmetic .cpp

FFA::FFA() {}
FFA::~FFA() {}

uint8_t FFA::ffAdd(uint8_t n1, uint8_t n2) { return (n1 ^ n2); }
uint8_t FFA::xtime(uint8_t num) {
	short shift = num << 1;
	return ((shift & 0x100) == 0x100) ? (shift ^ 0x11b) : (uint8_t)shift;
}
uint8_t FFA::ffMultiply(uint8_t num, uint8_t bitset) {
	uint8_t res = num;
	uint8_t out = 0x00;

	for (short x = 0x01; x < 0x100; x <<= 1) {
		if ((bitset & x) == x) { out ^= res; }
		res = xtime(res);
	}

	return out;
}
