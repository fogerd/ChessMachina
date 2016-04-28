/******************************************************************************
*
*	FILE - PERFT_TESTING.H
*	
*	PURPORSE - 
*	PERFT (PERFormance Test, move path enumeration)
*	
*	Debugging class that tests the Move Generation of the program.
*	Walks through the move generation tree of legal moves to count all leaf nodes
*	at a specified depth.
*
*	Results are compared to standard, accepted values to locate and isolate bugs
*	in either the move generation or the board representation.
* 
*	READING -
*	https://chessprogramming.wikispaces.com/Perft		
*	https://chessprogramming.wikispaces.com/Perft+Results
*		
*
*	AUTHOR - Dennis Fogerty
*	DATE - 4/25/2016
*
******************************************************************************/

#ifndef PERFT_TESTING_H
#define PERFT_TESTING_H

#include "defs.h"


class Perft {
	public:
		void RecursivePerft(int depth, Board *gameboard, MoveGenerator MoveGen);
		void PerformPerftTest(int depth, Board *gameboard, MoveGenerator MoveGen);

	private:
		long number_of_leaf_nodes;
};

#endif