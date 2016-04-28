/*
*	FILE - SQUARE_ATTACKED.CPP
*	PURPORSE - 
 * 
*	AUTHOR - Dennis Fogerty
*	DATE - 4/23/2016
*/

#include "defs.h"
#include "board.h"

// Rules for which squares a piece attacks from its current position
const int KnightDirections[8] = { -8, -19, -21, -12, 8, 19, 21, 12 };
const int RookDirections[4]   = { -1, -10, 1, 10 };
const int BishopDirections[4] = { -9, -11, 11, 9 };
const int KingDirections[8]   = { -1, -10, 1, 10, -9, -11, 11, 9 };


// Takes a game board, a board square and the side to move
// Looks at all squares which if a certain piece exists,
// the current square is being attacked
int IsSquareAttacked(const int square, const int side, Board *board) {

	int piece;
	int index;
	int tmp_square;
	int direction;

	// PAWNS
	if (side == WHITE) {
		if (board->pieces[square-11] == WHITE_PAWN || board->pieces[square-9] == WHITE_PAWN) 
			return true;
	}	
	else {
		if (board->pieces[square+11] == BLACK_PAWN || board->pieces[square+9] == BLACK_PAWN)
			return true;
	}

	// KNIGHTS
	for (index = 0; index < 8; ++index) {
		piece = board->pieces[square + KnightDirections[index]];
		if (PieceIsKnight[piece] && PieceColor[piece] == side)
			return true;
	}
	
	// QUEENS & ROOKS
	for (index = 0; index < 4; ++index) {
		direction = RookDirections[index];
		tmp_square = square + direction;
		piece = board->pieces[tmp_square];

		while (piece != OFFBOARD) {
			if (piece != EMPTY) {
				if (PieceIsQueenRook[piece] && PieceColor[piece] == side) {
					return true;
				}
				break;
			}
			tmp_square += direction;
			piece = board->pieces[tmp_square];
		}
	}

	// QUEENS & BISHOPS
	for (index = 0; index < 4; ++index) {
		direction = BishopDirections[index];
		tmp_square = square + direction;
		piece = board->pieces[tmp_square];

		while (piece != OFFBOARD) {
			if (piece != EMPTY) {
				if (PieceIsQueenBishop[piece] && PieceColor[piece] == side) {
					return true;
				}
				break;
			}
			tmp_square += direction;
			piece = board->pieces[tmp_square];
		}
	}

	// KING
	for (int index = 0; index < 8; ++index)	{
		piece = board->pieces[square + KingDirections[index]];
		if (PieceIsKing[piece] && PieceColor[piece] == side)
			return true;
	}

	return 0;
}