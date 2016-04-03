/*
*	FILE - BOARD.H
*	PURPORSE - Game Board object, contains all information regarding the game.
 * 
*	AUTHOR - Dennis Fogerty
*	DATE - 3/17/2016
*/


#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include "defs.h"

class Board {
	public:
		Board();
		void PrintBoard();
		void ResetBoard();
		void InitSquare120To64();
		void AddPiece(int square);
		int CountBits(U64 bitboard);
		int PopBit(U64 *bitboard);
		int side_to_move;
		int en_passant_square;
		int fifty_move_count;
		int castle_permission;
		int pieces[BOARD_SQUARE_NUMBER];
	protected:
		U64 Bitboard;

		// Move counter. Ply = 1 move by 1 side
		int ply_count;
		int ply_history;

		// Unique key generated for each possible board position
		U64 postion_key;

		int num_of_pieces[13];
		
		U64 pawns[3];

		int king_square_location[2];		
		int big_pieces_location[3];		// All pieces except pawns
		int major_pieces_location[3]; 	// Rooks & Queens
		int minor_pieces_location[3];	// Bishops & Knights
		
		// E.X.
		// piece_list[WHITE_BISHOP][0] = C1
		// piece_list[WHITE_BISHOP][1] = F1
		int piece_list[13][10];

		int square120_to_square64[BOARD_SQUARE_NUMBER];
		int square64_to_square120[64];

		UndoMove move_history[2048];

};


#endif