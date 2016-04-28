/******************************************************************************
*
*	FILE - INIT.CPP
*	PURPORSE - Run at the start of the program to initialize needed systems
* 
*	AUTHOR - Dennis Fogerty
*	DATE - 4/2/2016
*
******************************************************************************/

#include <stdlib.h>
#include "defs.h"
#include <iostream>

#define RAND_64 	((U64)rand() | \
					(U64)rand() << 15 | \
					(U64)rand() << 30 | \
					(U64)rand() << 45 | \
					((U64)rand() & 0xf) << 60 )

int square120_to_square64[BOARD_SQUARE_NUMBER];
int square64_to_square120[64];

U64 SetMask[64];
U64 ClearMask[64];

U64 PieceKeys[13][120];
U64 SideKey;
U64 CastleKeys[16];

// Keep track of while File / Rank a given square is in
int file_of[BOARD_SQUARE_NUMBER];
int rank_of[BOARD_SQUARE_NUMBER];

void InitFilesAndRanks() {
	int index = 0;
	int file = FILE_A;
	int rank = RANK_1;
	int square = A1;

	for(index = 0; index < BOARD_SQUARE_NUMBER; ++index) {
		file_of[index] = OFFBOARD;
		rank_of[index] = OFFBOARD;
	}

	for(rank = RANK_1; rank <= RANK_8; ++rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			square = CONVERT_FILE_AND_RANK_TO_SQUARE(file,rank);
			file_of[square] = file;
			rank_of[square] = rank;
		}
	}
}


void InitBitMasks() {
	int index = 0;

	for (index = 0; index < 64; index++) {
		SetMask[index] = 0ULL;
		ClearMask[index] = 0ULL;
	}

	for (index = 0; index < 64; index++) {
		SetMask[index] |= (1ULL << index);
		ClearMask[index] = ~SetMask[index];
	}
}

// Convert 120 square board to 64 square board
void InitSquare120To64() {

	int index = 0;
	int file = FILE_A;
	int rank = RANK_1;
	int square120 = A1;
	int square64 = 0;
	for(index = 0; index < BOARD_SQUARE_NUMBER; ++index) {
		square120_to_square64[index] = 65;
	}

	for(index = 0; index < 64; ++index) {
		square64_to_square120[index] = 120;
	}

	for(rank = RANK_1; rank <= RANK_8; ++rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			square120 = CONVERT_FILE_AND_RANK_TO_SQUARE(file,rank);
			square64_to_square120[square64] = square120;
			square120_to_square64[square120] = square64;
			square64++;
		}
	}
}

void InitHashKeys() {

	int index = 0;
	int index2 = 0;
	for(index = 0; index < 13; ++index) {
		for(index2 = 0; index2 < 120; ++index2) {
			PieceKeys[index][index2] = RAND_64;
		}
	}
	SideKey = RAND_64;
	for(index = 0; index < 16; ++index) {
		CastleKeys[index] = RAND_64;
	}

}


void InitAll() {
	InitSquare120To64();
	InitBitMasks();
	InitHashKeys();
	InitFilesAndRanks();
}