#include "defs.h"

// Indexes used for checking piece type, value & color

/* 
	Pieces 
	{
		EMPTY_SQ, 
		WHITE_PAWN, WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, WHITE_KING,
		BLACK_PAWN, BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, BLACK_KING
	}
*/

int BigPiecePostions[13] = 
	{ 
		false, 
		false, true, true, true, true, true, 
		false, true, true, true, true, true 
	};

int MajorPiecePostions[13] = 
	{ 
		false, 
		false, false, false, true, true, true, 
		false, false, false, true, true, true 
	};

int MinorPiecePostions[13] = 
	{ 
		false, 
		false, true, true, false, false, false, 
		false, true, true, false, false, false 
	};

int PieceValues[13] = 
	{ 
		0, 
		100, 325, 325, 550, 1000, 50000, 
		100, 325, 325, 550, 1000, 50000  
	};

int PieceColor[13] = 
	{ 
		BOTH, 
		WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
		BLACK, BLACK, BLACK, BLACK, BLACK, BLACK 
	};

int PieceIsPawn[13] = 
	{ 
		false, 
		true, false, false, false, false, false, 
		true, false, false, false, false, false 
	};	

int PieceIsKnight[13] = 
	{ 
		false, 
		false, true, false, false, false, false,
		false, true, false, false, false, false 
	};

int PieceIsKing[13] = 
	{ 
		false, 
		false, false, false, false, false, true,
		false, false, false, false, false, true 
	};

int PieceIsQueenRook[13] = 
	{ 
		false, 
		false, false, false, true, true, false, 
		false, false, false, true, true, false 
	};

int PieceIsQueenBishop[13] = 
	{ 
		false, 
		false, false, true, false, true, false, 
		false, false, true, false, true, false 
	};

int PieceIsSlider[13] =
	{
		false,
		false, false, true, true, true, false,
		false, false, true, true, true, false
	};



