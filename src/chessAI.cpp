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

///A-B pruning is not a priority compared to making sure this works in general

using namespace std;

const int INFINITY = numeric_limits<int>::max();



pair<pair<int,int>,pair<int,int>> GameControl::minimax( int depth , U64 _B )
{
    /// ///MinMax tree. Currently does NOT use Alpha-Beta Pruning or horizon effect reduction e.g. Quiescence search
    /// Note: Currently assumes AI plays WHITE
    ///int depth is the ply at which we do the search. Start at one, move on down through minSearch/maxSearch
    short int i,j;
    //assume worse score initially; "win" is +INF
    int bestValue = -INFINITY;

    //Pair of pairs for <starting i,j>, <ending i,j>
    pair<pair<int,int>,pair<int,int>> bestMove;

    vector< pair<int,int> > ::iterator it;

    //Vector of pairs to store possible moves
    vector< pair<int,int> > Z;
    //Board iteration
    for( i = 0; i < 8; i++ )
    {
        for( j = 0; j < 8; j++ )
        {
            //Board != empty and piece is black
            if(_B.Board[i][j]!=EMPTY && _B.Board[i][j]->pieceColor==BLACK )
            {
                Z=_B.Board[i][j]->getPossibleMovesList(i,j,_B);
                //Iterate through move list
                for(it=Z.begin();it!=Z.end();it++)
                {
                    pair<int,int> temp;
                    temp.first=i,temp.second=j;
                    ChessPieces* DestinationPiece;
                    DestinationPiece=_B.Board[(*it).first][(*it).second];
                    //Moving
                    _B.Board[(*it).first][(*it).second]=_B.Board[i][j];
                    _B.Board[i][j]=EMPTY;
                    //Calls minSearch function below, passing depth(variable based on ply) and the board
                    //returns a point value
                    int value = minSearch( depth-1 , _B);
                    //compares the value to the best so far
                    if( value > bestValue )
                    {
                        bestValue = value;
                        bestMove.first.first = i;
                        bestMove.first.second = j;
                        bestMove.second.first = (*it).first;
                        bestMove.second.second = (*it).second;

                    }
                    //Undo Move
                    _B.Board[i][j]=_B.Board[((*it).first)][(*it).second];
                    _B.Board[(*it).first][(*it).second]=DestinationPiece;
                  }
              }
          }
      }
      //return the best move found by minSearch
      return bestMove;
}


int GameControl::minSearch( int depth , U64 _B )
{
    ///minSearch finds the MINIMUM possible score out of the set of possible moves
    short int i;
    short int j;
    //evaluate score at bottom ply
    if(depth==0)
        return Evaluate(_B);
    //starting at best score instead
    int bestValue = INFINITY;
    //board iteration
    for( i = 0; i < 8; i++ ){
        for( j = 0; j < 8; j++ )
        {
            vector< pair<int,int> > ::iterator it;
            //As above, Z is vector of possible dest. moves
            vector< pair<int,int> > Z;
            //checks to see if piece exists and if it is white
            if(_B.Board[i][j]!=EMPTY && _B.Board[i][j]->pieceColor==WHITE)
            {
                //fill Z
                Z=_B.Board[i][j]->getPossibleMovesList(i,j,_B);
                for(it=Z.begin();it!=Z.end();it++)
                {
                    //Works similarly to the iteration mechanism in minmax
                    pair<int,int> temp;
                    temp.first=i;
                    temp.second=j;
                    ///ChessPieces is placeholder name
                    ChessPieces* DestinationPiece;
                    DestinationPiece=_B.Board[(*it).first][(*it).second];
                    // Moving
                    _B.Board[(*it).first][(*it).second]=_B.Board[i][j];
                    _B.Board[i][j]=EMPTY;
                    //send to maxSearch for max score
                    int value = maxSearch( depth-1 , _B );
                    //for min, we want the lowest possible score
                    if( value < bestValue )
                        bestValue = value;
                    //Undo Move
                    _B.Board[i][j]=_B.Board[(*it).first][(*it).second];
                    _B.Board[(*it).first][(*it).second]=DestinationPiece;
                }
            }
        }
    }
    return bestValue;
}


int GameControl::maxSearch( int depth , U64 _B )
{
    ///As minSearch, just....for max score
    short int i;
    short int j;
    //return score if depth = 0
    if(depth==0)
        return Evaluate(_B);
    vector< pair<int,int> > ::iterator it;
    //Vector of board locations
    vector< pair<int,int> > Z;
    //Start at -INF b/c we want to maximize score
    int bestValue = -INFINITY;

    //Board iteration
    for( i = 0; i < 8; i++ )
        for( j = 0; j < 8; j++ )
        {

            if(_B.Board[i][j]!=EMPTY && _B.Board[i][j]->pieceColor==BLACK )
            {
                //Possible move lists
                Z=_B.Board[i][j]->getPossibleMovesList(i,j,_B);
                for(it=Z.begin();it!=Z.end();it++)
                {
                    pair<int,int> temp;

                    temp.first=i,temp.second=j;
                    ChessPieces* DestinationPiece;
                    DestinationPiece=_B.Board[(*it).first][(*it).second];
                    //Moving
                    _B.Board[(*it).first][(*it).second]=_B.Board[i][j];
                    _B.Board[i][j]=EMPTY;
                    //Do a minSearch, because same reasons
                    int value = minSearch( depth-1 , _B );
                    //Want max score
                    if( value > bestValue )
                        bestValue = value;
                    //Undo Move
                    _B.Board[i][j]=_B.Board[((*it).first)][(*it).second];
                    _B.Board[(*it).first][(*it).second]=DestinationPiece;
                }
            }
        }
        return bestValue;
}

int GameControl::Evaluate(U64 _B)
{
    /// ///This function adds the overall scores of each player based on (currently):
    /// ///Piece type
    /// ///Possible moves
    ///may want to add in a score factor for how strong the current location is?
    int material=0,mobility=0;
    //rows & columns
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
        {
            //if space is not empty
            if(_B.Board[i][j]!=EMPTY)
            {
                //white piece
                if(_B.Board[i][j]->pieceColor==WHITE){
                    //points based on piece type
                    material+=-_B.Board[i][j]->Weight;
                    //points based on possible move amounts
                    mobility+=-_B.Board[i][j]->getPossibleMovesList(i,j,_B).size();
                }
                //black piece
                else {
                    material+=_B.Board[i][j]->Weight;
                    mobility+=_B.Board[i][j]->getPossibleMovesList(i,j,_B).size();
                }
            }
        }
        //Mobility is much less important that type of piece, but still valid
        return material+mobility/20;
}
