//
// Created by james on 4/14/16.
//

#include "naive_move_scorer.h"

//Assign a move a score value, positive values are good for white, negative values are good for black.
//This is a stupid little implementation that only values taking high value pieces.
double Naive_Move_Scorer::score_move(const Move& move){
	Pieces taken=move.taken_piece;
	switch (taken){
		case WHITE_PAWN:
			return 1.0;
		case WHITE_KNIGHT:
			return 3.0;
		case WHITE_BISHOP:
			return 3.0;
		case WHITE_ROOK:
			return 5.0;
		case WHITE_QUEEN:
			return 9.0;
		case WHITE_KING:
			return 100.0;
		case BLACK_PAWN:
			return -1.0;
		case BLACK_KNIGHT:
			return -3.0;
		case BLACK_BISHOP:
			return -3.0;
		case BLACK_ROOK:
			return -5.0;
		case BLACK_QUEEN:
			return -9.0;
		case BLACK_KING:
			return -100.0;
		default:
			return 0.0;
	}
}