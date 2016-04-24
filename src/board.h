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

class Hash;

class Board {
	public:
		Board();
		void PrintBoard();
		void ResetBoard();
		void UpdateMaterial();
		int ParseFEN(char *FEN, Hash HashGenerator);
		void InitSquare120To64();
		void AddPiece(int square);
		int ValidBoard();
		int CountBits(U64 bitboard);
		int PopBit(U64 *bitboard);
		int side_to_move;
		int en_passant_square;
		int fifty_move_count;
		int castle_permission;
		int pieces[BOARD_SQUARE_NUMBER];
	//protected:
		U64 Bitboard;

		// Move counter. Ply = 1 move by 1 side
		int ply_count;
		int ply_history;

		// Unique key generated for each possible board position
		U64 postion_key;

		int piece_number[13];
		
		U64 pawns[3];

		int king_square_location[2];	

		int big_pieces_count[2];  	// All pieces except pawns
		int major_pieces_count[2];	// Rooks & Queens
		int minor_pieces_count[2];	// Bishops & Knights
		int material_score[2];
		
		// E.X.
		// piece_list[WHITE_BISHOP][0] = C1
		// piece_list[WHITE_BISHOP][1] = F1
		int piece_list[13][10];

		int square120_to_square64[BOARD_SQUARE_NUMBER];
		int square64_to_square120[64];

		UNDOMOVE move_history[2048];

};


#endif