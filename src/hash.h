/*
*	FILE - HASH.CPP
*	PURPORSE - Generate a unique key for each possible board position.
* 
*	AUTHOR - Dennis Fogerty
*	DATE - 4/2/2016
*/

#ifndef HASH_H
#define HASH_H

#include "defs.h"

class Board;

class Hash {
	public:
		Hash();
		U64 GeneratePosKey(const Board *postion_key);
		U64 Rand64();
	private:
		U64 PieceKeys[13][120];
		U64 SideKey;
		U64 CastleKeys[16];
};

#endif