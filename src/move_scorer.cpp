//
// Created by james on 4/14/16.
//

#include "move_scorer.h"

void Move_Scorer::sort_moves(std::vector <Move>* moves, Colors color) {
	//Sort a Vector of moves in favor of the given color.
	std::sort(moves->begin(),moves->end(),Move_Comparator(this,color));
}