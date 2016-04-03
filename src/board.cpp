/*
*	FILE - BOARD.CPP
*	PURPORSE - Game Board object function implementations.
 * 
*	AUTHOR - Dennis Fogerty
*	DATE - 3/17/2016
*/

#include "defs.h"
#include "board.h"

Board::Board() {
	U64 Bitboard = 0ULL;
	InitSquare120To64();
}

// Convert 120 square board to 64 squares
void Board::InitSquare120To64() {
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

void Board::PrintBoard() {
	U64 bitshift = 1ULL;
	int rank = 0;
	int file = 0;
	int square120 = 0;
	int square64 = 0;

	std::cout << "\n";
	for (rank = RANK_8; rank >= RANK_1; rank--) {
		for(file=FILE_A; file <= FILE_H; file++){
			square120 = CONVERT_FILE_AND_RANK_TO_SQUARE(file, rank);
			square64 = square120_to_square64[square120];
			if ((bitshift << square64) & (Bitboard))
				std::cout << "X";
			else
				std::cout << "-";
		}
		std::cout << "\n";
	}
}

void Board::AddPiece(int square) {
	Bitboard |= (1ULL << square120_to_square64[square]);
}
