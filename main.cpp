#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <string>

#include "aes.h"
#include "flags.h"

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#define VS_MEM_CHECK
#endif

using namespace std;

void usage(char *executable) {
	cout << endl << "usage: " << executable << " [-v] [-e] [-d] [-o] msg key" ;
	cout << endl << "\t-v, -verbose: verbose output";
	cout << endl << "\t-e, -encrypt: encrypt message only";
	cout << endl << "\t-d, -decrypt: decrypt message only";
	cout << endl << "\t-o, -output: output to a file";
	cout << endl << "\t-msg: 128 bit hexadecimal char sequence";
	cout << endl << "\t-key: 128/192/256 bit hexadecimal char sequence";
	cout << endl;
}

void printStream(ostream& out, uint8_t *stream, int size, const char *prepend) {
	out << endl << left << setfill(' ') << setw(20) << prepend;

	for (int i = 0; i < (size*4); i++) {
		out << right << setfill('0') << setw(2) << hex << int(stream[i]);
	}
}

uint8_t charToByte(char c) {
	switch (c) {
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case 54:
		case 55:
		case 56:
		case 57:
			return (int)c - 48;
		case 97:
		case 98:
		case 99:
		case 100:
		case 101:
		case 102:
			return (int)c - 87;
		default:
			string error = "input must be hexadecimal\nunrecognized character: ";
			error += c;
			throw string(error);
	}
}
void charStreamToByteStream(char *charStream, uint8_t *byteStream, int size) {
	for (int i = 0; i < (size*8); i += 2) {
		uint8_t nibble_1 = charToByte(charStream[i]);
		uint8_t nibble_2 = charToByte(charStream[i + 1]);
		uint8_t byte = (nibble_1 << 4) ^ nibble_2;
		byteStream[i / 2] = byte;
	}
}

string genFileName(int Nk) {
	string filename = "out/out__.txt";

	switch (Nk) {
	case 4:
		filename.insert(8, "128");
		break;
	case 6:
		filename.insert(8, "192");
		break;
	case 8:
		filename.insert(8, "256");
		break;
	default:
		cerr << endl <<"unrecognized key length: " << Nk;
		return NULL;
	}
	if (flags::decrypt) { filename.insert(12, "d"); }
	if (flags::encrypt) { filename.insert(12, "e"); }
	return filename;
}
int findLength(char *charStream) {
	int i = 0;
	while (charStream[i] != '\0') { i++; }
	return (i / 8);
}
bool set_flag(char f) {
	switch (f) {
		case 'v':
			flags::verbose = true;
			break;
		case 'e':
			flags::encrypt = true;
			flags::decrypt = false;
			break;
		case 'd':
			flags::encrypt = false;
			flags::decrypt = true;
			break;
		case 'o':
			flags::outfile = true;
			break;
		default:
			return false;
	}
	return true;
}

bool validate_cmd_line(int argc, char **argv, int *Nb, int *Nk, int *msg_i) {
	int i = 1;
	while (i < argc && argv[i][0] == '-') { 
		set_flag(argv[i][1]);
		i++; 
	}
	*msg_i = i;
	if (*msg_i + 2 != argc) { return false; }

	*Nb = findLength(argv[*msg_i]);
	if (*Nb != 4) { return false; }

	*Nk = findLength(argv[*msg_i + 1]);
	if (!(*Nk == 4 || *Nk == 6 || *Nk == 8)) { return false; }

	return true;
}

int main(int argc, char** argv) {
	// ENABLE MEMORY LEAK CHECKING
	VS_MEM_CHECK;

	int Nb, Nk, Nr = 0;
	int msg_i = 1;

	// validate command line arguments
	if (argc < 3 || !validate_cmd_line(argc, argv, &Nb, &Nk, &msg_i)) { 
		usage(argv[0]); 
		return 1;
	}
	Nr = Nk + 6;

	// redirect output
	ofstream outFile;
	if (flags::outfile) {
		string filename = genFileName(Nk);
		outFile.open(filename);
		if (!outFile.is_open()) {
			cerr << endl << "unable to open file: " << filename;
			cerr << endl << "redirecting output to stdout...";
		}
	}
	ostream& output = (outFile.is_open()) ? outFile : cout;

	// initialize holders 
	uint8_t in[16]{ 0x00 };
	uint8_t out[16]{ 0x00 };
	uint8_t inv[16]{ 0x00 };

	uint8_t *key = new uint8_t[Nk*4];
	uint32_t *w = new uint32_t[Nb*(Nr + 1)];

	try {

		// convert cmd line args to byte arrays
		charStreamToByteStream(argv[msg_i], in, Nb);
		charStreamToByteStream(argv[msg_i], out, Nb);
		charStreamToByteStream(argv[msg_i+1], key, Nk);

		printStream(output, in, Nb, "PLAINTEXT:");
		printStream(output, key, Nk, "KEY:");

		// begin encryption
		Aes aes(Nb, Nk, Nr);
		aes.keyExpansion(key, w);

		if (flags::encrypt) {
			output << endl << endl << "CIPHER (ENCRYPT):";
			aes.cipher(output, in, out, w);
		}
		if (flags::decrypt) {
			output << endl << endl << "INVERSE CIPHER (DECRYPT):";
			aes.invCipher(output, out, inv, w);
		}

	} catch (string& e) {
		cerr << endl << e << endl;
		usage(argv[0]);
	}

	delete[] key;
	delete[] w;

	output << endl;

	outFile.close();


	return 0;
}
