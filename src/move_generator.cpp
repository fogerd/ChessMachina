/*
*	FILE - MOVE_GENERATOR.CPP
*	PURPORSE - Object for generating all possible moves for a given board position
 * 
*	AUTHOR - Dennis Fogerty
*	DATE - 4/23/2016
*/

#include "defs.h"
#include "move_generator.h"
#include "board.h"


#define MOVE(from, to, capture, promotion, flag) \
	( (from) | ((to) << 7) | ((capture) << 14) | ((promotion) << 20 ) | (flag))

#define SQUAREOFFBOARD(square) (file_of[(square)] == OFFBOARD)


MoveGenerator::MoveGenerator() {

}

void MoveGenerator::AddQuietMove(const Board *gameboard, int move, MOVELIST *move_list) {
	move_list->moves[move_list->move_count].move = move;
	move_list->moves[move_list->move_count].score = 0;
	move_list->move_count++;
}

void MoveGenerator::AddCaptureMove(const Board *gameboard, int move, MOVELIST *move_list) {
	move_list->moves[move_list->move_count].move = move;
	move_list->moves[move_list->move_count].score = 0;
	move_list->move_count++;
}

void MoveGenerator::AddEnPassantMove(const Board *gameboard, int move, MOVELIST *move_list) {
	move_list->moves[move_list->move_count].move = move;
	move_list->moves[move_list->move_count].score = 0;
	move_list->move_count++;
}

void MoveGenerator::GenerateAllMoves(Board *gameboard, MOVELIST *move_list) {

	move_list->move_count = 0;

	GenerateAllPawnMoves(gameboard, move_list);

	GenerateAllSlideMoves(gameboard, move_list);

	GenerateAllNonSlideMoves(gameboard, move_list);

}


void MoveGenerator::GenerateAllPawnMoves(Board *gameboard, MOVELIST *move_list) {

	side = gameboard->side_to_move;

	/* WHITE PAWN MOVES */
	if (side == WHITE) {

		for (piece_number = 0; piece_number < gameboard->piece_number[WHITE_PAWN]; ++piece_number) {
			square = gameboard->piece_list[WHITE_PAWN][piece_number];

			if (gameboard->pieces[square+10] == EMPTY) {
				AddWhitePawnMove(gameboard, square, square+10, move_list);
				if (rank_of[square] == RANK_2 && gameboard->pieces[square+20] == EMPTY) {
					AddQuietMove(gameboard, MOVE(square, (square+20), EMPTY, EMPTY, MOVEFLAGPAWNSTART), move_list);
				}
			}

			if(!SQUAREOFFBOARD(square + 9) && PieceColor[gameboard->pieces[square + 9]] == BLACK) {
				AddWhitePawnCapture(gameboard, square, square+9, gameboard->pieces[square + 9], move_list);
			}
			if(!SQUAREOFFBOARD(square + 11) && PieceColor[gameboard->pieces[square + 11]] == BLACK) {
				AddWhitePawnCapture(gameboard, square, square+11, gameboard->pieces[square + 11], move_list);
			}

			if(gameboard->en_passant_square != NO_SQUARE) {
				if(square + 9 == gameboard->en_passant_square) {
					AddEnPassantMove(gameboard, MOVE(square,square + 9,EMPTY,EMPTY,MOVEFLAGENPASSANT), move_list);
				}
				if(square + 11 == gameboard->en_passant_square) {
					AddEnPassantMove(gameboard, MOVE(square,square + 11,EMPTY,EMPTY,MOVEFLAGENPASSANT), move_list);
				}
			}
		}

		/* WHITE SIDE CASTLING */
		if(gameboard->castle_permission & WHITE_KING_CASTLE) {
			if(gameboard->pieces[F1] == EMPTY && gameboard->pieces[G1] == EMPTY) {
				if(!IsSquareAttacked(E1,BLACK,gameboard) && !IsSquareAttacked(F1,BLACK,gameboard) ) {
					AddQuietMove(gameboard, MOVE(E1, G1, EMPTY, EMPTY, MOVEFLAGCASTLE), move_list);
				}
			}
		}

		if(gameboard->castle_permission & WHITE_QUEEN_CASTLE) {
			if(gameboard->pieces[D1] == EMPTY && gameboard->pieces[C1] == EMPTY && gameboard->pieces[B1] == EMPTY) {
				if(!IsSquareAttacked(E1,BLACK,gameboard) && !IsSquareAttacked(D1,BLACK,gameboard) ) {
					AddQuietMove(gameboard, MOVE(E1, C1, EMPTY, EMPTY, MOVEFLAGCASTLE), move_list);
				}
			}
		}
	} 

	/* BLACK PAWN MOVES */
	else {

		for(piece_number = 0; piece_number < gameboard->piece_number[BLACK_PAWN]; ++piece_number) {
			square = gameboard->piece_list[BLACK_PAWN][piece_number];

			if(gameboard->pieces[square - 10] == EMPTY) {
				AddBlackPawnMove(gameboard, square, square-10, move_list);
				if(rank_of[square] == RANK_7 && gameboard->pieces[square - 20] == EMPTY) {
					AddQuietMove(gameboard, MOVE(square,(square-20),EMPTY,EMPTY,MOVEFLAGPAWNSTART),move_list);
				}
			}

			if(!SQUAREOFFBOARD(square - 9) && PieceColor[gameboard->pieces[square - 9]] == WHITE) {
				AddBlackPawnCapture(gameboard, square, square-9, gameboard->pieces[square - 9], move_list);
			}

			if(!SQUAREOFFBOARD(square - 11) && PieceColor[gameboard->pieces[square - 11]] == WHITE) {
				AddBlackPawnCapture(gameboard, square, square-11, gameboard->pieces[square - 11], move_list);
			}
			if(gameboard->en_passant_square != NO_SQUARE) {
				if(square - 9 == gameboard->en_passant_square) {
					AddEnPassantMove(gameboard, MOVE(square,square - 9,EMPTY,EMPTY,MOVEFLAGENPASSANT), move_list);
				}
				if(square - 11 == gameboard->en_passant_square) {
					AddEnPassantMove(gameboard, MOVE(square,square - 11,EMPTY,EMPTY,MOVEFLAGENPASSANT), move_list);
				}
			}
		}
		/* BLACK SIDE CASTLING */
		if(gameboard->castle_permission & BLACK_KING_CASTLE) {
			if(gameboard->pieces[F8] == EMPTY && gameboard->pieces[G1] == EMPTY) {
				if(!IsSquareAttacked(E8,WHITE,gameboard) && !IsSquareAttacked(F8,WHITE,gameboard) ) {
					AddQuietMove(gameboard, MOVE(E8, G8, EMPTY, EMPTY, MOVEFLAGCASTLE), move_list);
				}
			}
		}

		if(gameboard->castle_permission & BLACK_QUEEN_CASTLE) {
			if(gameboard->pieces[D8] == EMPTY && gameboard->pieces[C1] == EMPTY && gameboard->pieces[B1] == EMPTY) {
				if(!IsSquareAttacked(E8,WHITE,gameboard) && !IsSquareAttacked(D8,WHITE,gameboard) ) {
					AddQuietMove(gameboard, MOVE(E8, C8, EMPTY, EMPTY, MOVEFLAGCASTLE), move_list);
				}
			}
		}
	}
}

void MoveGenerator::GenerateAllSlideMoves(Board *gameboard, MOVELIST *move_list) {
	piece_index = sliding_pieces_index[side];
	piece = sliding_pieces[piece_index];

	while (piece != 0) {

		for (piece_number = 0; piece_number < gameboard->piece_number[piece]; ++piece_number) {
			square = gameboard->piece_list[piece][piece_number];

			for (index = 0; index < number_of_directions_per_piece[piece]; ++index) {
				direction = piece_direction[piece][index];
				tmp_square = square + direction;

				while (!SQUAREOFFBOARD(tmp_square)) {
					// BLACK ^ 1 == WHITE       WHITE ^ 1 == BLACK
					if(gameboard->pieces[tmp_square] != EMPTY) {
						if( PieceColor[gameboard->pieces[tmp_square]] == (side ^ 1)) {
							AddCaptureMove(gameboard, MOVE(square, tmp_square, gameboard->pieces[tmp_square], EMPTY, 0), move_list);
						}
						break;
					}
				AddQuietMove(gameboard, MOVE(square, tmp_square, EMPTY, EMPTY, 0), move_list);
				tmp_square += direction;
				}
			}
		}

		piece = sliding_pieces[piece_index++];
	}
}

void MoveGenerator::GenerateAllNonSlideMoves(Board *gameboard, MOVELIST *move_list) {
	piece_index = non_sliding_pieces_index[side];
	piece = non_sliding_pieces[piece_index++];

	while (piece != 0) {

		for (piece_number = 0; piece_number < gameboard->piece_number[piece]; ++piece_number) {
			square = gameboard->piece_list[piece][piece_number];

			for (index = 0; index < number_of_directions_per_piece[piece]; ++index) {
				direction = piece_direction[piece][index];
				tmp_square = square + direction;

				if (SQUAREOFFBOARD(tmp_square)) {
					continue;
				}

				// BLACK ^ 1 == WHITE       WHITE ^ 1 == BLACK
				if (gameboard->pieces[tmp_square] != EMPTY) {
					if(PieceColor[gameboard->pieces[tmp_square]] == (side ^ 1)) {
						AddCaptureMove(gameboard, MOVE(square, tmp_square, gameboard->pieces[tmp_square], EMPTY, 0), move_list);
					}
					continue;
				}
				AddQuietMove(gameboard, MOVE(square, tmp_square, EMPTY, EMPTY, 0), move_list);
			}
		}

		piece = non_sliding_pieces[piece_index++];
	}
}


void MoveGenerator::AddWhitePawnCapture(const Board *gameboard, const int from_sq, const int to_sq, const int capture, MOVELIST *move_list) {
	if (rank_of[from_sq] == RANK_7) {
		AddCaptureMove(gameboard, MOVE(from_sq,to_sq,capture,WHITE_QUEEN,0), move_list);
		AddCaptureMove(gameboard, MOVE(from_sq,to_sq,capture,WHITE_ROOK,0), move_list);
		AddCaptureMove(gameboard, MOVE(from_sq,to_sq,capture,WHITE_BISHOP,0), move_list);
		AddCaptureMove(gameboard, MOVE(from_sq,to_sq,capture,WHITE_KNIGHT,0), move_list);
	} 
	else 
		AddCaptureMove(gameboard, MOVE(from_sq,to_sq,capture,EMPTY,0), move_list);
}

void MoveGenerator::AddWhitePawnMove(const Board *gameboard, const int from_sq, const int to_sq, MOVELIST *move_list) {
	if (rank_of[from_sq] == RANK_7) {
		AddQuietMove(gameboard, MOVE(from_sq,to_sq,EMPTY,WHITE_QUEEN,0), move_list);
		AddQuietMove(gameboard, MOVE(from_sq,to_sq,EMPTY,WHITE_ROOK,0), move_list);
		AddQuietMove(gameboard, MOVE(from_sq,to_sq,EMPTY,WHITE_BISHOP,0), move_list);
		AddQuietMove(gameboard, MOVE(from_sq,to_sq,EMPTY,WHITE_KNIGHT,0), move_list);
	} 
	else 
		AddQuietMove(gameboard, MOVE(from_sq,to_sq,EMPTY,EMPTY,0), move_list);
}

void MoveGenerator::AddBlackPawnCapture(const Board *gameboard, const int from_sq, const int to_sq, const int capture, MOVELIST *move_list) {
	if (rank_of[from_sq] == RANK_2) {
		AddCaptureMove(gameboard, MOVE(from_sq,to_sq,capture,BLACK_QUEEN,0), move_list);
		AddCaptureMove(gameboard, MOVE(from_sq,to_sq,capture,BLACK_ROOK,0), move_list);
		AddCaptureMove(gameboard, MOVE(from_sq,to_sq,capture,BLACK_BISHOP,0), move_list);
		AddCaptureMove(gameboard, MOVE(from_sq,to_sq,capture,BLACK_KNIGHT,0), move_list);
	} 
	else
		AddCaptureMove(gameboard, MOVE(from_sq,to_sq,capture,EMPTY,0), move_list);
}

void MoveGenerator::AddBlackPawnMove(const Board *gameboard, const int from_sq, const int to_sq, MOVELIST *move_list) {
	if (rank_of[from_sq] == RANK_2) {
		AddQuietMove(gameboard, MOVE(from_sq,to_sq,EMPTY,BLACK_QUEEN,0), move_list);
		AddQuietMove(gameboard, MOVE(from_sq,to_sq,EMPTY,BLACK_ROOK,0), move_list);
		AddQuietMove(gameboard, MOVE(from_sq,to_sq,EMPTY,BLACK_BISHOP,0), move_list);
		AddQuietMove(gameboard, MOVE(from_sq,to_sq,EMPTY,BLACK_KNIGHT,0), move_list);
	} 
	else
		AddQuietMove(gameboard, MOVE(from_sq,to_sq,EMPTY,EMPTY,0), move_list);
}
