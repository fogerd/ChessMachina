/// /// ///Chess Machina AI; MinMax only
/// /// ///First iteration made by Nicholas Lockwood, Pushed 4/25/2016
/// /// ///
///This was made largely on paper first. Several of the function names will need to be
///changed as they came out of psudocode. Optimization will still require several more additions,
///including but not limited to alpha-beta search and Quiescence search. Easily changeable but currently
///a notable part is that AI player color is currently hardcoded in.
///Operation is as follows:
///     Board and depth goes into minimax function
///     minimax sets initial "losing" score, and searches for "best" move
///     It does this by calling minSearch, which calls maxSearch, which calls minSearch........
///     When depth == 0, Evaluate is called. This adds the total score of all the pieces
///
/// /// The iteration does not currently factor in the 128 size bitboard, and only iterates an 8x8 board
/// /// Weights are simple, but need to be established
/// /// getPossibleMovesList probably needs changed
/// /// Chessboard _B probably does not properly utilize bitboard architecture.
/// /// ///

#include<iostream>
#include<vector>
#include<limits>
#include "chessAI.h"
#include "board.h"
#include "move_generator.h"

///A-B pruning is not a priority compared to making sure this works in general

using namespace std;

const int INFINITY = numeric_limits<int>::max();



int GameControl::minimax(Board *_B, int depth)
{
    /// ///MinMax tree. Currently does NOT use Alpha-Beta Pruning or horizon effect reduction e.g. Quiescence search
    /// Note: Currently assumes AI plays WHITE
    ///int depth is the ply at which we do the search. Start at one, move on down through minSearch/maxSearch

    //assume worse score initially; "win" is +INF
    int bestValue = -INFINITY;

    //Pair of pairs for <starting i,j>, <ending i,j>
    int bestMove;

    MOVELIST move_list[1];
    MoveGenerator MoveGen;
    MoveGen.GenerateAllMoves(_B, move_list);
    int _size = move_list->move_count;
    //int random_move = rand() % size;
    //MakeMove(b1,move_list->moves[random_move].move);
    //Iterate through move list
    for(int i = 0;i < _size;i++)
    {
        int move_this = move_list->moves[i].move;
        //Moving
        MakeMove(_B,move_this);
        //Calls minSearch function below, passing depth(variable based on ply) and the board
        //returns a point value
        int value = minSearch( depth-1 , _B);
        //compares the value to the best so far
        if( value > bestValue )
        {
            bestValue = value;
            bestMove = move_this;
        }
        ///Undo Move
        TakeMove(_B);

    }
      //return the best move found by min/maxSearch
    return bestMove;
}


int GameControl::minSearch( int depth , Board *_B )
{
    ///minSearch finds the MINIMUM possible score out of the set of possible moves

    //evaluate score at bottom ply
    if(depth==0)
        return Evaluate(_B);
    //starting at best score instead
    int bestValue = INFINITY;
    //Works similarly to the iteration mechanism in minmax

    MOVELIST move_list[1];
    MoveGenerator MoveGen;
    MoveGen.GenerateAllMoves(_B, move_list);
    int _size = move_list->move_count;

    for(int i = 0;i < _size;i++)
    {
        int move_this = move_list->moves[i].move;
        //Moving
        MakeMove(_B,move_this);
        //send to maxSearch for max score
        int value = maxSearch( depth-1 , _B );
        //compares the value to the best so far
        if( value < bestValue )
            bestValue = value;
    ///Undo Move
        TakeMove(_B);
    }
    return bestValue;
}


int GameControl::maxSearch( int depth , Board *_B )
{
    ///As minSearch, just....for max score

    //return score if depth = 0
    if(depth==0)
        return Evaluate(_B);

    //Start at -INF b/c we want to maximize score
    int bestValue = -INFINITY;

    MOVELIST move_list[1];
    MoveGenerator MoveGen;
    MoveGen.GenerateAllMoves(_B, move_list);
    int _size = move_list->move_count;

    for(int i = 0; i < _size; i++)
    {
        int move_this = move_list->moves[i].move;
        //Moving
        MakeMove(_B,move_this);
        //send to maxSearch for max score
        int value = minSearch( depth-1 , _B );
        //compares the value to the best so far
        if( value > bestValue )
            bestValue = value;
    ///Undo Move
    TakeMove(_B);
    }
    return bestValue;
}

int GameControl::Evaluate(Board *_B)
{
    /// ///This function adds the overall scores of each player based on (currently):
    /// ///Piece type
    /// ///Possible moves
    ///may want to add in a score factor for how strong the current location is?
    float type = 0, mobility = 0;

    //white pieces
    type += _B->piece_number[WHITE_PAWN];
    type += _B->piece_number[WHITE_KNIGHT] * 3.2;
    type += _B->piece_number[WHITE_BISHOP] * 3.33;
    type += _B->piece_number[WHITE_ROOK]   * 5.1;
    type += _B->piece_number[WHITE_QUEEN]  * 8.8;
    if(_B->piece_number[WHITE_KING] == 0)
        type = INFINITY;

    //black piece
    type -= _B->piece_number[BLACK_PAWN];
    type -= _B->piece_number[BLACK_KNIGHT] * 3.2;
    type -= _B->piece_number[BLACK_BISHOP] * 3.33;
    type -= _B->piece_number[BLACK_ROOK]   * 5.1;
    type -= _B->piece_number[BLACK_QUEEN]  * 8.8;
    if(_B->piece_number[BLACK_KING] == 0)
        type = -INFINITY;

//Mobility is much less important that type of piece, but still valid
return type+mobility/20;
}
