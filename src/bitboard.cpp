/*****************************************************************************
*
*	FILE - BITBOARD.CPP
*	PURPORSE - Helper functions for 64-Bit Bitboards
*	
*	Bitboards are used to represent the game board with 64 bits,
*	where each bit represents a corresponding square on the board.
*	
*	For a given Bit / Square: 
*		Bit = 1, piece exists
*		Bit = 0, no piece
*
*  	For Example, the Bitboard for all PAWNS at the start of the game is:
*
*		00000000
*		11111111
*		00000000
*		00000000
*		00000000
*		00000000
*		11111111
*		00000000
*
*	
*	READING - 
*	[1] https://chessprogramming.wikispaces.com/Bitboards
*	[2] https://chessprogramming.wikispaces.com/General+Setwise+Operations
*
*	AUTHOR - Dennis Fogerty
*	DATE - 4/1/2016
*
******************************************************************************/

#include <iostream>
#include <stdio.h>
#include "defs.h"


// Matt Taylor's Folding trick : 
// https://chessprogramming.wikispaces.com/Matt+Taylor
//
// Used by PopBit() to find bit-index of Least Significant Bit in a Bitboard.
const int lsb_64_table[64] = 
{
	63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
	51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
	26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
	58, 20, 37, 17, 36, 8
};

// Find Least Significant Bit (LSB) of given bitboard, 
// set bit to 0 and return bit's index.
int PopBit(U64 *bitboard) {
  U64 bb = *bitboard ^ (*bitboard - 1);
  unsigned int fold = (unsigned) ((bb & 0xffffffff) ^ (bb >> 32));
  *bitboard &= (*bitboard - 1);
  return lsb_64_table[(fold * 0x783a9b23) >> 26];
}

// Iterate through the given Bitboard and use the Bitwise AND operation 
// to count the number of bits = 1 for the given bitboard. 
// Return the total count.
int CountBits(U64 bitboard) {
	int count;
	for (count = 0; bitboard; bitboard &= bitboard-1) {
		count++;
	}
	return count;
}

// Outputs Bitboard with X's representing bits turned on
void PrintBitBoard(U64 bitboard) {

	U64 bitshift = 1ULL;
	
	int rank = 0;
	int file = 0;
	int square120 = 0;
	int square64 = 0;
	
	printf("\n");
	for (rank = RANK_8; rank >= RANK_1; --rank) {
		for (file = FILE_A; file <= FILE_H; ++file) {
			square120 = CONVERT_FILE_AND_RANK_TO_SQUARE(file,rank);		
			square64 = SQ64_TO_SQ120(square120); 
			
			if ((bitshift << square64) & bitboard) 
				printf("X");
			else 
				printf("-");
		}
		printf("\n");
	}
    printf("\n\n");
}
