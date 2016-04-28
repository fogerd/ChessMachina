/******************************************************************************
*
*	FILE - BOARD.CPP
*	PURPORSE - Game Board object function implementations.
* 
*	AUTHOR - Dennis Fogerty
*	DATE - 3/17/2016
*
******************************************************************************/

#include <assert.h>
#include <stdio.h>
#include "defs.h"
#include "board.h"
#include "hash.h"


int PceListOk(const Board *pos) {
	int pce = WHITE_PAWN;
	int sq;
	int num;
	for (pce = WHITE_PAWN; pce <= BLACK_KING; ++pce) {
		if(pos->piece_number[pce]<0 || pos->piece_number[pce]>=10) 
			return false;
	}

	if (pos->piece_number[WHITE_KING]!=1 || pos->piece_number[BLACK_KING]!=1) 
		return false;

	for (pce = WHITE_PAWN; pce <= BLACK_KING; ++pce) {
		for (num = 0; num < pos->piece_number[pce]; ++num) {
			sq = pos->piece_list[pce][num];
			if (!SquareIsOnBoard(sq)) 
				return false;
		}
	}
    return true;
}

Board::Board() {
	InitSquare120To64();
}

void Board::UpdateBoard() {
	int piece, square, index, color;

	for (index = 0; index < BOARD_SQUARE_NUMBER; ++index) {
		square = index;
		piece = pieces[index];
		if (piece != OFFBOARD && piece != EMPTY) {
			color = PieceColor[piece];

			if ( IsBigPiece[piece] == true )
				big_pieces_count[color]++;
			if ( IsMajorPiece[piece] == true )
				major_pieces_count[color]++;
			if ( IsMinorPiece[piece] == true )
				minor_pieces_count[color]++;

			material_score[color] += PieceValues[piece];

			piece_list[piece][piece_number[piece]] = square;
			piece_number[piece]++;


			if (piece == WHITE_KING) {
				king_square_location[WHITE] = square;
				std::cout << "KING MOVED TO " << SQ64_TO_SQ120(king_square_location[0]) << std::endl;
				//PrintBoard();
			}
			if (piece == BLACK_KING)
				king_square_location[BLACK] = square;

			if (piece == WHITE_PAWN) {
				SETBIT(pawns[WHITE],SQ64_TO_SQ120(square));
				SETBIT(pawns[BOTH],SQ64_TO_SQ120(square));
			}
			else if (piece == BLACK_PAWN) {
				SETBIT(pawns[BLACK],SQ64_TO_SQ120(square));
				SETBIT(pawns[BOTH],SQ64_TO_SQ120(square));
			}
		}
	}

}

bool Board::ValidBoard() {
	int t_pceNum[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int t_bigPce[2] = {0, 0};
	int t_majPce[2] = {0, 0};
	int t_minPce[2] = {0, 0};
	int t_material[2] = {0, 0};

	int sq64,t_piece,t_pce_num,sq120,colour,pcount;

	U64 t_pawns[3] = {0ULL, 0ULL, 0ULL};

	t_pawns[WHITE] = pawns[WHITE];
	t_pawns[BLACK] = pawns[BLACK];
	t_pawns[BOTH] = pawns[BOTH];

	// check piece lists
	for(t_piece = WHITE_PAWN; t_piece <= BLACK_KING; ++t_piece) {
		for(t_pce_num = 0; t_pce_num < piece_number[t_piece]; ++t_pce_num) {
			sq120 = piece_list[t_piece][t_pce_num];
			
			if (pieces[sq120]==t_piece)
				continue;
			else {
				std::cout << "\nFUCK" << std::endl;
				PrintBoard();
				std::cout << "WHITE KING " << SQ64_TO_SQ120(king_square_location[0]) << std::endl;

				std::cout << "BLACK KING " << SQ64_TO_SQ120(king_square_location[1]) << std::endl;

				//TakeMove(this);
				//int move = move_history[ply_history].move;
				//PrintBoard();
				//MakeMove(this, move);
				std::cout << pieces[sq120] << "!=" << t_piece << std::endl;
				return -1;
			}

		}
	}

	// check piece count and other counters
	for(sq64 = 0; sq64 < 64; ++sq64) {
		sq120 = SQ120_TO_SQ64(sq64);
		t_piece = pieces[sq120];
		t_pceNum[t_piece]++;
		colour = PieceColor[t_piece];
		if( IsBigPiece[t_piece] == true) t_bigPce[colour]++;
		if( IsMinorPiece[t_piece] == true) t_minPce[colour]++;
		if( IsMajorPiece[t_piece] == true) t_majPce[colour]++;

		t_material[colour] += PieceValues[t_piece];
	}

	for(t_piece = WHITE_PAWN; t_piece <= BLACK_KING; ++t_piece) {
		assert(t_pceNum[t_piece]==piece_number[t_piece]);
	}

	// check bitboards count
	pcount = CNT(t_pawns[WHITE]);
	assert(pcount == piece_number[WHITE_PAWN]);
	pcount = CNT(t_pawns[BLACK]);
	assert(pcount == piece_number[BLACK_PAWN]);
	pcount = CNT(t_pawns[BOTH]);
	assert(pcount == (piece_number[BLACK_PAWN] + piece_number[WHITE_PAWN]));

	// check bitboards squares
	while(t_pawns[WHITE]) {
		sq64 = POP(&t_pawns[WHITE]);
		assert(pieces[SQ120_TO_SQ64(sq64)] == WHITE_PAWN);
	}

	while(t_pawns[BLACK]) {
		sq64 = POP(&t_pawns[BLACK]);
		assert(pieces[SQ120_TO_SQ64(sq64)] == BLACK_PAWN);
	}

	while(t_pawns[BOTH]) {
		sq64 = POP(&t_pawns[BOTH]);
		assert( (pieces[SQ120_TO_SQ64(sq64)] == BLACK_PAWN) || (pieces[SQ120_TO_SQ64(sq64)] == WHITE_PAWN) );
	}

	assert(t_material[WHITE]==material_score[WHITE] && t_material[BLACK]==material_score[BLACK]);
	assert(t_minPce[WHITE]==minor_pieces_count[WHITE] && t_minPce[BLACK]==minor_pieces_count[BLACK]);
	assert(t_majPce[WHITE]==major_pieces_count[WHITE] && t_majPce[BLACK]==major_pieces_count[BLACK]);
	assert(t_bigPce[WHITE]==big_pieces_count[WHITE] && t_bigPce[BLACK]==big_pieces_count[BLACK]);

	assert(side_to_move==WHITE || side_to_move==BLACK);
	assert(HashGenerator.GeneratePosKey(this) == position_key);

	assert(en_passant_square==NO_SQUARE || ( rank_of[en_passant_square]==RANK_6 && side_to_move == WHITE)
		 || ( rank_of[en_passant_square]==RANK_3 && side_to_move == BLACK));

	assert(pieces[king_square_location[WHITE]] == WHITE_KING);
	assert(pieces[king_square_location[BLACK]] == BLACK_KING);

	assert(castle_permission >= 0 && castle_permission <= 15);

	assert(PceListOk(this));

	return true;
}

// Convert 120 square board to 64 squares
void Board::InitSquare120To64() {
	int index = 0;
	int file = FILE_A;
	int rank = RANK_1;
	int square120 = A1;
	int square64 = 0;
	
	for(index = 0; index < BOARD_SQUARE_NUMBER; ++index) {
		square120_to_square64[index] = 65;
	}
	
	for(index = 0; index < 64; ++index) {
		square64_to_square120[index] = 120;
	}
	
	for(rank = RANK_1; rank <= RANK_8; ++rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			square120 = CONVERT_FILE_AND_RANK_TO_SQUARE(file,rank);
			square64_to_square120[square64] = square120;
			square120_to_square64[square120] = square64;
			square64++;
		}
	}
}

void Board::PrintBoard() {
	int square,file,rank,piece;
	char PieceChars[] = ".PNBRQKpnbrqk";
	printf("\nGame Board:\n\n");

	// RANK + PIECES
	for(rank = RANK_8; rank >= RANK_1; rank--) {
		printf("%d|",rank+1);
		for(file = FILE_A; file <= FILE_H; file++) {
			square = CONVERT_FILE_AND_RANK_TO_SQUARE(file,rank);
			piece = pieces[square];
			printf("%3c",PieceChars[piece]);
		}
		printf("\n");
	}

	// FILES
	printf("    ");
	printf("______________________\n  ");
	for(file = FILE_A; file <= FILE_H; file++) {
		printf("%3c",'A'+file);
	}
	printf("\n\n");

	// SIDE TO MOVE
	if (side_to_move == 0)
		printf("side   - White\n");
	else if (side_to_move == 1)
		printf("side   - Black\n");
	else
		printf("side   - -\n");
	
	// EN PASSANT
	if (en_passant_square==99)
		printf("enPas  -  \n");
	else
		printf("enPas  - %d\n",en_passant_square);

	// CASTLING PERMISSION
	printf("castle - %c%c%c%c\n",
			castle_permission & WHITE_KING_CASTLE ? 'K' : '-',
			castle_permission & WHITE_QUEEN_CASTLE ? 'Q' : '-',
			castle_permission & BLACK_KING_CASTLE ? 'k' : '-',
			castle_permission & BLACK_QUEEN_CASTLE ? 'q' : '-'
			);
	
	// POSTION HASH KEY
	printf("PosKey - %llX\n",position_key);

}

void Board::ResetBoard() {
	int index = 0;

	for(index = 0; index < BOARD_SQUARE_NUMBER; ++index) {
		pieces[index] = OFFBOARD;
	}

	for(index = 0; index < 64; ++index) {
		pieces[SQ120_TO_SQ64(index)] = EMPTY;
	}

	for(index = 0; index < 2; ++index) {
		big_pieces_count[index] = 0;
		major_pieces_count[index] = 0;
		minor_pieces_count[index] = 0;
		pawns[index] = 0ULL;
		material_score[index] = 0;
	}

	for(index = 0; index < 13; ++index) {
		piece_number[index] = 0;
	}

	king_square_location[WHITE] = king_square_location[BLACK] = NO_SQUARE;

	side_to_move = BOTH;
	en_passant_square = NO_SQUARE;
	fifty_move_count = 0;

	ply_count = 0;
	ply_history = 0;

	castle_permission = 0;

	position_key = 0ULL;
}

int Board::ParseFEN(char *FEN, Hash HashGenerator) {
	int  rank = RANK_8;
	int  file = FILE_A;
	int  piece = 0;
	int  count = 0;
	int  i = 0;
	int  sq64 = 0;
	int  sq120 = 0;
	ResetBoard();

	while ((rank >= RANK_1) && *FEN) {
		count = 1;
		switch (*FEN) {
			case 'P': piece = WHITE_PAWN; break;
			case 'N': piece = WHITE_KNIGHT; break;
			case 'B': piece = WHITE_BISHOP; break;
			case 'R': piece = WHITE_ROOK; break;
			case 'Q': piece = WHITE_QUEEN; break;
			case 'K': piece = WHITE_KING; break;
			case 'p': piece = BLACK_PAWN; break;
			case 'n': piece = BLACK_KNIGHT; break;
			case 'b': piece = BLACK_BISHOP; break;
			case 'r': piece = BLACK_ROOK; break;
			case 'q': piece = BLACK_QUEEN; break;
			case 'k': piece = BLACK_KING; break;

			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
				piece = EMPTY;
				count = *FEN - '0';
				break;

			case '/':
			case ' ':
				rank--;
				file = FILE_A;
				FEN++;
				continue;

			default:
				std::cout << "FEN error \n";
				return -1;
		}

		for (i = 0; i < count; i++) {
			sq64 = rank * 8 + file;
			sq120 = SQ120_TO_SQ64(sq64);
			if (piece != EMPTY) {
				pieces[sq120] = piece;
			}
			if (piece == WHITE_KING) {
				std::cout << sq120 << std::endl;
			}
			file++;
		}
		FEN++;
	}

	assert(*FEN == 'w' || *FEN == 'b');

	side_to_move = (*FEN == 'w') ? WHITE : BLACK;
	FEN += 2;

	for (i = 0; i < 4; i++) {
		if (*FEN == ' ') {
			break;
		}
		switch(*FEN) {
			case 'K': castle_permission |= WHITE_KING_CASTLE; break;
			case 'Q': castle_permission |= WHITE_QUEEN_CASTLE; break;
			case 'k': castle_permission |= BLACK_KING_CASTLE; break;
			case 'q': castle_permission |= BLACK_QUEEN_CASTLE; break;
			default:	     break;
		}
		FEN++;
	}
	FEN++;

	assert(castle_permission>=0 && castle_permission <= 15);
	
	if (*FEN != '-') {
		file = FEN[0] - 'a';
		rank = FEN[1] - '1';

		assert(file>=FILE_A && file <= FILE_H);
		assert(rank>=RANK_1 && rank <= RANK_8);

		en_passant_square = CONVERT_FILE_AND_RANK_TO_SQUARE(file,rank);
	}

	position_key = HashGenerator.GeneratePosKey(this);

	UpdateBoard();

	return 0;
}
