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

class Move_Generator {
  public:
    //Create a new MoveGenerator that works with the given Board.
    Move_Generator(Board *b);
    //Return a pointer to the move representing the piece on starting_square moving to ending_square.
    //Returns null if the move is invalid.
    Move* gen_move(BoardSquares starting_square,
                   BoardSquares ending_square);
    //Add all the possible moves of the given piece to the given move_vector.
    //Assumes that the given piece is on the given square.
    void gen_moves(std::vector <Move> *move_vector, BoardSquares square, Pieces piece);
  private:
    Board *board;
};


#endif //CHESSMACHINA_MOVE_H
