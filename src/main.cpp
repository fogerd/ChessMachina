/******************************************************************************
*
*	FILE - MAIN.CPP
*	PURPORSE - Set-up all necessary components of program and wait for user input
* 
*	AUTHOR - Dennis Fogerty
*	DATE - 3/14/2016
*
******************************************************************************/

#include <iostream>
#include <stdio.h>
#include <stdlib.h> 

#include "defs.h"
#include "board.h"
#include "hash.h"
#include "move_generator.h"
#include "perft_testing.h"

#define PERFT "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

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
	b1->ParseFEN(START_FEN, HashGenerator);
	//b1->PrintBoard();
	
	MOVELIST list[1];

	Perft Perft_Tester;

	// b2b3
	//MakeMove(b1, 5408);
	//MakeMove(b1, 10072);
	//std::cout << SQ120_TO_SQ64(b1->king_square_location[0]) << std::endl;

	MoveGenerator MoveGen;
	//MoveGen.GenerateAllMoves(b1, list);

	//PrintMoveList(list);
	//Perft_Tester.PerformPerftTest(4, b1, MoveGen);

	char input[6];
	int Move = NOMOVE;
	MOVELIST move_list[1];
	int size = 0;

	// MAIN LOOP
	while (true) {
		b1->PrintBoard();
		printf("Enter move : ");
		fgets(input, 6, stdin);

		if (input[0] == 'q') {
			break;
		}
		else if (input[0] == 't') {
			TakeMove(b1);
		}
		else {
			Move = ParseMove(input, b1, MoveGen);
			std::cout << Move << std::endl;
			if (Move != NOMOVE) {
				MakeMove(b1, Move);
				MoveGen.GenerateAllMoves(b1, move_list);
				size = move_list->move_count;
				int random_move = rand() % size;
				MakeMove(b1,move_list->moves[random_move].move);
			}
		}

		fflush(stdin);
	}

	return 0;
}