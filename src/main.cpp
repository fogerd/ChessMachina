/*
*	FILE - MAIN.CPP
*	PURPORSE - Set-up all necessary components of program and wait for user input
* 
*	AUTHOR - Dennis Fogerty
*	DATE - 3/14/2016
*/

#include <iostream>
#include "defs.h"
#include "board.h"
#include "hash.h"
#include "move_generator.h"
#define PERFT "3rk2r/8/8/8/8/8/6p1/R3K2R w KQk - 0 1"


int main() {

	std::cout << ("\n");
	std::cout << ENGINE_NAME << std::endl;
	std::cout << ("RPI / SD&D / SPRING 2016\n");
	std::cout << ("\n");
	std::cout << ("Type \"help\" or \"?\" to display a list of commands.\n");
	std::cout << ("\n");

	InitAll();
	Hash HashGenerator;
	Board b1[1];
	b1->ParseFEN(PERFT, HashGenerator);
	b1->PrintBoard();
	
	MOVELIST list[1];

	MoveGenerator MoveGen;
	MoveGen.GenerateAllMoves(b1, list);

	PrintMoveList(list);

	return 0;
}