//
// Created by james on 4/14/16.
//

#ifndef CHESSMACHINA_MOVE_SCORER_H
#define CHESSMACHINA_MOVE_SCORER_H

#include "defs.h"
#include <vector>
#include <algorithm>

struct Move_Comparator;

class Move_Scorer {
	public:
		//Assign a move a score value, positive values are good for white, negative values are good for black.
		virtual double score_move(const Move& move)=0;
		//Sort a vector of moves so that the positively scoring moves are first, the negative ones last.
		void sort_moves(std::vector<Move>* moves, Colors color);
};

//A small struct for sorting moves in favor of the given color using the given Move_Scorer.
struct Move_Comparator{
	Move_Scorer* scorer;
	Colors color;
	bool operator()(const Move& a, const Move& b) {
		if (color == WHITE) {
			return this->scorer->score_move(a) > this->scorer->score_move(b);
		} else {
			return this->scorer->score_move(a) < this->scorer->score_move(b);
		}
	}
	Move_Comparator(Move_Scorer* s, Colors c){
		scorer=s;
		color=c;
	}
};

#endif //CHESSMACHINA_MOVE_SCORER_H
