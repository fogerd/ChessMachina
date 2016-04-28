/******************************************************************************
*
*   FILE - PERFT_TESTING.CPP
*   PURPORSE - 
* 
*   AUTHOR - Dennis Fogerty
*   DATE - 4/23/2016
*
******************************************************************************/

#include <assert.h>
#include <stdio.h>

#include "perft_testing.h"
#include "defs.h"
#include "board.h"
#include "move_generator.h"


void Perft::RecursivePerft(int depth, Board *gameboard, MoveGenerator MoveGen) {

    assert(gameboard->ValidBoard());

    // Base Case
	if (depth == 0) {
        number_of_leaf_nodes++;
        return;
    }	

    MOVELIST move_list[1];
    MoveGen.GenerateAllMoves(gameboard,move_list);
      
	for (int move_count = 0; move_count < move_list->move_count; ++move_count) {	
        
        // If not a valid move, skip it
        if (!MakeMove(gameboard,move_list->moves[move_count].move))  {
            continue;
        }

        // If move was valid, continue generating moves based on it
        RecursivePerft(depth - 1, gameboard, MoveGen);

        // Undo the move to reset the board
        TakeMove(gameboard);
    }

    return;
}


void Perft::PerformPerftTest(int depth, Board *gameboard, MoveGenerator MoveGen) {

    assert(gameboard->ValidBoard());

	printf("Performing Perft Test at Depth %d\n",depth);	
	number_of_leaf_nodes = 0;
	//int start = GetTimeMs();
    MOVELIST move_list[1];
    MoveGen.GenerateAllMoves(gameboard,move_list);	
    
    int move;

	for(int move_count = 0; move_count < move_list->move_count; ++move_count) {
        move = move_list->moves[move_count].move;
        if ( !MakeMove(gameboard,move)) {
            continue;
        }
        
        long total_nodes = number_of_leaf_nodes;
        RecursivePerft(depth - 1, gameboard, MoveGen);
        TakeMove(gameboard);        
        long oldnodes = number_of_leaf_nodes - total_nodes;
        //printf("move %d : %s : %ld\n",move_count+1,PrintMove(move),oldnodes);
    }
	
	printf("Test Complete : %ld nodes visited in ms\n", number_of_leaf_nodes);

    return;
}
