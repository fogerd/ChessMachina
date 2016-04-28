/******************************************************************************
*
*	FILE - BOARD.H
*	PURPORSE - Game Board object, contains all information regarding the game.
* 
*	The 8x8 (64 square) game board is represented using an array of size 120.
*	This is used to help recognize offboard indices while generating moves. 
*	
*	int pieces[BOARD_SQUARE_NUMBER] =
*
*      	 0     1   2   3   4   5   6   7   8    9 
*	  	10    11  12  13  14  15  16  17  18   19
*		     --------------------------------
*	   	20  | 21  22  23  24  25  26  27  28 | 29 
*		30  | 31  32  33  34  35  36  37  38 | 39 
*	  	40  | 41  42  43  44  45  46  47  48 | 49 
*	  	50  | 51  52  53  54  55  56  57  58 | 59 
*	  	60  | 61  62  63  64  65  66  67  68 | 69 
*	  	70  | 71  72  73  74  75  76  77  78 | 79 
*	  	80  | 81  82  83  84  85  86  87  88 | 89 
*     	90  | 91  92  93  94  95  96  97  98 | 99 
*		     --------------------------------
*	   100   101 102 103 104 105 106 107 108  109
*	   110   111 112 113 114 115 116 117 118  119
*
*
*	EMPTY BOARD:
*
*     		99   99 99 99 99 99 99 99 99   99 
*	  		99   99 99 99 99 99 99 99 99   99
*		 		-------------------------
*	  		99 |  0  0  0  0  0  0  0  0 | 99 
*	  		99 |  0  0  0  0  0  0  0  0 | 99 
*	  		99 |  0  0  0  0  0  0  0  0 | 99 
*	  		99 |  0  0  0  0  0  0  0  0 | 99 
*	 		99 |  0  0  0  0  0  0  0  0 | 99 
*	 		99 |  0  0  0  0  0  0  0  0 | 99 
*	 		99 |  0  0  0  0  0  0  0  0 | 99 
*    		99 |  0  0  0  0  0  0  0  0 | 99 
*		 		-------------------------
*	 		99   99 99 99 99 99 99 99 99   99
*	  		99   99 99 99 99 99 99 99 99   99
*
*
*	STARTING POSITION:
*
*     		99   99 99 99 99 99 99 99 99   99 
*	  		99   99 99 99 99 99 99 99 99   99
*		 		-------------------------
*	  		99 |  4  2  3  6  5  3  2  4 | 99 
*	  		99 |  1  1  1  1  1  1  1  1 | 99 
*	  		99 |  0  0  0  0  0  0  0  0 | 99 
*	  		99 |  0  0  0  0  0  0  0  0 | 99 
*	 		99 |  0  0  0  0  0  0  0  0 | 99 
*	 		99 |  0  0  0  0  0  0  0  0 | 99 
*	 		99 |  7  7  7  7  7  7  7  7 | 99 
*    		99 | 10  8  9 12 11  9  8 10 | 99 
*		 		-------------------------
*	 		99   99 99 99 99 99 99 99 99   99
*	  		99   99 99 99 99 99 99 99 99   99
*
*
*	RESPONSIBILTIES  	
*		The Game Board Object is the main component of Chess Machina.
*		Board keeps track of the current board position & move history.
*
*	COLLABORATORS
*		MoveGenerator - Calculates all valid moves given a Board's position.
*		HashGenerator - Creates a unique Hash Key for each possible board position.
*
*	FUNCTIONS
*		PrintBoard()  - Output board to console in a human-readable format.
*		ResetBoard()  - Reset all Board values to a blank board.
*		UpdateBoard() - Called after a new FEN is parsed to set board up.
*		ValidBoard()  - Assert that all board positions / values are legal.
*		ParseFEN()    - Update board to FEN position. See implementation for details.
*
*	VARIABLES
*		piece_number[13] - Keeps track of the # of each piece type on the board.
*			e.x -> At the start of the game, piece_number[WHITE_PAWN] = 8. 
*
*		piece_list[13][10] - Keep track of the location for all pieces on board.
*			e.x. ->	At Start Postion:
*						piece_list[WHITE_BISHOP][0] = C1
*						piece_list[WHITE_BISHOP][1] = F1
*
*		ply - Denotes a half-move, a move by only one side.
*		pawns[3] - 64 Bitboards for WHITE, BLACK, & BOTH PAWNS. See bitboard.cpp
*		king_square_location[2] - Used for quick lookups regrading Checks/Mates.
*		position_key - Unique hash key generated for each possible board position.
*		move_history[MAXGAMEMOVES] - Struct with information needed to undo a move.
*	
*	NOTES
*		Using piece_number and piece_list together is the fastest way of 
*		iterating through all pieces on the board.
*
*	READING 
*		[1] https://chessprogramming.wikispaces.com/Board+Representation
*		[2] https://chessprogramming.wikispaces.com/10x12+Board
*
*	AUTHOR - Dennis Fogerty								   
*	DATE   - 3/17/2016
*
******************************************************************************/

#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include "defs.h"
#include "hash.h"


// TODO : Split FEN parser into its own Class.

class Hash;

class Board {
	public:
		Board();
		
		void PrintBoard();
		void ResetBoard();
		void UpdateBoard();
		bool ValidBoard();
		
		int ParseFEN(char *FEN, Hash HashGenerator);
		void InitSquare120To64();

		int side_to_move;
		int en_passant_square;
		int fifty_move_count;
		int castle_permission;

		int ply_count;
		int ply_history;

		U64 position_key;

		int pieces[BOARD_SQUARE_NUMBER];
		int piece_number[13];
		int piece_list[13][10];

		U64 pawns[3];
		int king_square_location[2];	

		int big_pieces_count[2];  	// All pieces except pawns
		int major_pieces_count[2];	// Rooks & Queens
		int minor_pieces_count[2];	// Bishops & Knights
		int material_score[2];
		
		int square120_to_square64[BOARD_SQUARE_NUMBER];
		int square64_to_square120[64];

		UNDOMOVE move_history[MAXGAMEMOVES];

		Hash HashGenerator;
};

#endif