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

void Board::ResetBoard() {
	int index = 0;

	for(index = 0; index < BOARD_SQUARE_NUMBER; ++index) {
		pieces[index] = OFFBOARD;
	}

	for(index = 0; index < 64; ++index) {
		pieces[SQ120_TO_SQ64(index)] = EMPTY;
	}

	for(index = 0; index < 3; ++index) {
		big_pieces_location[index] = 0;
		major_pieces_location[index] = 0;
		minor_pieces_location[index] = 0;
		pawns[index] = 0ULL;
	}

	for(index = 0; index < 13; ++index) {
		num_of_pieces[index] = 0;
	}

	king_square_location[WHITE] = king_square_location[BLACK] = NO_SQUARE;

	side_to_move = BOTH;
	en_passant_square = NO_SQUARE;
	fifty_move_count = 0;

	ply_count = 0;
	ply_history = 0;

	castle_permission = 0;

	postion_key = 0ULL;
}


void Board::AddPiece(int square) {
	Bitboard |= (1ULL << square120_to_square64[square]);
}
