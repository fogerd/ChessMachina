/******************************************************************************
*
*	FILE - HASH.CPP
*	PURPORSE - Generate a unique key for each possible board position.
* 
*	AUTHOR - Dennis Fogerty
*	DATE - 4/2/2016
*
******************************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "hash.h"
#include "board.h"
#include "defs.h"


Hash::Hash() {

}

U64 Hash::Rand64() {
	U64 random = \
		(U64)rand() | \
		(U64)rand() << 15 | \
		(U64)rand() << 30 | \
		(U64)rand() << 45 | \
		((U64)rand() & 0xf) << 60;
	return random;
}

U64 Hash::GeneratePosKey(const Board *game_board) {
	int square = 0;
	U64 final_key = 0;
	int piece = EMPTY;
	
	// pieces
	for(square = 0; square < BOARD_SQUARE_NUMBER; ++square) {
		piece = game_board->pieces[square];
		if(piece!=NO_SQUARE && piece!=EMPTY && piece!=OFFBOARD) {
			final_key ^= PieceKeys[piece][square];
		}		
	}
	
	if(game_board->side_to_move == WHITE) {
		final_key ^= SideKey;
	}
		
	if(game_board->en_passant_square != NO_SQUARE) {
		assert (game_board->en_passant_square>=0 && game_board->en_passant_square<BOARD_SQUARE_NUMBER);
		final_key ^= PieceKeys[EMPTY][game_board->en_passant_square];
	}
	
	assert (game_board->castle_permission>=0 && game_board->castle_permission<=15);
	
	final_key ^= CastleKeys[game_board->castle_permission];
	
	return final_key;
}

