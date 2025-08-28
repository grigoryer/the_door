#pragma once
#include "attack.hpp"
#include "zobrist.hpp"
#include "move.hpp"
#include <array>
#include <cassert>
#include <cstddef>
#include <types.hpp>
#include <unordered_map>



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
    StateInfo* state = &state_history[state_index];
    std::unordered_map<Key, U8> reptition_counter;

    std::array<std::array<Bitboard, NUM_SQUARES>, NUM_SQUARES> between_array = {0ULL};
    std::array<std::array<Bitboard, NUM_SQUARES>, NUM_SQUARES> through_array = {0ULL};


//member functions
    Position();
    Position(const std::string& fen);
    void init();
    void init_piece_board();
    Key init_hash();
    void init_between();
    void fen_parser(const std::string& fen);
    int perft(int depth);
    void perft_divide(int depth);
    int perft_debug(int depth);

    //between array in state.cpp
    Bitboard generate_between(Square s1, Square s2);
    Bitboard between_bb(Square s1, Square s2) const;
    Bitboard line_bb(Square s1, Square s2) const;
    Bitboard generate_through(Square s1, Square s2);
    Bitboard through_bb(Square s1, Square s2) const;

    //utilities
    StateInfo& get_state();
    Bitboard get_piece(Color color, Piece piece) const ;
    Square get_king_square(Color color) const ;
    bool is_check() const;
    bool is_double_check() const;
    bool is_mate(MoveList *ml);
    bool enough_material(Color us);
    bool is_draw();
    bool is_stalemate(MoveList *ml);
    U8 can_castle(Color color) const;
    Piece list_to_type(Square sq);
    Piece type_to_list(Piece piece, Color color);
    void update_occupancy();
    bool is_square_attacked(Square sq, Color color);
    bool is_square_attacked(Square sq, Color color, Bitboard occ);



    //state / pinners and checkers
    bool is_legal(Move m);
    void set_state();
    void set_check_info(Color color);
    void set_check_squares(Color color);
    void set_pins_info(Color color);
    Bitboard set_checkers();

    //do move
    void make_move(Move move);
    void prepare_state(Move move);
    void handle_clock(Piece piece);
    void handle_specials(Move& move, Color us, Color enemy, Square from, Square to, Piece* piece);
    void move_piece(Color color, Piece piece, Square from, Square to);
    void remove_piece(Color color, Piece piece, Square sq);
    void put_piece(Color color, Piece piece, Square sq);
    void swap_sides();
    void update_castling_permissions(U8 permissions);
    void set_epsquare(Square sq);
    void clear_epsquare();
    void castling_support(Color color, Square king_from, Square king_to);
    void castling_permissions_support(Color color, Square from, Piece piece);

    //unmake move 
    void unmake_move();
    void restore_castling(Move move, Color us);
    void restore_enpassant(Move move);

};




void print_piece_board(Position& pos);
void print_state_info(const Position* pos);
void test_correct();