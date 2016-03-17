/*
*	FILE - BOARD.H
*	PURPORSE - Game Board object, contains all information regarding the game.
 * 
*	AUTHOR - Dennis Fogerty
*	DATE - 3/17/2016
*/

#include <iostream>
#include "defs.h"

class Board {
	public:
		Board();
		void InitSquare120To64();
		void PrintBoard();
		void AddPiece(int square);
		inline int convert_file_and_rank_to_square(int file, int rank) 
			{return (21 + (file)) + ((rank) * 10); }
	private:
		int pieces[BOARD_SQUARE_NUMBER];
		U64 Bitboard;

		/*
			EX - 
			piece_list[WHITE_BISHOP][0] = C4
		*/
		int piece_list[13][10];
		U64 pawns[3];

		int side_to_move;
		int en_passant_square;
		int fifty_move_count;
		int castle_permission;

		// Move counter. Ply = 1 move by 1 side
		int ply_count;
		int ply_history;

		// Unique key generated for each possible board position
		U64 postion_key;

		int num_of_pieces[13];
		
		int king_square_location[2];		
		int big_pieces_location[3];		// All pieces except pawns
		int major_pieces_location[3]; 	// Rooks & Queens
		int minor_pieces_location[3];	// Bishops & Knights

		int square120_to_square64[BOARD_SQUARE_NUMBER];
		int square64_to_square120[64];


		UndoMove move_history[2048];

};