/*
*	FILE - BITBOARD.CPP
*	PURPORSE - Helper functions for bitboards
 * 
*	AUTHOR - Dennis Fogerty
*	DATE - 4/1/2016
*/

#include <stdio.h>
#include "defs.h"


const int BitTable[64] = {
	63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
	51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
	26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
	58, 20, 37, 17, 36, 8
};

// Take 1st least sign first bit, returns index and sets bit to 0 
int PopBit(U64 *bitboard) {
	U64 b = *bitboard ^ (*bitboard - 1);
	unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
	*bitboard &= (*bitboard - 1);
	return BitTable[(fold * 0x789a9b23) >>  26];
}

// Counts and returns number of bits that are 1 in a bitboard
int CountBits(U64 bitboard) {
	int count;
	for ( count = 0; bitboard; count++, bitboard &= bitboard-1 );
	return count;
}

void PrintBitBoard(U64 bitboard) {

	U64 bitshift = 1ULL;
	
	int rank = 0;
	int file = 0;
	int square120 = 0;
	int square64 = 0;
	
	printf("\n");
	for(rank = RANK_8; rank >= RANK_1; --rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			square120 = CONVERT_FILE_AND_RANK_TO_SQUARE(file,rank);		
			square64 = SQ120_TO_SQ64(square120); 
			
			if((bitshift << square64) & bitboard) 
				printf("X");
			else 
				printf("-");
		}
		printf("\n");
	}
    printf("\n\n");
}
