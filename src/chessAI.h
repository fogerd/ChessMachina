#ifndef ADD_H
#define ADD_H

#include <stdio.h>
#include "defs.h"

using namespace std;

///I want to say ChessBoard is BitBoard, but I'm not sure so it's like this for now
class GameControl{
    protected:


    public:
        pair<pair<int,int>,pair<int,int>> minimax( int depth , U64 _B );
        int maxSearch( int depth , U64 _B );
        int minSearch( int depth , U64 _B );
        int Evaluate(U64 _B);

};

#endif
