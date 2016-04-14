//
// Created by james on 4/14/16.
//

#ifndef CHESSMACHINA_NAIVE_MOVE_SCORER_H
#define CHESSMACHINA_NAIVE_MOVE_SCORER_H

#include "move_scorer.h"

//This is a stupid little implementation that only values taking high value pieces.
class Naive_Move_Scorer : Move_Scorer{
	double score_move(const Move& move);
};


#endif //CHESSMACHINA_NAIVE_MOVE_SCORER_H
