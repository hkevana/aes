#ifndef AES_H
#define AES_H

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

class Aes {

	private:

		static uint32_t Rcon[52];

		static uint8_t matrix_a[4][4];
		static uint8_t Sbox[16][16];

		static uint8_t matrix_inv_a[4][4];
		static uint8_t InvSbox[16][16];

		int Nb;
		int Nk;
		int Nr;

		uint8_t matrixMultiply(uint8_t row[4], uint8_t column[4]);
		uint8_t subByte(uint8_t byte);
		uint8_t invSubByte(uint8_t byte);

		void printRound(ostream& out, int round, const char *stage);
		void printState(ostream& out, uint8_t state[4][4], int round, const char *stage);
		void printSchedule(ostream& out, uint32_t k_sch[4], int round, const char *stage);

		uint32_t *subWord(uint32_t *word);
		uint32_t *rotWord(uint32_t *word);

		void subBytes(uint8_t state[4][4]);
		void shiftRows(uint8_t state[4][4]);
		void mixColumns(uint8_t state[4][4]);

		void invSubBytes(uint8_t state[4][4]);
		void invShiftRows(uint8_t state[4][4]);
		void invMixColumns(uint8_t state[4][4]);

		void addRoundKey(uint8_t state[4][4], uint32_t *Rkey);

	public:

		Aes(int b, int k, int r) : Nb(b), Nk(k), Nr(r) {}
		~Aes() {}

		void keyExpansion(uint8_t key[16], uint32_t *w);
		void cipher(ostream& output, uint8_t in[16], uint8_t out[16], uint32_t *dw);
		void invCipher(ostream& output, uint8_t in[16], uint8_t out[16], uint32_t *dw);
};
#endif

