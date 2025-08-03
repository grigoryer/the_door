#include "position.hpp"
#include "types.hpp"
#include <iostream>


void Position::make_move(Move& move)
{
    // update state history index and copy previous state to current state.
    state_index++;
    StateInfo& prev_state = state_history[state_index - 1];
    state = prev_state;
    state.move = move;
    set_check_info(side_to_move);

    const Square us = side_to_move;
    const Square enemy = side_to_move ^ 1;
    const U8 from = move.get_from();
    const U8 to = move.get_to();
    Piece piece = move.get_piece();

    set_epsquare(ep_none);
    if(move.is_double()) { set_epsquare(state.square_to_ep_num(to)); }

    if(move.is_capture()) 
    { 
        Piece captured = list_to_type(to);
        remove_piece(enemy, captured,  to); 
        state.captured_piece = type_to_list(captured, enemy);
    }
    
    if(move.is_castling()) { castling_support(us, from, to); }

    if(move.is_enpassant()) { remove_piece(enemy, PAWN,  to + (enemy == BLACK ? SOUTH : NORTH)); }

    if(move.is_promoted()) { piece = move.get_promoted(); }

    move_piece(us, piece,  from,  to);

    swap_sides();
    set_check_info(side_to_move);
}

void Position::castling_support(Color color, Square king_from, Square king_to)
{
    const bool is_kingside = (king_to > king_from);

    U8 rook_from = 0;
    U8 rook_to = 0;

    if (color == WHITE) 
    {
        rook_from = is_kingside ? h1 : a1;
        rook_to = is_kingside ? f1 : d1;
        update_castling_permissions(state.castling_rights & ~WHITE_CASTLING);
    } 
    else 
    {
        rook_from = is_kingside ? h8 : a8;
        rook_to = is_kingside ? f8 : d8;
        update_castling_permissions(state.castling_rights & ~BLACK_CASTLING);
    }

    move_piece(color, ROOK, rook_from, rook_to);
}


void Position::move_piece(Color color, Piece piece, Square from, Square to)
{
    remove_piece(color, piece, from);
    put_piece(color, piece, to);
}

void Position::remove_piece(Color color, Piece piece, Square sq)
{
    pop_bit(piece_bb[piece], sq);
    pop_bit(color_bb[color], sq);
    pop_bit(occupancy, sq);
    piece_board[sq] = no_piece; 
    state.hash ^= zobrist.piece(color, piece, sq);
}

void Position::put_piece(Color color, Piece piece, Square sq)
{
    set_bit(piece_bb[piece], sq);
    set_bit(color_bb[color], sq);
    set_bit(occupancy, sq);
    piece_board[sq] = piece + (color == WHITE ? 0 : NUM_PIECES);; 
    state.hash ^= zobrist.piece(color, piece, sq);
}

void Position::swap_sides()
{
    state.hash ^= zobrist.color(side_to_move);
    side_to_move ^= 1;
    state.hash ^= zobrist.color(side_to_move);
}

void Position::update_castling_permissions(U8 permissions)
{
    state.hash ^= zobrist.castling(state.castling_rights);
    state.castling_rights = permissions;
    state.hash ^= zobrist.castling(state.castling_rights);
}

void Position::set_epsquare(Square sq)
{
    state.hash ^= zobrist.en_passant(state.ep_num);
    state.ep_num = sq;
    state.hash ^= zobrist.en_passant(state.ep_num);
}

void Position::clear_epsquare()
{
    state.hash ^= zobrist.en_passant(state.ep_num);
    state.ep_num = ep_none;
    state.hash ^= zobrist.en_passant(state.ep_num);
}

