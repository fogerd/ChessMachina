/******************************************************************************
*
*	FILE - HASH.H
*	PURPORSE - Generate a unique key for each possible board position.
* 
*	Used to detect equivalent boards repetition detection and for pruning
*	search trees.
*
*	AUTHOR - Dennis Fogerty
*	DATE - 4/2/2016
*
******************************************************************************/

#ifndef HASH_H
#define HASH_H

#include "defs.h"

class Board;

class Hash {
	public:
		Hash();
		U64 GeneratePosKey(const Board *position_key);
		U64 Rand64();
};

#endif