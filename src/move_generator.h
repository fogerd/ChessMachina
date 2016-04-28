/*
*	FILE - MOVE_GENERATOR.H
*	PURPORSE -	PURPORSE - Object for generating all possible moves for a given board position
* 
*	AUTHOR - Dennis Fogerty
*	DATE - 4/23/2016
*/


#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include <iostream>
#include "defs.h"

class Board;

class MoveGenerator {
	public:
		MoveGenerator();
		static void AddQuietMove(const Board *gameboard, int move, MOVELIST *move_list );
		static void AddCaptureMove(const Board *gameboard, int move, MOVELIST *move_list );
		static void AddEnPassantMove(const Board *gameboard, int move, MOVELIST *move_list );
		
		void GenerateAllMoves(Board *gameboard, MOVELIST *move_list);
		void GenerateAllPawnMoves(Board *gameboard, MOVELIST *move_list);
		void GenerateAllSlideMoves(Board *gameboard, MOVELIST *move_list);
		void GenerateAllNonSlideMoves(Board *gameboard, MOVELIST *move_list);

		static void AddWhitePawnCapture(const Board *gameboard, const int from_sq, const int to_sq, const int capture, MOVELIST *move_list);
		static void AddWhitePawnMove(const Board *gameboard, const int from_sq, const int to_sq, MOVELIST *move_list);
		static void AddBlackPawnCapture(const Board *gameboard, const int from_sq, const int to_sq, const int capture, MOVELIST *move_list);
		static void AddBlackPawnMove(const Board *gameboard, const int from_sq, const int to_sq, MOVELIST *move_list);

	private:
		const int sliding_pieces_index[2] = {0,4};
		const int sliding_pieces[8] = {
			WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, 0, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, 0
		};

		const int non_sliding_pieces_index[2] = {0,3};
		const int non_sliding_pieces[6] = {
			WHITE_KNIGHT, WHITE_KING, 0, BLACK_KNIGHT, BLACK_KING, 0
		};

		// Arrays used to know which directions a piece can move in
		// Pawns are handled sepeartlys
		const int number_of_directions_per_piece[13] = {
			0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8
		};
		const int piece_direction[13][8] = {
			{ 0, 0, 0, 0, 0, 0, 0 },                // EMPTY
			{ 0, 0, 0, 0, 0, 0, 0 },                // WHITE_PAWNS
			{ -8, -19,	-21, -12, 8, 19, 21, 12 },  // WHITE_KNIGHT
			{ -9, -11, 11, 9, 0, 0, 0, 0 },         // WHITE_BISHOP
			{ -1, -10,	1, 10, 0, 0, 0, 0 },        // WHITE_ROOK
			{ -1, -10,	1, 10, -9, -11, 11, 9 },    // WHITE_QUEEN
			{ -1, -10,	1, 10, -9, -11, 11, 9 },    // WHITE_KING
			{ 0, 0, 0, 0, 0, 0, 0 },                // BLACK_PAWNS
			{ -8, -19,	-21, -12, 8, 19, 21, 12 },  // BLACK_KNIGHT
			{ -9, -11, 11, 9, 0, 0, 0, 0 },         // BLACK_BISHOP
			{ -1, -10,	1, 10, 0, 0, 0, 0 },        // BLACK_ROOK
			{ -1, -10,	1, 10, -9, -11, 11, 9 },    // BLACK_QUEEN
			{ -1, -10,	1, 10, -9, -11, 11, 9 }     // BLACK_KING
		};

		int piece = EMPTY;
		int side;
		int square = 0;
		int tmp_square = 0;
		int piece_number = 0;
		int direction = 0;
		int index = 0;
		int piece_index = 0;
};


#endif
