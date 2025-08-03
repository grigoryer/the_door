#include "move_gen.hpp"
#include "attack.hpp"
#include "move.hpp"
#include "position.hpp"
#include "types.hpp"



template<GenType Type>
void gen_promo(MoveList& move_list, Square from, Square to, bool capture)
{
    if constexpr (Type == CAPTURES || Type == NON_EVASIONS)
    {
        move_list.add(Move(from, to, PAWN, QUEEN, capture, false, false, true));
    }
    if constexpr (Type == QUIETS || Type == NON_EVASIONS)
    {
        move_list.add(Move(from, to, PAWN, KNIGHT, capture, false, false, true));
        move_list.add(Move(from, to, PAWN, BISHOP, capture, false, false, true));
        move_list.add(Move(from, to, PAWN, ROOK, capture, false, false, true));
    }
}


template<GenType Type, Colors color>
void gen_pawns(const Position& pos, MoveList& move_list, Bitboard target)
{
    const Direction up         = (color == WHITE ? NORTH : SOUTH);
    const Direction up_right   = (color == WHITE ? NORTH_EAST : SOUTH_WEST);
    const Direction up_left    = (color == WHITE ? NORTH_WEST : SOUTH_EAST);
    const Bitboard promo_rank  = (color == WHITE ? Rank7BB : Rank2BB);
    const Bitboard double_rank = (color == WHITE ? Rank3BB : Rank6BB);
    const Bitboard enemies     = pos.color_bb[color ^ 1];

    Bitboard regular_bb = pos.get_piece(color, PAWN) & ~promo_rank;
    Bitboard promo_bb = pos.get_piece(color, PAWN) & promo_rank;
        
    // single. and double pushes
    if constexpr (Type != CAPTURES)
    {
        Bitboard single_push = (shift<up>(regular_bb) & ~pos.occupancy);
        Bitboard double_push = (shift<up>(single_push & double_rank) & ~pos.occupancy);

        if constexpr (Type == EVASIONS)
        {
            single_push &= target;
            double_push &= target;
        }

        while(single_push)
        {
            Square to = pop_lsb(single_push);
            move_list.add(Move::create<QUIET>(to - up, to,  PAWN));
        }
        while(double_push)
        {
            Square to = pop_lsb(double_push);
            move_list.add(Move::create<DOUBLE>(to - up - up, to,  PAWN));
        }
    }

    //captures
    if constexpr (Type != QUIETS)
    {
        Bitboard eat_right = (shift<up_right>(regular_bb) & enemies);
        Bitboard eat_left = (shift<up_left>(regular_bb) & enemies);

        if constexpr (Type == EVASIONS)
        {
            eat_right &= target;
            eat_left &= target;
        }

        while(eat_right)
        {
            Square to = pop_lsb(eat_right);
            move_list.add(Move::create<CAPTURE>(to - up_right, to,  PAWN));
        }

        while(eat_left)
        {
            Square to = pop_lsb(eat_left);
            move_list.add(Move::create<CAPTURE>(to - up_left, to,  PAWN));
        }

        //en passant
        if(pos.state.ep_num != ep_none)
        {
            Square ep = pos.state.ep_num_to_square();
            Bitboard ep_attacks = AttackTables::pawn_attacks[color ^ 1][ep] & regular_bb;

            while(ep_attacks)
            {
                move_list.add(Move::create<ENPASSANT>(pop_lsb(ep_attacks),ep,PAWN));
            }
        }
    }

    //promotions
    if (promo_bb)
    {
        Bitboard single = (shift<up>(promo_bb) & ~pos.occupancy);
        Bitboard eat_right = (shift<up_right>(promo_bb) & enemies);
        Bitboard eat_left = (shift<up_left>(promo_bb) & enemies);

        if constexpr( Type == EVASIONS)
        {
            single &= target;
        }
        
        while(single)
        {
            Square to = pop_lsb(single);
            gen_promo<Type>(move_list, to - up, to, false);
        }

        while(eat_right)
        {
            Square to = pop_lsb(eat_right);
            gen_promo<Type>(move_list, to - up_right, to, true);
        }

        while(eat_left)
        {
            Square to = pop_lsb(eat_left);
            gen_promo<Type>(move_list, to - up_left, to, true);
        }
        
    }
}

template<PieceType pt, Color us>
void gen_piece(const Position& pos, MoveList& move_list, Bitboard target)
{
    Color enemy = us ^ 1;
    Bitboard piece = pos.get_piece(us,pt);

    while(piece)
    {
        Square sq = pop_lsb(piece);
        Bitboard attacks = attacks_bb<pt>(sq , pos.occupancy);

        while(attacks)
        {
            Square to = pop_lsb(attacks);

            if(target & (1ULL << to))
            {   
                move_list.add(Move(sq, to, pt, NONE, get_bit(pos.occupancy, to)));
            }
        }
    }
}


template<GenType Type, Colors us>
void generate_all(const Position& pos, MoveList& move_list)
{
    Color enemy = us ^ 1;
    Bitboard target = FullBB;
    Square ksq = pos.get_king_square(us);

    if(!pos.is_double_check())
    {

        if (Type == EVASIONS) 
        { 
            target = pos.line_bb(ksq, lsb(pos.state.checkers_bb)); 
        }

        target &= ~pos.color_bb[us];

        if (Type == CAPTURES) { target &= pos.color_bb[enemy]; }
        if (Type == QUIETS) { target &= ~pos.color_bb[enemy]; }

        gen_pawns<Type, us>(pos, move_list, target);
        gen_piece<QUEEN, us>(pos, move_list, target);
        gen_piece<ROOK, us>(pos, move_list, target);
        gen_piece<BISHOP, us>(pos, move_list, target);
        gen_piece<KNIGHT, us>(pos, move_list, target);
    }

    Bitboard king_attacks = attacks_bb<KING>(ksq, pos.occupancy) & (Type == EVASIONS ? ~pos.color_bb[us] : target);

    while(king_attacks)
    {
        move_list.add(Move(ksq, pop_lsb(king_attacks), KING, NONE));
    }

    if constexpr (Type != EVASIONS)
    {
        const U8 king_side  = (us == WHITE ? WK : BK);
        const U8 queen_side = (us == WHITE ? WQ : BQ);
        const Square from  = (us == WHITE ? e1 : e8);
        const Square to_king = (us == WHITE ? g1 : g8);
        const Square to_queen = (us == WHITE ? c1 : c8);
        
        U8 cr = pos.can_castle(us);
        
        if((cr & king_side) == king_side) { move_list.add(Move::create<CASTLE>(from, to_king, KING)); }
        if((cr & queen_side) == queen_side) { move_list.add(Move::create<CASTLE>(from, to_queen, KING)); }
    }
}




template<GenType Type>
void generate(const Position& pos, MoveList& move_list)
{
    Color us = pos.side_to_move;

    if(us == WHITE)
    {
        generate_all<Type, WHITE>(pos, move_list);
    }
    else
    {
        generate_all<Type, BLACK>(pos, move_list);
    }

}

template void generate<QUIETS>(const Position& pos, MoveList& move_list);
template void generate<CAPTURES>(const Position& pos, MoveList& move_list);
template void generate<NON_EVASIONS>(const Position& pos, MoveList& move_list);
template void generate<EVASIONS>(const Position& pos, MoveList& move_list);
template void generate<LEGAL>(const Position& pos, MoveList& move_list);