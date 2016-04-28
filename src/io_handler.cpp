/*
*	FILE - IO_HANDLER.CPP
*	PURPORSE - Input command function implementations
 * 
*	AUTHOR - Dennis Fogerty
*	DATE - 4/3/2016
*/

#include <utility>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "board.h"
#include "io_handler.h"


// Input loop that will be always running
// If input doesn't match command, assume it is a move
// quit / q - quit program
// p 		- print game board
int InputHandler::IO_Loop(Board GameBoard) {
	while(true) {
		printf("ChessMachina> ");
		scanf ("%s",input);

		if (!strcmp(input, "quit") || !strcmp(input, "q")) {
			return EXIT_SUCCESS;
		}
		if (!strcmp(input, "help") || !strcmp(input, "?")) {
			printf("quit - exit the program\n");
			printf("p - display the game board\n");
		}
		if (!strcmp(input, "p")) {
			GameBoard.PrintBoard();
		}
		else {
			ParseMove(input, GameBoard);
			printf("\n");
		}
	}
}


/* 
* Takes move input from player (E.X. e2e4) 
* Returns the FROM and TO square as a pair of ints
* Returns (-1,-1) if move is invalid
*/
std::pair<int,int> InputHandler::ParseMove(char *move_input, Board GameBoard) {
	std::pair <int,int> from_to_pair;

	from_to_pair = std::make_pair(-1,-1);
	if (move_input[0] < 'a' || move_input [0] > 'h') return from_to_pair;
	if (move_input[1] < '1' || move_input [1] > '8') return from_to_pair;
	if (move_input[2] < 'a' || move_input [2] > 'h') return from_to_pair;
	if (move_input[3] < '1' || move_input [3] > '8') return from_to_pair;

	int from_square = CONVERT_FILE_AND_RANK_TO_SQUARE(move_input[0] - 'a', move_input[1] - '1');
    int to_square = CONVERT_FILE_AND_RANK_TO_SQUARE(move_input[2] - 'a', move_input[3] - '1');

	from_to_pair = std::make_pair(from_square,to_square);
	return from_to_pair;
}
