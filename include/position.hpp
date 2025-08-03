#pragma once
#include "attack.hpp"
#include "zobrist.hpp"
#include "move.hpp"
#include <array>
#include <cassert>
#include <types.hpp>



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

    //copy;
    U8 castling_rights = NO_CASTLING;
    U8 ep_num = ep_none;
    int half_move = 0;
    int full_move = 0;
    Piece captured_piece = NONE;
    Key hash;

    //redo each time
    Move move;
    Bitboard checkers_bb = 0ULL;
    std::array<Bitboard, NUM_PIECES> check_squares = {0ULL};  
    Bitboard blockers_for_king = 0ULL;                       
    Bitboard pinners = 0ULL;                                 

    Square ep_num_to_square() const;
    Square square_to_ep_num(Square sq);

    StateInfo();
    StateInfo(const StateInfo& old);
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
    ZobristRandoms zobrist;

    int state_index = 0;
    std::array<StateInfo, MAX_HISTORY> state_history;
    StateInfo& state = state_history[state_index];

    std::array<std::array<Bitboard, NUM_SQUARES>, NUM_SQUARES> between_array= {0ULL};

//member functions
    Position();
    Position(const std::string& fen);
    void init();
    void init_piece_board();
    Key init_hash();
    void fen_parser(const std::string& fen);

    //between array
    Bitboard generate_between(Square s1, Square s2);
    Bitboard between_bb(Square s1, Square s2) const;
    void init_between();
    Bitboard line_bb(Square s1, Square s2) const;

    //utilities
    Bitboard get_piece(Color color, Piece piece) const ;
    Square get_king_square(Color color) const ;
    bool is_check() const;
    bool is_double_check() const;
    U8 can_castle(Color color) const;
    Piece list_to_type(U8 sq);
    Piece type_to_list(Piece piece, Color color);
    void update_occupancy();

    //state / pinners and checkers
    void set_state();
    void set_check_info(Color color);
    void set_check_squares(Color color);
    void set_pins_info(Color color);
    Bitboard set_checkers();

    //do move
    void make_move(Move& move);
    void move_piece(Color color, Piece piece, Square from, Square to);
    void remove_piece(Color color, Piece piece, Square sq);
    void put_piece(Color color, Piece piece, Square sq);
    void swap_sides();
    void update_castling_permissions(U8 permissions);
    void set_epsquare(Square sq);
    void clear_epsquare();
    void castling_support(Color color, Square king_from, Square king_to);

};




void print_piece_board(Position& pos);
void print_state_info(const StateInfo& state);