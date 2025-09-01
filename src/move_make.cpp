#include "position.hpp"
#include "types.hpp"
#include "move.hpp"
#include <iostream>


void Position::make_move(Move move)
{
    // update state history index and copy previous state to current state->
    prepare_state(move);

    const Square us = side_to_move;
    const Square enemy = side_to_move ^ 1;
    const U8 from = move_get_from(move);
    const U8 to = move_get_to(move);
    Piece piece = move_get_piece(move);

    handle_clock(piece);

    if(state->ep_num != ep_none) { clear_epsquare(); }

    handle_specials(move, us, enemy, from, to, piece);

    if(move_is_promoted(move)) 
    {   
        remove_piece(us, PAWN, from);
        piece = move_get_promoted(move); 
    } 

    move_piece(us, piece, from, to);

    swap_sides();
    
    if(piece == KING)
    {
        set_check_squares(us);
    }
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

void Position::handle_specials(Move& move, Color us, Color enemy, Square from, Square to, Piece piece)
{
    if(move_is_double(move)) { set_epsquare(state->square_to_ep_num(to)); return; }

    if(move_is_castling(move)) { castling_support(us, from, to); return; }

    if(move_is_enpassant(move)) { remove_piece(enemy, PAWN,  to + (enemy == BLACK ? SOUTH : NORTH)); return; }
    
    if(move_is_capture(move)) 
    { 
        state->half_move = 0;
        state_history[state_index - 1].captured_piece = list_to_type(to);
        remove_piece(enemy, state_history[state_index - 1].captured_piece,  to); 
        if(state_history[state_index - 1].captured_piece == ROOK)
        {
            castling_permissions_support(enemy, to, ROOK);
        }
    }

    if((piece == ROOK || piece == KING) && state->castling_rights != 0) { castling_permissions_support(us, from, piece); }
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
    state->hash ^= zobrist.castling(permissions);
}

void Position::set_epsquare(Square sq)
{
    state->hash ^= zobrist.en_passant(state->ep_num);
    state->ep_num = sq;
    state->hash ^= zobrist.en_passant(sq);
}

void Position::clear_epsquare()
{
    state->hash ^= zobrist.en_passant(state->ep_num);
    state->ep_num = ep_none;
    state->hash ^= zobrist.en_passant(ep_none);
}

void Position::unmake_move()
{
    assert(state_index > 0);
    Key saved_hash = state_history[state_index - 1].hash;
    swap_sides();
    state_index--;
    state = &state_history[state_index];

    Move move = state->move;
    const Square us = side_to_move;
    const Square enemy = side_to_move ^ 1;
    const U8 from = move_get_from(move);
    const U8 to = move_get_to(move);
    Piece piece = move_get_piece(move);

    
    move_piece(side_to_move, move_get_piece(move), move_get_to(move), move_get_from(move));

    if(move_is_promoted(move))
    {
        remove_piece(us, move_get_promoted(move), to);
    }

    if(move_is_capture(move))
    {
        put_piece(enemy, state->captured_piece, to);
    }

    if(move_is_castling(move))
    {
        const bool is_kingside = (to > from);
        U8 rook_from = 0;
        U8 rook_to = 0;
        
        if (us == WHITE) 
        {
            rook_from = is_kingside ? h1 : a1;
            rook_to = is_kingside ? f1 : d1;
        } 
        else 
        {
            rook_from = is_kingside ? h8 : a8;
            rook_to = is_kingside ? f8 : d8;
        }
        // Move rook back
        move_piece(us, ROOK, rook_to, rook_from);
    }

    if(move_is_enpassant(move))
    {
        Square ep_square = StateInfo::ep_num_to_square(state->ep_num);
        const U8 captured_square = ep_square + (us == WHITE ? SOUTH : NORTH);
        put_piece(enemy, PAWN, captured_square);
    }

    state->hash = saved_hash;
}