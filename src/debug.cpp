/******************************************************************************
*
*	FILE - DEBUG.CPP
*	
*	PURPORSE - 
*	Functions to help debug and assert that the program is 
*	not creating illegal board positions or invalid moves.
* 
*	AUTHOR - Dennis Fogerty
*	DATE - 4/25/2016
*
******************************************************************************/

#include "defs.h"
#include "board.h"


bool ValidMoveList(const MOVELIST *move_list,  const Board *gameboard) {
	
	if(move_list->move_count < 0 || move_list->move_count >= MAXMOVEPOSITIONS) {
		return false;
	}

	int from_square = 0;
	int to_square = 0;

	for (int move_count = 0; move_count < move_list->move_count; ++move_count) {
		
		to_square = TOSQUARE(move_list->moves[move_count].move);
		from_square = FROMSQUARE(move_list->moves[move_count].move);
		
		if (!SquareIsOnBoard(to_square) || !SquareIsOnBoard(from_square)) return false;
		
		if (!ValidPiece(gameboard->pieces[from_square])) return false;
	}

	return true;
}

// 
bool SqIs120(const int square) {
	return (square >= 0 && square < 120);
}

bool PceValidEmptyOffbrd(const int pce) {
	return (ValidPieceOrEmpty(pce) || pce == OFFBOARD);
}

bool SquareIsOnBoard(const int square) {
	return file_of[square] != OFFBOARD;
}

bool ValidSide(const int side) {
	return (side==WHITE || side==BLACK);
}

bool ValidPieceOrEmpty(const int piece) {
	return (piece>=EMPTY && piece<=BLACK_KING);
}

bool ValidPiece(const int piece) {
	return (piece>=WHITE_PAWN && piece<=BLACK_KING);
}