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
