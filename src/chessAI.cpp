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


int INFINITY = numeric_limits<int>::max();

GameControl::GameControl(){

};

int GameControl::minimax(Board *_B, int depth)
{
    /// ///MinMax tree. Currently does NOT use Alpha-Beta Pruning or horizon effect reduction e.g. Quiescence search
    /// Note: Currently assumes AI plays WHITE
    ///int depth is the ply at which we do the search. Start at one, move on down through minSearch/maxSearch

    //assume worse score initially; "win" is +INF ONLY IF side to move is white
    //If side is black, we want the low score to win
    int bestValue=0;
    if(_B->side_to_move == WHITE){
        bestValue = -INFINITY;
    }else if(_B->side_to_move == BLACK){
        INFINITY = -INFINITY;
        bestValue = -INFINITY;
    }

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
    float type = 0, mobility = 0, control_ = 0;
    int KnightCoeff=0, BishopCoeff=0, RookCoeff=0, QueenCoeff;


    ///--material-------------------------------------------
    //white pieces
    type += _B->piece_number[WHITE_PAWN];

    if(_B->piece_number[WHITE_PAWN] > 5){
        KnightCoeff = 3.2;
        BishopCoeff = 3.33;
        RookCoeff = 5.1;
        QueenCoeff = 8.8;
    }else if(_B->piece_number[WHITE_PAWN] > 2){
        KnightCoeff = 2.2;
        BishopCoeff = 3.33;
        RookCoeff = 6.1;
        QueenCoeff = 8.9;
    }else{
        KnightCoeff = 1.7;
        BishopCoeff = 3.33;
        RookCoeff = 7.1;
        QueenCoeff = 9.1;
    }

    type += _B->piece_number[WHITE_KNIGHT] * KnightCoeff;
    type += _B->piece_number[WHITE_BISHOP] * BishopCoeff;
    type += _B->piece_number[WHITE_ROOK]   * RookCoeff;
    type += _B->piece_number[WHITE_QUEEN]  * QueenCoeff;
    if(_B->piece_number[WHITE_KING] == 0)
        type = INFINITY;

    //black piece

    type -= _B->piece_number[BLACK_PAWN];
    //As pawns are lost, values of pieces change
    if(_B->piece_number[BLACK_PAWN] > 5){
        KnightCoeff = 3.2;
        BishopCoeff = 3.33;
        RookCoeff = 5.1;
        QueenCoeff = 8.8;
    }else if(_B->piece_number[BLACK_PAWN] > 2){
        KnightCoeff = 2.2;
        BishopCoeff = 3.33;
        RookCoeff = 6.1;
        QueenCoeff = 8.9;
    }else{
        KnightCoeff = 1.7;
        BishopCoeff = 3.33;
        RookCoeff = 7.1;
        QueenCoeff = 9.1;
    }

    type -= _B->piece_number[BLACK_KNIGHT] * 3.2;
    type -= _B->piece_number[BLACK_BISHOP] * 3.33;
    type -= _B->piece_number[BLACK_ROOK]   * 5.1;
    type -= _B->piece_number[BLACK_QUEEN]  * 8.8;
    if(_B->piece_number[BLACK_KING] == 0)
        type = -INFINITY;

    ///--mobility--------------------------------------------
    MOVELIST move_list[1];
    MoveGenerator MoveGen;
    MoveGen.GenerateAllMoves(_B, move_list);
    int _size = move_list->move_count;

    mobility += _size;

    _B->side_to_move = !_B->side_to_move;
    MoveGen.GenerateAllMoves(_B, move_list);
    _size = move_list->move_count;

    mobility += _size;

    _B->side_to_move = !_B->side_to_move;


    //((21+file) + (rank * 10))

    ///--center control---------------------------------------------
    //white first
    //center is best
    MoveGen.GenerateAllMoves(_B,move_list);
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('d' - 'a', '4' - '1')] ] == WHITE){
        control_ += 3.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('e' - 'a', '4' - '1')] ] == WHITE){
        control_ += 3.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('d' - 'a', '5' - '1')] ] == WHITE){
        control_ += 4.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('e' - 'a', '5' - '1')] ] == WHITE){
        control_ += 4.3;
    }

    //8 around center
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('c' - 'a', '4' - '1')] ] == WHITE){
        control_ += 2.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('d' - 'a', '3' - '1')] ]== WHITE){
        control_ += 2.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('e' - 'a', '3' - '1')] ]== WHITE){
        control_ += 2.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('f' - 'a', '4' - '1')] ]== WHITE){
        control_ += 2.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('c' - 'a', '5' - '1')] ]== WHITE){
        control_ += 3.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('d' - 'a', '6' - '1')] ]== WHITE){
        control_ += 3.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('e' - 'a', '6' - '1')] ]== WHITE){
        control_ += 3.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('f' - 'a', '5' - '1')] ]== WHITE){
        control_ += 3.3;
    }

    //"outer" 12
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('b' - 'a', '4' - '1')] ]== WHITE){
        control_ += 1.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('c' - 'a', '3' - '1')] ]== WHITE){
        control_ += 1.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('d' - 'a', '2' - '1')] ]== WHITE){
        control_ += 1.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('e' - 'a', '2' - '1')] ]== WHITE){
        control_ += 1.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('f' - 'a', '3' - '1')] ]== WHITE){
        control_ += 1.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('g' - 'a', '4' - '1')] ]== WHITE){
        control_ += 1.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('b' - 'a', '5' - '1')] ]== WHITE){
        control_ += 2.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('c' - 'a', '6' - '1')] ]== WHITE){
        control_ += 2.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('d' - 'a', '7' - '1')] ]== WHITE){
        control_ += 2.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('e' - 'a', '7' - '1')] ]== WHITE){
        control_ += 2.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('f' - 'a', '6' - '1')] ]== WHITE){
        control_ += 2.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('g' - 'a', '5' - '1')] ]== WHITE){
        control_ += 2.3;
    }

    //and black
    //center!
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('d' - 'a', '4' - '1')] ]== BLACK){
        control_ -= 4.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('e' - 'a', '4' - '1')] ]== BLACK){
        control_ -= 4.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('d' - 'a', '5' - '1')] ]== BLACK){
        control_ -= 3.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('e' - 'a', '5' - '1')] ]== BLACK){
        control_ -= 3.7;
    }

    //8 around center
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('c' - 'a', '4' - '1')] ]== BLACK){
        control_ -= 3.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('d' - 'a', '3' - '1')] ]== BLACK){
        control_ -= 3.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('e' - 'a', '3' - '1')] ]== BLACK){
        control_ -= 3.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('f' - 'a', '4' - '1')] ]== BLACK){
        control_ -= 3.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('c' - 'a', '5' - '1')] ]== BLACK){
        control_ -= 2.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('d' - 'a', '6' - '1')] ]== BLACK){
        control_ -= 2.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('e' - 'a', '6' - '1')] ]== BLACK){
        control_ -= 2.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('f' - 'a', '5' - '1')] ]== BLACK){
        control_ -= 2.7;
    }

    //"outer" 12
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('b' - 'a', '4' - '1')] ]== BLACK){
        control_ -= 2.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('c' - 'a', '3' - '1')] ]== BLACK){
        control_ -= 2.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('d' - 'a', '2' - '1')] ]== BLACK){
        control_ -= 2.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('e' - 'a', '2' - '1')] ]== BLACK){
        control_ -= 2.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('f' - 'a', '3' - '1')] ]== BLACK){
        control_ -= 2.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('g' - 'a', '4' - '1')] ]== BLACK){
        control_ -= 2.3;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('b' - 'a', '5' - '1')] ]== BLACK){
        control_ -= 1.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('c' - 'a', '6' - '1')] ]== BLACK){
        control_ -= 1.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('d' - 'a', '7' - '1')] ]== BLACK){
        control_ -= 1.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('e' - 'a', '7' - '1')] ]== BLACK){
        control_ -= 1.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('f' - 'a', '6' - '1')] ]== BLACK){
        control_ -= 1.7;
    }
    if(   PieceColor[_B->pieces[CONVERT_FILE_AND_RANK_TO_SQUARE('g' - 'a', '5' - '1')] ]== BLACK){
        control_ -= 1.7;
    }


//Mobility is much less important that type of piece, but still valid



return type+mobility/10 + control_;
}
