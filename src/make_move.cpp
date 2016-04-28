/******************************************************************************
*
*	FILE - MAKE_MOVE.CPP
*	PURPORSE - Game Board object function implementations.
* 
*	AUTHOR - Dennis Fogerty
*	DATE - 3/17/2016
*
******************************************************************************/

#include "defs.h"
#include "board.h"
#include <cassert>


#define HASH_PIECE(piece,square) (gameboard->position_key ^= (PieceKeys[(piece)][(square)]))
#define HASH_CASTLE (gameboard->position_key ^= (CastleKeys[(gameboard->castle_permission)]))
#define HASH_SIDE (gameboard->position_key ^= (SideKey))
#define HASH_ENPASSANT (gameboard->position_key ^= (PieceKeys[EMPTY][(gameboard->en_passant_square)]))

const int castle_permission_squares[120] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

static void ClearPiece(const int square, Board *gameboard) {

    assert(SquareIsOnBoard(square));
    assert(gameboard->ValidBoard());

    int piece = gameboard->pieces[square];
        
    assert(ValidPiece(piece));

    int color = PieceColor[piece];
    int index = 0;
    int tmp_piece_number = -1;
        
    assert(ValidSide(color));

    HASH_PIECE(piece,square);
    
    gameboard->pieces[square] = EMPTY;
    gameboard->material_score[color] -= PieceValues[piece];
    
    if (IsBigPiece[piece]) {
            gameboard->big_pieces_count[color]--;
        if (IsMajorPiece[piece]) {
            gameboard->major_pieces_count[color]--;
        } 
        else {
            gameboard->minor_pieces_count[color]--;
        }
    } 
    else {
        CLEARBIT(gameboard->pawns[color],SQ64_TO_SQ120(square));
        CLEARBIT(gameboard->pawns[BOTH],SQ64_TO_SQ120(square));
    }
    
    for (index = 0; index < gameboard->piece_number[piece]; ++index) {
        if (gameboard->piece_list[piece][index] == square) {
            tmp_piece_number = index;
            break;
        }
    }
    
    assert(tmp_piece_number != -1);
    assert(tmp_piece_number>=0 && tmp_piece_number<10);
    
    gameboard->piece_number[piece]--;     
    
    gameboard->piece_list[piece][tmp_piece_number] = gameboard->piece_list[piece][gameboard->piece_number[piece]];
}

static void AddPiece(const int square, Board *gameboard, const int piece) {
    
    assert(ValidPiece(piece));
    assert(SquareIsOnBoard(square));

    int color = PieceColor[piece];
    assert(ValidSide(color));

    HASH_PIECE(piece,square);
    
    gameboard->pieces[square] = piece;

    if (IsBigPiece[piece]) {
            gameboard->big_pieces_count[color]++;
        if (IsMajorPiece[piece]) {
            gameboard->major_pieces_count[color]++;
        } 
        else {
            gameboard->minor_pieces_count[color]++;
        }
    } 
    else {
        SETBIT(gameboard->pawns[color],SQ64_TO_SQ120(square));
        SETBIT(gameboard->pawns[BOTH],SQ64_TO_SQ120(square));
    }
    
    gameboard->material_score[color] += PieceValues[piece];
    gameboard->piece_list[piece][gameboard->piece_number[piece]++] = square;  
}

static void MovePiece(const int from_square, const int to_square, Board *gameboard) {    
    
    assert(SquareIsOnBoard(from_square));
    assert(SquareIsOnBoard(to_square));
    
    int index = 0;
    int piece = gameboard->pieces[from_square];    
    int color = PieceColor[piece];

    assert(ValidSide(color));
    assert(ValidPiece(piece));
    
    HASH_PIECE(piece,from_square);
    gameboard->pieces[from_square] = EMPTY;
    
    HASH_PIECE(piece,to_square);
    gameboard->pieces[to_square] = piece;

    if (!IsBigPiece[piece]) {
        CLEARBIT(gameboard->pawns[color],SQ64_TO_SQ120(from_square));
        CLEARBIT(gameboard->pawns[BOTH],SQ64_TO_SQ120(from_square));
        SETBIT(gameboard->pawns[color],SQ64_TO_SQ120(to_square));
        SETBIT(gameboard->pawns[BOTH],SQ64_TO_SQ120(to_square));      
    }    
    
    for (index = 0; index < gameboard->piece_number[piece]; ++index) {
        if (gameboard->piece_list[piece][index] == from_square) {
            gameboard->piece_list[piece][index] = to_square;
            break;
        }
    }
}

void TakeMove(Board *gameboard) {
        
    assert(gameboard->ValidBoard());

    gameboard->ply_history--;
    gameboard->ply_count--;
    
    assert(gameboard->ply_history >= 0 && gameboard->ply_history < MAXGAMEMOVES);
    assert(gameboard->ply_count >= 0 && gameboard->ply_count < MAXDEPTH);
    
    int move = gameboard->move_history[gameboard->ply_history].move;
    int from_square = FROMSQUARE(move);
    int to_square = TOSQUARE(move);    
    
    if (gameboard->en_passant_square != NO_SQUARE) 
        HASH_ENPASSANT;

    HASH_CASTLE;

    gameboard->castle_permission = gameboard->move_history[gameboard->ply_history].castle_permission;
    gameboard->fifty_move_count = gameboard->move_history[gameboard->ply_history].fifty_move_count;
    gameboard->en_passant_square = gameboard->move_history[gameboard->ply_history].en_passant_square;

    if (gameboard->en_passant_square != NO_SQUARE) 
        HASH_ENPASSANT;
    
    HASH_CASTLE;

    gameboard->side_to_move ^= 1;
    HASH_SIDE;
    
    if (MOVEFLAGENPASSANT & move) {
        if (gameboard->side_to_move == WHITE) {
            AddPiece(to_square-10, gameboard, BLACK_PAWN);
        } 
        else {
            AddPiece(to_square+10, gameboard, WHITE_PAWN);
        }
    } 
    else if (MOVEFLAGCASTLE & move) {
        switch(to_square) {
            case C1: MovePiece(D1, A1, gameboard); break;
            case C8: MovePiece(D8, A8, gameboard); break;
            case G1: MovePiece(F1, H1, gameboard); break;
            case G8: MovePiece(F8, H8, gameboard); break;
            default: assert(false); break;
        }
    }
    
    MovePiece(to_square, from_square, gameboard);
    
    if (PieceIsKing[gameboard->pieces[from_square]]) {
        gameboard->king_square_location[gameboard->side_to_move] = from_square;
    }
    
    int captured_piece = CAPTURED(move);
    if (captured_piece != EMPTY) {
        assert(ValidPiece(captured_piece));
        AddPiece(to_square, gameboard, captured_piece);
    }
    
    if (PROMOTED(move) != EMPTY) {
        assert(ValidPiece(PROMOTED(move)) && !PieceIsPawn[PROMOTED(move)]);
        ClearPiece(from_square, gameboard);
        AddPiece(from_square, gameboard, (PieceColor[PROMOTED(move)] == WHITE ? WHITE_PAWN : BLACK_PAWN));
    }

    assert(gameboard->ValidBoard());
    
}

int MakeMove(Board *gameboard, int move) {

    assert(gameboard->ValidBoard());

    int from_square = FROMSQUARE(move);
    int to_square = TOSQUARE(move);
    int side = gameboard->side_to_move;

    assert(SquareIsOnBoard(from_square));
    assert(SquareIsOnBoard(to_square));
    assert(ValidSide(side));
    if (ValidPiece(gameboard->pieces[from_square]))
        int x;
    else {
        gameboard->PrintBoard();
        PrintMove(move);
    }

    assert(gameboard->ply_history >= 0 && gameboard->ply_history < MAXGAMEMOVES);
    assert(gameboard->ply_count >= 0 && gameboard->ply_count < MAXDEPTH);
    
    gameboard->move_history[gameboard->ply_history].position_key = gameboard->position_key;
    
    if (move & MOVEFLAGENPASSANT) {
        if (side == WHITE) {
            ClearPiece(to_square-10, gameboard);
        } 
        else {
            ClearPiece(to_square+10, gameboard);
        }
    } 
    else if (move & MOVEFLAGCASTLE) {
        switch(to_square) {
            case C1:
                MovePiece(A1, D1, gameboard);
            break;
            case C8:
                MovePiece(A8, D8, gameboard);
            break;
            case G1:
                MovePiece(H1, F1, gameboard);
            break;
            case G8:
                MovePiece(H8, F8, gameboard);
            break;
            default: assert(false); break;
        }
    }   
    
    if (gameboard->en_passant_square != NO_SQUARE) 
        HASH_ENPASSANT;
    
    HASH_CASTLE;
    
    // Store move info in history
    gameboard->move_history[gameboard->ply_history].move = move;
    gameboard->move_history[gameboard->ply_history].fifty_move_count = gameboard->fifty_move_count;
    gameboard->move_history[gameboard->ply_history].en_passant_square = gameboard->en_passant_square;
    gameboard->move_history[gameboard->ply_history].castle_permission = gameboard->castle_permission;

    // Use array at top of file to check if castle squares have been affected
    gameboard->castle_permission &= castle_permission_squares[from_square];
    gameboard->castle_permission &= castle_permission_squares[to_square];
    gameboard->en_passant_square = NO_SQUARE;

    HASH_CASTLE;
    
    int captured = CAPTURED(move);
    gameboard->fifty_move_count++;
    
    if (captured != EMPTY) {
        assert(ValidPiece(captured));
        ClearPiece(to_square, gameboard);
        gameboard->fifty_move_count = 0;
    }
    
    gameboard->ply_history++;
    gameboard->ply_count++;
    
    assert(gameboard->ply_history >= 0 && gameboard->ply_history < MAXGAMEMOVES);
    assert(gameboard->ply_count >= 0 && gameboard->ply_count < MAXDEPTH);
    
    // Check if new EnPassant Square
    if (PieceIsPawn[gameboard->pieces[from_square]]) {
        gameboard->fifty_move_count = 0;
        if (move & MOVEFLAGPAWNSTART) {
            if (side == WHITE) {
                gameboard->en_passant_square=from_square+10;
                assert(rank_of[gameboard->en_passant_square]==RANK_3);
            } 
            else {
                gameboard->en_passant_square=from_square-10;
                assert(rank_of[gameboard->en_passant_square]==RANK_6);
            }
            HASH_ENPASSANT;
        }
    }
    
    MovePiece(from_square, to_square, gameboard);
    
    int promoted_piece = PROMOTED(move);
    if (promoted_piece != EMPTY) {
        assert(ValidPiece(promoted_piece) && !PieceIsPawn[promoted_piece]);
        ClearPiece(to_square, gameboard);
        AddPiece(to_square, gameboard, promoted_piece);
    }
    
    if (PieceIsKing[gameboard->pieces[to_square]]) {
        gameboard->king_square_location[gameboard->side_to_move] = to_square;
    }
    
    // Change side to move
    gameboard->side_to_move ^= 1;
    HASH_SIDE;

    assert(gameboard->ValidBoard());
    
    // Do not allow moves that place own KING in check
    if (IsSquareAttacked(gameboard->king_square_location[side],gameboard->side_to_move,gameboard)) {
        std::cout << "DONT PUT YOUR KING IN CHECK DUMBASS" << std::endl;
        TakeMove(gameboard);
        return false;
    }
    
    return true;
    
}
