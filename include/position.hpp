#pragma once
#include "attack.hpp"
#include <array>
#include <cassert>
#include <types.hpp>
#include <iostream>


template<PieceType pt>
Bitboard attacks_bb(Square sq, Bitboard occupancy)
{
    switch(pt) 
    {
        case(KING)   : return AttackTables::king_attacks[sq]; break;
        case(KNIGHT) : return AttackTables::knight_attacks[sq]; break;
        case(BISHOP) : return AttackTables::get_bishop_attacks(sq, occupancy); break;
        case(ROOK)   : return AttackTables::get_rook_attacks(sq, occupancy); break;
        case(QUEEN)  : return AttackTables::get_queen_attacks(sq, occupancy); break;
    }
}
    

class StateInfo
{
public:
    U8 castling_rights;
    U8 ep_num;
    int half_move;
    int full_move;
    Key hash;

    Bitboard checkers_bb;
    std::array<std::array<Bitboard, NUM_PIECES>, NUM_COLOR> check_squares;
    std::array<Bitboard, NUM_COLOR> blockers_for_king;
    std::array<Bitboard, NUM_COLOR> pinners;
    StateInfo* previous;

};

class Position
{
public: 
//member variables
    std::array<Bitboard, NUM_PIECES> piece_bb;
    std::array<Bitboard, NUM_COLOR> color_bb;
    std::array<Piece, NUM_SQUARES> piece_board;
    Bitboard occupancy;
    Color side_to_move;
    StateInfo state;

    std::array<std::array<Bitboard, NUM_SQUARES>, NUM_SQUARES> between_array= {0ULL};

//member functions

    Position();
    void init();

    void fen_parser(const std::string& fen);



    Bitboard generate_between(Square s1, Square s2);
    Bitboard between_bb(Square s1, Square s2);
    void init_between();
    Bitboard line_bb(Square s1, Square s2);

    Bitboard const get_piece(Color color, PieceType Piece);
    Square const get_king_square(Color color);

    void set_check_info(Color color);
    void set_check_squares(Color color);
    void set_pins_info(Color color);
    Bitboard set_checkers();
    bool is_check();

    
};


