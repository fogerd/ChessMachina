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

class Board;

// Used by BitBoard to represent piece position 
typedef unsigned long long U64;

#define ENGINE_NAME "Chess Machina"
#define BOARD_SQUARE_NUMBER 120
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

// Needed for Move list array
#define MAXGAMEMOVES 2028
#define MAXMOVEPOSITIONS 256

enum Colors {
	WHITE,
	BLACK,
	BOTH
};

enum Pieces{
	EMPTY, 
	WHITE_PAWN, WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, WHITE_KING,
	BLACK_PAWN, BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, BLACK_KING
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


struct MOVE {
	int move;
	int score;
};

struct MOVELIST {
	MOVE moves[MAXMOVEPOSITIONS];
	int move_count;
};

struct UNDOMOVE {
	int move;
	int castle_permission;
	int en_passant_square;
	int fifty_move_count;
	U64 position_key;
};





/* GAME MOVE */

/*
0000 0000 0000 0000 0000 0111 1111 -> From 0x7F
0000 0000 0000 0011 1111 1000 0000 -> To >> 7, 0x7F
0000 0000 0011 1100 0000 0000 0000 -> Captured >> 14, 0xF
0000 0000 0100 0000 0000 0000 0000 -> EP 0x40000
0000 0000 1000 0000 0000 0000 0000 -> Pawn Start 0x80000
0000 1111 0000 0000 0000 0000 0000 -> Promoted Piece >> 20, 0xF
0001 0000 0000 0000 0000 0000 0000 -> Castle 0x1000000
*/

#define FROMSQUARE(move) ( (move)      & 0x7F)
#define TOSQUARE(move)   (((move)>>7)  & 0x7F)
#define CAPTURED(move)   (((move)>>14) & 0xF)
#define PROMOTED(move)   (((move)>>20) & 0xF)

#define MOVEFLAGENPASSANT 0x40000
#define MOVEFLAGPAWNSTART 0x80000   // PAWN made first move, creating an EnPassant square 
#define MOVEFLAGCASTLE    0x1000000

#define MOVEFLAGCAPATURE  0x7C000
#define MOVEFLAGPROMOTION 0xF00000


extern int square120_to_square64[BOARD_SQUARE_NUMBER];
extern int square64_to_square120[64];
extern U64 PieceKeys[13][120];
extern U64 SideKey;
extern U64 CastleKeys[16];

// Easy check for piece positions, value and color
extern int BigPiecePostions[13];
extern int MajorPiecePostions[13];
extern int MinorPiecePostions[13];
extern int PieceValues[13];
extern int PieceColor[13];

extern U64 SetMask[64];
extern U64 ClearMask[64];

extern int file_of[BOARD_SQUARE_NUMBER];
extern int rank_of[BOARD_SQUARE_NUMBER];

extern int PieceIsKing[13];
extern int PieceIsQueenRook[13];
extern int PieceIsQueenBishop[13];
extern int PieceIsKnight[13];
extern int PieceIsPawn[13];
extern int PieceIsSlider[13];


#define CONVERT_FILE_AND_RANK_TO_SQUARE(file, rank) ( (21 + (file) ) + ( (rank) * 10 ) )
#define SQ64_TO_SQ120(sq120) (square120_to_square64[(sq120)])
#define SQ120_TO_SQ64(sq64) (square64_to_square120[(sq64)])
#define SETBIT(bitboard, square) ((bitboard) |= SetMask[(square)])
#define CLEARBIT(bitboard, square) ((bitboard) &= SetMask[(square)])

#define PieceIsKnight(p) (PieceIsKnight[(p)])
#define PieceIsQueenRook(p) (PieceIsQueenRook[(p)])
#define PieceIsQueenBishop(p) (PieceIsQueenBishop[(p)])
#define PieceIsKing(p) (PieceIsKing[(p)])

// init.cpp
extern void InitAll();

// bitboard.cpp
extern void PrintBitBoard(U64 bb);
extern int PopBit(U64 *bb);
extern int CountBits(U64 b);

// attack_geneartor.cpp
extern int IsSquareAttacked(const int square, const int side, Board *board);

extern void PrintMoveList(const MOVELIST *move_list);

#endif