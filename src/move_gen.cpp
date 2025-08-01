#include "move_gen.hpp"
#include "attack.hpp"
#include "move.hpp"
#include "position.hpp"
#include "types.hpp"



template<GenType Type>
void gen_promo(MoveList& move_list, Bitboard target, bool capture)
{



}


template<GenType Type, Colors color>
void gen_pawns(const Position& pos, MoveList& move_list, Bitboard target)
{
    const Direction up         = (color == WHITE ? NORTH : SOUTH);
    const Direction up_right   = (color == WHITE ? NORTH_EAST : SOUTH_WEST);
    const Direction up_left    = (color == WHITE ? NORTH_WEST : SOUTH_EAST);
    const Bitboard promo_rank  = (color == WHITE ? Rank7BB : Rank2BB);
    const Bitboard double_rank = (color == WHITE ? Rank3BB : Rank6BB);
    const Color enemy = color ^ 1;




    Bitboard pawns = pos.get_piece(color, PAWN);

    while(pawns)
    {
        Bitboard 
    }


}

template<PieceType pt, GenType Type>
void gen_piece(const Position& pos, MoveList& move_list, Bitboard target)
{

    Color us = pos.side_to_move;
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

                if constexpr (Type == CAPTURES || Type == ALL)
                {
                    move_list.add(Move::create<CAPTURE>(sq, to, pt));
                }
                
                if constexpr (Type == QUIETS || Type == ALL)
                {
                    move_list.add(Move::create<QUIET>(sq, to, pt));
                }

            }
        }
    }
}





template<GenType Type>
void generate_all(const Position& pos, MoveList& move_list, Bitboard target)
{

}


template<GenType Type>
void generate(const Position& pos, MoveList& move_list)
{
    
}


template void generate<CAPTURES>(const Position& pos, MoveList& move_list);
template void generate<NON_EVASIONS>(const Position& pos, MoveList& move_list);
template void generate<EVASIONS>(const Position& pos, MoveList& move_list);
template void generate<LEGAL>(const Position& pos, MoveList& move_list);