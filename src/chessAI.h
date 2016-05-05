#ifndef ADD_H
#define ADD_H

#include <stdio.h>
#include "defs.h"
#include "board.h"

using namespace std;

///I want to say ChessBoard is BitBoard, but I'm not sure so it's like this for now
class GameControl{
    protected:


    public:
	GameControl();
        int minimax(Board *_B , int depth);
        int maxSearch( int depth , Board *_B );
        int minSearch( int depth , Board *_B );
        int Evaluate(Board *_B);

};

#endif
