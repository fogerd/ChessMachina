/*
*	FILE - HASH.CPP
*	PURPORSE - Generate a unique key for each possible board position.
* 
*	AUTHOR - Dennis Fogerty
*	DATE - 4/2/2016
*/

#include <stdlib.h>
#include <assert.h>
#include "hash.h"
#include "board.h"
#include "defs.h"


Hash::Hash() {
	for(int index = 0; index < 13; ++index) {
		for(int index2 = 0; index2 < 120; ++index2) {
			PieceKeys[index][index2] = Rand64();
		}
	}
	SideKey = Rand64();
	for(int index = 0; index < 16; ++index) {
		CastleKeys[index] = Rand64();
	}
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

U64 Hash::GeneratePosKey(const Board *postion_key) {
	int square = 0;
	U64 final_key = 0;
	int piece = EMPTY;
	
	// pieces
	for(square = 0; square < BOARD_SQUARE_NUMBER; ++square) {
		piece = postion_key->pieces[square];
		if(piece!=NO_SQUARE && piece!=EMPTY && piece!=OFFBOARD) {
			final_key ^= PieceKeys[piece][square];
		}		
	}
	
	if(postion_key->side_to_move == WHITE) {
		final_key ^= SideKey;
	}
		
	if(postion_key->en_passant_square != NO_SQUARE) {
		assert (postion_key->en_passant_square>=0 && postion_key->en_passant_square<BOARD_SQUARE_NUMBER);
		final_key ^= PieceKeys[EMPTY][postion_key->en_passant_square];
	}
	
	assert (postion_key->castle_permission>=0 && postion_key->castle_permission<=15);
	
	final_key ^= CastleKeys[postion_key->castle_permission];
	
	return final_key;
}

