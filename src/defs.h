/*
*	FILE - DEFS.H
*	PURPORSE - Centralize commonly used variables & functions 
*				for use across multiple files
* 
*	AUTHOR - Dennis Fogerty
*	DATE - 3/14/2016
*/

#ifndef DEFS_H
#define DEFS_H


// Used by BitBoard to represent piece position 
typedef unsigned long long U64;

#define ENGINE_NAME "Chess Machina"
#define BOARD_SQUARE_NUMBER 120
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

// Needed for Move list array
#define MAXGAMEMOVES 2028

enum Colors {
	WHITE,
	BLACK,
	BOTH,
  CLEAR
};

enum Pieces{
	EMPTY, 
	WHITE_PAWN, WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, WHITE_KING,
	BLACK_PAWN, BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, BLACK_KING,
	INVALID
};

enum Files {	
	FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE
};

enum Ranks {
	RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE
};

enum BoardSquares {
	A1 = 21, B1, C1, D1, E1, F1, G1, H1,
	A2 = 31, B2, C2, D2, E2, F2, G2, H2,
	A3 = 41, B3, C3, D3, E3, F3, G3, H3,
	A4 = 51, B4, C4, D4, E4, F4, G4, H4,
	A5 = 61, B5, C5, D5, E5, F5, G5, H5,
	A6 = 71, B6, C6, D6, E6, F6, G6, H6,
	A7 = 81, B7, C7, D7, E7, F7, G7, H7,
	A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQUARE, OFFBOARD
};

// 4 Bit representation of whether or not each castling move is possible
// E.X. -> 1001
// White can castle king side, but not queen side.
// Black can not castle king side, but can castle queen side.
enum Castling {
	WHITE_KING_CASTLE = 1, WHITE_QUEEN_CASTLE = 2, 
	BLACK_KING_CASTLE = 4, BLACK_QUEEN_CASTLE = 8
};

struct UndoMove {
	int move;
	int castle_permission;
	int en_passant_square;
	int fifty_move_count;
	U64 position_key;
};

struct Move {
	BoardSquares starting_square;
	BoardSquares ending_square;
	Colors color;
	Pieces taken_piece;
	bool is_castle;
	bool is_en_passant;
	Pieces promotionType;


	Move(BoardSquares ss, BoardSquares es, Colors c,
			 Pieces taken,
			 bool cast,
			 bool p,
			 Pieces promotion){
		starting_square=ss;
		ending_square=es;
		color=c;
		taken_piece=taken;
		is_castle=cast;
		is_en_passant=p;
		promotionType=promotion;
	}

	Move(BoardSquares ss, BoardSquares es, Colors c):
		taken_piece(EMPTY),
		is_castle(false),
		is_en_passant(false),
		promotionType(EMPTY){
		starting_square=ss;
		ending_square=es;
		color=c;
	}

	Move(BoardSquares ss,
			 BoardSquares es,
			 Colors c,
			 Pieces taken):
		is_castle(false),
		is_en_passant(false),
		promotionType(EMPTY){
		starting_square=ss;
		ending_square=es;
		color=c;
		taken_piece=taken;
	}

	Move(BoardSquares ss,
			 BoardSquares es,
			 Colors c,
			 bool cast,
			 bool p):
		taken_piece(EMPTY),
		promotionType(EMPTY){
		starting_square=ss;
		ending_square=es;
		color=c;
		is_castle=cast;
		is_en_passant=p;
	}
};

extern int square120_to_square64[BOARD_SQUARE_NUMBER];
extern int square64_to_square120[64];

extern U64 SetMask[64];
extern U64 ClearMask[64];

#define CONVERT_FILE_AND_RANK_TO_SQUARE(file, rank) ( (21 + (file) ) + ( (rank) * 10 ) )
#define SQ64_TO_SQ120(sq120) (square120_to_square64[(sq120)])
#define SQ120_TO_SQ64(sq64) (square64_to_square120[(sq64)])
#define SETBIT(bitboard, square) ((bitboard) |= SetMask[(square)])
#define CLEARBIT(bitboard, square) ((bitboard) &= SetMask[(square)])

// init.cpp
extern void InitAll();

// bitboard.cpp
extern void PrintBitBoard(U64 bb);
extern int PopBit(U64 *bb);
extern int CountBits(U64 b);

#endif