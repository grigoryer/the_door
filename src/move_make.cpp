#include "position.hpp"
#include "types.hpp"
#include <iostream>


void Position::make_move(Move move)
{
    // update state history index and copy previous state to current state->
    prepare_state(move);

    const Square us = side_to_move;
    const Square enemy = side_to_move ^ 1;
    const U8 from = move.get_from();
    const U8 to = move.get_to();
    Piece piece = move.get_piece();
    
    handle_clock(piece);
    handle_specials(move, us, enemy, from, to, &piece);
    move_piece(us, piece, from, to);
    swap_sides();

    set_check_info(enemy);
}

void Position::prepare_state(Move move)
{
    state_index++;
    StateInfo& prev_state = state_history[state_index - 1];
    prev_state.move = move;
    state = &state_history[state_index];
    *state = prev_state;                 
}

void Position::handle_clock(Piece piece)
{
    state->half_move++;
    if(side_to_move == BLACK){ state->full_move++; }
    if(piece == PAWN) { state->half_move = 0; }
}

void Position::handle_specials(Move& move, Color us, Color enemy, Square from, Square to, Piece* piece)
{
    set_epsquare(ep_none);

    if(move.is_double()) { set_epsquare(state->square_to_ep_num(to)); }

    if(move.is_capture()) 
    { 
        state->half_move = 0;
        state_history[state_index - 1].captured_piece = list_to_type(to);
        remove_piece(enemy, state_history[state_index - 1].captured_piece,  to); 

    }
    
    if(move.is_castling()) { castling_support(us, from, to); }

    if(move.is_enpassant()) { remove_piece(enemy, PAWN,  to + (enemy == BLACK ? SOUTH : NORTH)); }

    if(state->castling_rights != 0 && (*piece == ROOK || *piece == KING)) { castling_permissions_support(us, from, *piece); }
    
    if(move.is_promoted()) 
    {
        remove_piece(us, PAWN, from);
        *piece = move.get_promoted(); 
    } 
    
}

void Position::castling_permissions_support(Color color, Square from, Piece piece)
{
    if(piece == ROOK) 
    {
        U8 new_castling = state->castling_rights;

        if(from == h1) { new_castling &= ~WK; }      // White kingside
        else if(from == a1) { new_castling &= ~WQ; } // White queenside  
        else if(from == h8) { new_castling &= ~BK; } // Black kingside
        else if(from == a8) { new_castling &= ~BQ; } // Black queenside
        
        update_castling_permissions(new_castling);
    }

    if (piece == KING) 
    {
        update_castling_permissions(state->castling_rights & ~(color == WHITE ? WHITE_CASTLING : BLACK_CASTLING));
    }
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
        update_castling_permissions(state->castling_rights & ~WHITE_CASTLING);
    } 
    else 
    {
        rook_from = is_kingside ? h8 : a8;
        rook_to = is_kingside ? f8 : d8;
        update_castling_permissions(state->castling_rights & ~BLACK_CASTLING);
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
    state->hash ^= zobrist.piece(color, piece, sq);
}

void Position::put_piece(Color color, Piece piece, Square sq)
{
    set_bit(piece_bb[piece], sq);
    set_bit(color_bb[color], sq);
    set_bit(occupancy, sq);
    piece_board[sq] = piece + (color == WHITE ? 0 : NUM_PIECES);
    state->hash ^= zobrist.piece(color, piece, sq);
}

void Position::swap_sides()
{
    state->hash ^= zobrist.color(side_to_move);
    side_to_move ^= 1;
    state->hash ^= zobrist.color(side_to_move);
}

void Position::update_castling_permissions(U8 permissions)
{
    state->hash ^= zobrist.castling(state->castling_rights);
    state->castling_rights = permissions;
    state->hash ^= zobrist.castling(state->castling_rights);
}

void Position::set_epsquare(Square sq)
{
    state->hash ^= zobrist.en_passant(state->ep_num);
    state->ep_num = sq;
    state->hash ^= zobrist.en_passant(state->ep_num);
}

void Position::clear_epsquare()
{
    state->hash ^= zobrist.en_passant(state->ep_num);
    state->ep_num = ep_none;
    state->hash ^= zobrist.en_passant(state->ep_num);
}