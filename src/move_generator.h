//
// Created by james on 4/3/16.
//

#ifndef CHESSMACHINA_MOVE_H
#define CHESSMACHINA_MOVE_H

#include "board.h"
#include <vector>
#include <stdlib.h>
#include "move_generator.h"
#include "defs.h"

struct Move {
  BoardSquares starting_square;
  BoardSquares ending_square;
  Colors color;
  Pieces taken_piece;
  bool is_castle;
  bool is_en_passant;
  Pieces promotionType;


	Move(BoardSquares ss, BoardSquares es, Colors c, Pieces taken,bool cast, bool p, Pieces promotion){
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
	Move(BoardSquares ss, BoardSquares es, Colors c,Pieces taken):
		is_castle(false),
		is_en_passant(false),
		promotionType(EMPTY){
		starting_square=ss;
		ending_square=es;
		color=c;
		taken_piece=taken;
	}

	Move(BoardSquares ss, BoardSquares es, Colors c,bool cast, bool p):
		taken_piece(EMPTY),
		promotionType(EMPTY){
		starting_square=ss;
		ending_square=es;
		color=c;
		is_castle=cast;
		is_en_passant=p;
	}
};


class Move_Generator {
  public:
    //Create a new MoveGenerator that works with the given Board.
    Move_Generator(Board *b);
    //Return a pointer to the move representing the piece on starting_square moving to ending_square.
    Move* genMove(BoardSquares starting_square,
                  BoardSquares ending_square);
    //Return a pointer to the vector of possible moves that the piece on the given square can move.
    //Assumes that the given piece is on the given square.
    std::vector<Move>* genMoves(BoardSquares square, Pieces piece);
  private:
    Board *board;
};


#endif //CHESSMACHINA_MOVE_H
