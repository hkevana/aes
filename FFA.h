#ifndef FF_ARITHMETIC_H
#define FF_ARITHMETIC_H

#include <stdint.h>

// Finite Field Arithmetic .h
class FFA {

	public:
		FFA();
		~FFA();

		static uint8_t ffAdd(uint8_t n1, uint8_t n2);
		static uint8_t xtime(uint8_t num);
		static uint8_t ffMultiply(uint8_t num, uint8_t bitset);

};

#endif

