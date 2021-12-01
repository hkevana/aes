CC = g++
CFLAGS = -g -Wall
AES = ./aes
TEXT = "00112233445566778899aabbccddeeff"
KEY_128 = "000102030405060708090a0b0c0d0e0f"
KEY_192 = "000102030405060708090a0b0c0d0e0f1011121314151617"
KEY_256 = "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"

all: compile

FFA.o: FFA.cpp FFA.h
	$(CC) $(CFLAGS) -c FFA.cpp

flags.o: flags.cpp flags.h
	$(CC) $(CFLAGS) -c flags.cpp

aes.o: aes.cpp aes.h
	$(CC) $(CFLAGS) -c aes.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

compile: main.o aes.o flags.o FFA.o
	$(CC) $(CFLAGS) -o aes main.o aes.o flags.o FFA.o

run: compile
	$(AES) -v -o $(TEXT) $(KEY_128)
	$(AES) -v -o $(TEXT) $(KEY_192)
	$(AES) -v -o $(TEXT) $(KEY_256)

clean:
	rm -f aes ./*.o ./out/*
