/******************************************************************************
*
*	FILE - IO_HANDLER.CPP
*	PURPORSE - Handle Input / Output
* 
*	AUTHOR - Dennis Fogerty
*	DATE - 4/23/2016
*
******************************************************************************/

#include <stdio.h>
#include <iostream>
#include "defs.h"
#include "board.h"
#include "move_generator.h"


int ParseMove(char * input_move, Board *gameboard, MoveGenerator MoveGen) {

	//assert(gameboard->ValidBoard());

	if(input_move[1] > '8' || input_move[1] < '1') return NOMOVE;
    if(input_move[3] > '8' || input_move[3] < '1') return NOMOVE;
    if(input_move[0] > 'h' || input_move[0] < 'a') return NOMOVE;
    if(input_move[2] > 'h' || input_move[2] < 'a') return NOMOVE;

    int from_square = CONVERT_FILE_AND_RANK_TO_SQUARE(input_move[0] - 'a', input_move[1] - '1');
    int to_square = CONVERT_FILE_AND_RANK_TO_SQUARE(input_move[2] - 'a', input_move[3] - '1');

	//assert(SqOnBoard(from_square) && SqOnBoard(to_square));

	MOVELIST move_list[1];
    MoveGen.GenerateAllMoves(gameboard,move_list);
    int move_count = 0;
	int current_move = 0;
	int promoted_piece = EMPTY;

	for (move_count = 0; move_count < move_list->move_count; ++move_count) {
		current_move = move_list->moves[move_count].move;
		if (FROMSQUARE(current_move)==from_square && TOSQUARE(current_move)==to_square) {
			promoted_piece = PROMOTED(current_move);
			if (promoted_piece!=EMPTY) {
				if (PieceIsQueenRook(promoted_piece) && !PieceIsQueenBishop(promoted_piece) && input_move[4]=='r') {
					return current_move;
				} 
				else if (!PieceIsQueenRook(promoted_piece) && PieceIsQueenBishop(promoted_piece) && input_move[4]=='b') {
					return current_move;
				} 
				else if (PieceIsQueenRook(promoted_piece) && PieceIsQueenBishop(promoted_piece) && input_move[4]=='q') {
					return current_move;
				} 
				else if (PieceIsKing(promoted_piece)&& input_move[4]=='n') {
					return current_move;
				}
				continue;
			}
			return current_move;
		}
    }
    std::cout << current_move << std::endl;
    return NOMOVE;
}


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
	int promoted_piece = PROMOTED(move);

	if (promoted_piece) {
		char piece_char = 'q';
		if (PieceIsKing(promoted_piece)) {
			piece_char = 'n';
		} 
		else if (PieceIsQueenRook(promoted_piece) && !PieceIsQueenBishop(promoted_piece))  {
			piece_char = 'r';
		} 
		else if (!PieceIsQueenRook(promoted_piece) && PieceIsQueenBishop(promoted_piece))  {
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
