//
// Created by james on 4/3/16.
//

#include <stdlib.h>
#include "move_generator.h"
#include "defs.h"
#include "board.h"


Move_Generator::Move_Generator(Board *b) {
  board=b;
}

Move* Move_Generator::genMove(BoardSquares starting_square,
                              BoardSquares ending_square) {
  //Moves are invalid if nothing is moving.
  if(starting_square==ending_square){
    return NULL;
  }
  //Moves are invalid if there isn't a piece at the starting location.
  Pieces starting_piece=board->pieces[starting_square];
  if(starting_piece==EMPTY){
    return NULL;
  }
  //Moves are invalid if the color of the starting piece is the same as the ending piece color.
  Colors starting_color=GET_COLOR_OF_PIECE(starting_piece);
  Pieces ending_piece=board->pieces[ending_square];
  Colors ending_color=GET_COLOR_OF_PIECE(ending_piece);
  if(starting_color==ending_color){
    return NULL;
  }
  std::vector<Move>* possibleMoves= genMoves(starting_square,starting_piece);
  Move* answer=NULL;
  for (std::vector<Move>::iterator iter= possibleMoves->begin(); iter!=possibleMoves->end();++iter){
    if((*iter).ending_square==ending_square){
      answer= (Move*)malloc(sizeof *answer);
      *answer=*iter;
      break;
    }
  }
	free(possibleMoves);
  return answer;
}


void get_pawn_moves(Board* board,
                    std::vector<Move>* move_vector,
										BoardSquares square,
                    Colors color,
                    BoardSquares front_square,
										BoardSquares square2InFront,
                    bool is_promotion,
										bool can_passant){
  Colors opposite=color==WHITE?BLACK:WHITE;
  if(GET_COLOR_OF_PIECE(board->pieces[front_square])==CLEAR){
    if(is_promotion){
      if(color==WHITE) {
        move_vector->push_back(Move{square, front_square, color, EMPTY, false, false,WHITE_BISHOP});
        move_vector->push_back(Move{square, front_square, color, EMPTY, false, false,WHITE_ROOK});
        move_vector->push_back(Move{square, front_square, color, EMPTY, false, false,WHITE_KNIGHT});
        move_vector->push_back(Move{square, front_square, color, EMPTY, false, false,WHITE_ROOK});
      }else{
        move_vector->push_back(Move{square, front_square, color, EMPTY, false, false,BLACK_BISHOP});
        move_vector->push_back(Move{square, front_square, color, EMPTY, false, false,BLACK_ROOK});
        move_vector->push_back(Move{square, front_square, color, EMPTY, false, false,BLACK_KNIGHT});
        move_vector->push_back(Move{square, front_square, color, EMPTY, false, false,BLACK_ROOK});
      }
    }else {
			if(can_passant&&GET_COLOR_OF_PIECE(board->pieces[square2InFront])==CLEAR){
				move_vector->push_back(Move(square, square2InFront, color,false,true));
			}
			move_vector->push_back(Move(square, front_square, color));
    }
  }
  BoardSquares front_left_square=LEFT_SQUARE(front_square,1);
  Pieces front_left_square_piece=board->pieces[front_left_square];
  if(GET_COLOR_OF_PIECE(front_left_square_piece)==opposite){
    if(is_promotion){
      if(color==WHITE) {
        move_vector->push_back(Move{square, front_left_square, color, front_left_square_piece, false, false,WHITE_BISHOP});
        move_vector->push_back(Move{square, front_left_square, color, front_left_square_piece, false, false,WHITE_ROOK});
        move_vector->push_back(Move{square, front_left_square, color, front_left_square_piece, false, false,WHITE_KNIGHT});
        move_vector->push_back(Move{square, front_left_square, color, front_left_square_piece, false, false,WHITE_ROOK});
      }else{
        move_vector->push_back(Move{square, front_left_square, color, front_left_square_piece, false, false,BLACK_BISHOP});
        move_vector->push_back(Move{square, front_left_square, color, front_left_square_piece, false, false,BLACK_ROOK});
        move_vector->push_back(Move{square, front_left_square, color, front_left_square_piece, false, false,BLACK_KNIGHT});
        move_vector->push_back(Move{square, front_left_square, color, front_left_square_piece, false, false,BLACK_ROOK});
      }
    }else {
      move_vector->push_back(Move(square, front_left_square, color));
    }
  }
  BoardSquares front_right_square=RIGHT_SQUARE(front_square,1);
  Pieces front_right_square_piece=board->pieces[front_right_square];
  if(GET_COLOR_OF_PIECE(front_right_square_piece)==opposite){
    if(is_promotion){
      if(color==WHITE) {
        move_vector->push_back(Move{square, front_right_square, color, front_right_square_piece, false, false,WHITE_BISHOP});
        move_vector->push_back(Move{square, front_right_square, color, front_right_square_piece, false, false,WHITE_ROOK});
        move_vector->push_back(Move{square, front_right_square, color, front_right_square_piece, false, false,WHITE_KNIGHT});
        move_vector->push_back(Move{square, front_right_square, color, front_right_square_piece, false, false,WHITE_ROOK});
      }else{
        move_vector->push_back(Move{square, front_right_square, color, front_right_square_piece, false, false,BLACK_BISHOP});
        move_vector->push_back(Move{square, front_right_square, color, front_right_square_piece, false, false,BLACK_ROOK});
        move_vector->push_back(Move{square, front_right_square, color, front_right_square_piece, false, false,BLACK_KNIGHT});
        move_vector->push_back(Move{square, front_right_square, color, front_right_square_piece, false, false,BLACK_ROOK});
      }
    }else {
      move_vector->push_back(Move(square, front_right_square, color));
    }
  }
}

void get_knight_moves(Board* board,
											std::vector<Move>* move_vector,
											BoardSquares square,
											Colors color) {
	Colors opposite=color==WHITE?BLACK:WHITE;
	BoardSquares tmp;
	Pieces tmpPiece;
	Colors tmpColor;
	tmp = LEFT_SQUARE(FORWARD_SQUARE(square, 1), 2);
	tmpPiece = board->pieces[tmp];
	tmpColor = GET_COLOR_OF_PIECE(tmpPiece);
	if (tmpPiece == EMPTY) {
		move_vector->push_back(Move(square, tmp, color));
	} else if (tmpColor == opposite) {
		move_vector->push_back(Move(square, tmp, color, tmpPiece));
	}
	tmp = RIGHT_SQUARE(FORWARD_SQUARE(square, 1), 2);
	tmpPiece = board->pieces[tmp];
	tmpColor = GET_COLOR_OF_PIECE(tmpPiece);
	if (tmpPiece == EMPTY) {
		move_vector->push_back(Move(square, tmp, color));
	} else if (tmpColor == opposite) {
		move_vector->push_back(Move(square, tmp, color, tmpPiece));
	}
	tmp = LEFT_SQUARE(FORWARD_SQUARE(square, 2), 1);
	tmpPiece = board->pieces[tmp];
	tmpColor = GET_COLOR_OF_PIECE(tmpPiece);
	if (tmpPiece == EMPTY) {
		move_vector->push_back(Move(square, tmp, color));
	} else if (tmpColor == opposite) {
		move_vector->push_back(Move(square, tmp, color, tmpPiece));
	}
	tmp = RIGHT_SQUARE(FORWARD_SQUARE(square, 2), 1);
	tmpPiece = board->pieces[tmp];
	tmpColor = GET_COLOR_OF_PIECE(tmpPiece);
	if (tmpPiece == EMPTY) {
		move_vector->push_back(Move(square, tmp, color));
	} else if (tmpColor == opposite) {
		move_vector->push_back(Move(square, tmp, color, tmpPiece));
	}
	tmp = LEFT_SQUARE(BACKWARD_SQUARE(square, 1), 2);
	tmpPiece = board->pieces[tmp];
	tmpColor = GET_COLOR_OF_PIECE(tmpPiece);
	if (tmpPiece == EMPTY) {
		move_vector->push_back(Move(square, tmp, color));
	} else if (tmpColor == opposite) {
		move_vector->push_back(Move(square, tmp, color, tmpPiece));
	}
	tmp = RIGHT_SQUARE(BACKWARD_SQUARE(square, 1), 2);
	tmpPiece = board->pieces[tmp];
	tmpColor = GET_COLOR_OF_PIECE(tmpPiece);
	if (tmpPiece == EMPTY) {
		move_vector->push_back(Move(square, tmp, color));
	} else if (tmpColor == opposite) {
		move_vector->push_back(Move(square, tmp, color, tmpPiece));
	}
	tmp = LEFT_SQUARE(BACKWARD_SQUARE(square, 2), 1);
	tmpPiece = board->pieces[tmp];
	tmpColor = GET_COLOR_OF_PIECE(tmpPiece);
	if (tmpPiece == EMPTY) {
		move_vector->push_back(Move(square, tmp, color));
	} else if (tmpColor == opposite) {
		move_vector->push_back(Move(square, tmp, color, tmpPiece));
	}
	tmp = RIGHT_SQUARE(BACKWARD_SQUARE(square, 2), 1);
	tmpPiece = board->pieces[tmp];
	tmpColor = GET_COLOR_OF_PIECE(tmpPiece);
	if (tmpPiece == EMPTY) {
		move_vector->push_back(Move(square, tmp, color));
	} else if (tmpColor == opposite) {
		move_vector->push_back(Move(square, tmp, color, tmpPiece));
	}
}

void get_bishop_moves(Board* board,
											std::vector<Move>* move_vector,
											BoardSquares square,
											Colors color){
	Colors opposite=color==WHITE?BLACK:WHITE;
	Pieces tmpPiece;
	for(BoardSquares tmpSquare=RIGHT_SQUARE(FORWARD_SQUARE(square,1),1);;tmpSquare=RIGHT_SQUARE(FORWARD_SQUARE(tmpSquare,1),1)){
		tmpPiece=board->pieces[tmpSquare];
		if(tmpPiece==EMPTY){
			move_vector->push_back(Move(square,tmpSquare,color));
		}else if(GET_COLOR_OF_PIECE(tmpPiece)==opposite){
			move_vector->push_back(Move(square,tmpSquare,color,tmpPiece));
			break;
		}else{
			break;
		}
	}
	for(BoardSquares tmpSquare=LEFT_SQUARE(FORWARD_SQUARE(square,1),1);;tmpSquare=LEFT_SQUARE(FORWARD_SQUARE(tmpSquare,1),1)){
		tmpPiece=board->pieces[tmpSquare];
		if(tmpPiece==EMPTY){
			move_vector->push_back(Move(square,tmpSquare,color));
		}else if(GET_COLOR_OF_PIECE(tmpPiece)==opposite){
			move_vector->push_back(Move(square,tmpSquare,color,tmpPiece));
			break;
		}else{
			break;
		}
	}
	for(BoardSquares tmpSquare=RIGHT_SQUARE(BACKWARD_SQUARE(square,1),1);;tmpSquare=RIGHT_SQUARE(BACKWARD_SQUARE(tmpSquare,1),1)){
		tmpPiece=board->pieces[tmpSquare];
		if(tmpPiece==EMPTY){
			move_vector->push_back(Move(square,tmpSquare,color));
		}else if(GET_COLOR_OF_PIECE(tmpPiece)==opposite){
			move_vector->push_back(Move(square,tmpSquare,color,tmpPiece));
			break;
		}else{
			break;
		}
	}
	for(BoardSquares tmpSquare=LEFT_SQUARE(BACKWARD_SQUARE(square,1),1);;tmpSquare=LEFT_SQUARE(BACKWARD_SQUARE(tmpSquare,1),1)){
		tmpPiece=board->pieces[tmpSquare];
		if(tmpPiece==EMPTY){
			move_vector->push_back(Move(square,tmpSquare,color));
		}else if(GET_COLOR_OF_PIECE(tmpPiece)==opposite){
			move_vector->push_back(Move(square,tmpSquare,color,tmpPiece));
			break;
		}else{
			break;
		}
	}
}

void get_rook_moves(Board* board,
										std::vector<Move>* move_vector,
										BoardSquares square,
										Colors color){
	Colors opposite=color==WHITE?BLACK:WHITE;
	Pieces tmpPiece;
	for(BoardSquares tmpSquare=RIGHT_SQUARE(square,1);;tmpSquare=RIGHT_SQUARE(tmpSquare,1)){
		tmpPiece=board->pieces[tmpSquare];
		if(tmpPiece==EMPTY){
			move_vector->push_back(Move(square,tmpSquare,color));
		}else if(GET_COLOR_OF_PIECE(tmpPiece)==opposite){
			move_vector->push_back(Move(square,tmpSquare,color,tmpPiece));
			break;
		}else{
			break;
		}
	}
	for(BoardSquares tmpSquare=FORWARD_SQUARE(square,1);;tmpSquare=FORWARD_SQUARE(tmpSquare,1)){
		tmpPiece=board->pieces[tmpSquare];
		if(tmpPiece==EMPTY){
			move_vector->push_back(Move(square,tmpSquare,color));
		}else if(GET_COLOR_OF_PIECE(tmpPiece)==opposite){
			move_vector->push_back(Move(square,tmpSquare,color,tmpPiece));
			break;
		}else{
			break;
		}
	}
	for(BoardSquares tmpSquare=BACKWARD_SQUARE(square,1);;tmpSquare=BACKWARD_SQUARE(tmpSquare,1)){
		tmpPiece=board->pieces[tmpSquare];
		if(tmpPiece==EMPTY){
			move_vector->push_back(Move(square,tmpSquare,color));
		}else if(GET_COLOR_OF_PIECE(tmpPiece)==opposite){
			move_vector->push_back(Move(square,tmpSquare,color,tmpPiece));
			break;
		}else{
			break;
		}
	}
	for(BoardSquares tmpSquare=LEFT_SQUARE(square,1);;tmpSquare=LEFT_SQUARE(tmpSquare,1)){
		tmpPiece=board->pieces[tmpSquare];
		if(tmpPiece==EMPTY){
			move_vector->push_back(Move(square,tmpSquare,color));
		}else if(GET_COLOR_OF_PIECE(tmpPiece)==opposite){
			move_vector->push_back(Move(square,tmpSquare,color,tmpPiece));
			break;
		}else{
			break;
		}
	}
}

void get_king_moves(Board* board,
										std::vector<Move>* move_vector,
										BoardSquares square,
										Colors color){

	Colors opposite=color==WHITE?BLACK:WHITE;
	BoardSquares tmp;
	Pieces tmpPiece;
	Colors tmpColor;
	tmp = LEFT_SQUARE(FORWARD_SQUARE(square, 1), 1);
	tmpPiece = board->pieces[tmp];
	tmpColor = GET_COLOR_OF_PIECE(tmpPiece);
	if (tmpPiece == EMPTY) {
		move_vector->push_back(Move(square, tmp, color));
	} else if (tmpColor == opposite) {
		move_vector->push_back(Move(square, tmp, color, tmpPiece));
	}
	tmp = RIGHT_SQUARE(FORWARD_SQUARE(square, 1), 1);
	tmpPiece = board->pieces[tmp];
	tmpColor = GET_COLOR_OF_PIECE(tmpPiece);
	if (tmpPiece == EMPTY) {
		move_vector->push_back(Move(square, tmp, color));
	} else if (tmpColor == opposite) {
		move_vector->push_back(Move(square, tmp, color, tmpPiece));
	}
	tmp = LEFT_SQUARE(BACKWARD_SQUARE(square, 1), 1);
	tmpPiece = board->pieces[tmp];
	tmpColor = GET_COLOR_OF_PIECE(tmpPiece);
	if (tmpPiece == EMPTY) {
		move_vector->push_back(Move(square, tmp, color));
	} else if (tmpColor == opposite) {
		move_vector->push_back(Move(square, tmp, color, tmpPiece));
	}
	tmp = RIGHT_SQUARE(BACKWARD_SQUARE(square, 1), 1);
	tmpPiece = board->pieces[tmp];
	tmpColor = GET_COLOR_OF_PIECE(tmpPiece);
	if (tmpPiece == EMPTY) {
		move_vector->push_back(Move(square, tmp, color));
	} else if (tmpColor == opposite) {
		move_vector->push_back(Move(square, tmp, color, tmpPiece));
	}
	tmp = FORWARD_SQUARE(square, 1);
	tmpPiece = board->pieces[tmp];
	tmpColor = GET_COLOR_OF_PIECE(tmpPiece);
	if (tmpPiece == EMPTY) {
		move_vector->push_back(Move(square, tmp, color));
	} else if (tmpColor == opposite) {
		move_vector->push_back(Move(square, tmp, color, tmpPiece));
	}
	tmp = RIGHT_SQUARE(square, 1);
	tmpPiece = board->pieces[tmp];
	tmpColor = GET_COLOR_OF_PIECE(tmpPiece);
	if (tmpPiece == EMPTY) {
		move_vector->push_back(Move(square, tmp, color));
	} else if (tmpColor == opposite) {
		move_vector->push_back(Move(square, tmp, color, tmpPiece));
	}
	tmp = LEFT_SQUARE(square, 1);
	tmpPiece = board->pieces[tmp];
	tmpColor = GET_COLOR_OF_PIECE(tmpPiece);
	if (tmpPiece == EMPTY) {
		move_vector->push_back(Move(square, tmp, color));
	} else if (tmpColor == opposite) {
		move_vector->push_back(Move(square, tmp, color, tmpPiece));
	}
	tmp = BACKWARD_SQUARE(square, 1);
	tmpPiece = board->pieces[tmp];
	tmpColor = GET_COLOR_OF_PIECE(tmpPiece);
	if (tmpPiece == EMPTY) {
		move_vector->push_back(Move(square, tmp, color));
	} else if (tmpColor == opposite) {
		move_vector->push_back(Move(square, tmp, color, tmpPiece));
	}
}

//TODO check for check when generating moves.
//TODO castling
std::vector<Move>* Move_Generator::genMoves(BoardSquares square,Pieces piece){
  std::vector<Move>* move_vector=new std::vector<Move>;
  Colors color=GET_COLOR_OF_PIECE(piece);
  Colors opposite=color==WHITE?BLACK:WHITE;
  switch (piece){
    case WHITE_PAWN:{
      bool is_promotion=square>H7;
			bool can_passant=square>=A2&square<=H2;
      BoardSquares front=FORWARD_SQUARE(square,1);
			BoardSquares front2=FORWARD_SQUARE(square,2);
      get_pawn_moves(board,move_vector,square,color,front,front2,is_promotion,can_passant);
      break;
    }
    case WHITE_KNIGHT: {
			get_knight_moves(board, move_vector, square, color);
			break;
		}
    case WHITE_BISHOP: {
			get_bishop_moves(board,move_vector,square,color);
			break;
		}
    case WHITE_ROOK: {
			get_rook_moves(board,move_vector,square,color);
			break;
		}
    case WHITE_QUEEN: {
			get_bishop_moves(board,move_vector,square,color);
			get_rook_moves(board,move_vector,square,color);
			break;
		}
    case WHITE_KING: {
			break;
		}
    case BLACK_PAWN: {
			bool is_promotion = square > A2;
			bool can_passant = square >= A7 & square <= H7;
			BoardSquares front = BACKWARD_SQUARE(square, 1);
			BoardSquares front2 = BACKWARD_SQUARE(square, 2);
			get_pawn_moves(board, move_vector, square, color, front, front2, is_promotion, can_passant);
			break;
		}
    case BLACK_KNIGHT: {
			get_knight_moves(board, move_vector, square, color);
			break;
		}
    case BLACK_BISHOP: {
			get_bishop_moves(board,move_vector,square,color);
			break;
		}
    case BLACK_ROOK: {
			get_rook_moves(board,move_vector,square,color);
			break;
		}
    case BLACK_QUEEN: {
			get_bishop_moves(board,move_vector,square,color);
			get_rook_moves(board,move_vector,square,color);
			break;
		}
    case BLACK_KING: {
			break;
		}
  }
  return move_vector;
}