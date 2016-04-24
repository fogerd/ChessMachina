/*
*	FILE - IO_HANDLER.CPP
*	PURPORSE - Handle Input / Output
 * 
*	AUTHOR - Dennis Fogerty
*	DATE - 4/23/2016
*/

#include <stdio.h>
#include "defs.h"
#include <iostream>

char * PrintSquare(const int square) {
	static char SquareString[3];
	int file = file_of[square];
	int rank = rank_of[square];

	sprintf(SquareString, "%c%c", ('a' + file), ('1' + rank));

	return SquareString;
}

char * PrintMove(const int move) {
	static char MoveString[6];
	int from_file = file_of[FROMSQUARE(move)];
	int from_rank = rank_of[FROMSQUARE(move)];
	int to_file = file_of[TOSQUARE(move)]; 
	int to_rank = rank_of[TOSQUARE(move)];
	int promoted = PROMOTED(move);

	if (promoted) {
		char piece_char = 'q';
		if (PieceIsKing(promoted)) {
			piece_char = 'n';
		} 
		else if(PieceIsQueenRook(promoted) && !PieceIsQueenBishop(promoted))  {
			piece_char = 'r';
		} 
		else if(!PieceIsQueenRook(promoted) && PieceIsQueenBishop(promoted))  {
			piece_char = 'b';
		}
		sprintf(MoveString, "%c%c%c%c%c", ('a'+from_file), ('1'+from_rank), ('a'+to_file), ('1'+to_rank), piece_char);
	} 
	else {
		sprintf(MoveString, "%c%c%c%c", ('a'+from_file), ('1'+from_rank), ('a'+to_file), ('1'+to_rank));
	}

	return MoveString;
}

void PrintMoveList(const MOVELIST *move_list) {
	int index = 0;
	int score = 0;
	int move = 0;
	printf("Move List:\n");

	for(index = 0; index < move_list->move_count; ++index) {
		move = move_list->moves[index].move;
		score = move_list->moves[index].score;
		printf("Move: %d > %s (score:%d)\n", index+1, PrintMove(move), score);
	}
	
	printf("Move List Total %d Moves:\n\n", move_list->move_count);
}
