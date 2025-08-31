#pragma once
#include "types.hpp"
#include <array>
#include <algorithm>




class Move
{
private:
    U32 move;
    static const U32 FROM_MASK = 0x3F;        // 6 bits: 0-5
    static const U32 TO_MASK = 0xFC0;       // 6 bits: 6-11
    static const U32 PIECE_MASK = 0xF000;       // 4 bits: 12-15
    static const U32 PROMOTED_MASK = 0xF0000;   // 4 bits: 16-19
    static const U32 CAPTURE_MASK = 0x100000;   // 1 bit: 20
    static const U32 DOUBLE_MASK = 0x200000;    // 1 bit: 21
    static const U32 ENPASSANT_MASK = 0x400000; // 1 bit: 22
    static const U32 CASTLING_MASK = 0x800000;  // 1 bit: 23

public: 

    Move();
    Move(Square from, Square to, Piece piece, Piece promoted, 
         bool capture = false, bool double_push = false, 
         bool enpassant = false, bool castling = false);

    // Getters
    Square get_from() const;
    Square get_to() const;
    Piece get_piece() const;
    Piece get_promoted() const;
    bool is_capture() const;
    bool is_double() const;
    bool is_enpassant() const;
    bool is_castling() const;
    bool is_promoted() const;

    // Setters
    void set_from(Square source);
    void set_to(Square target);
    void set_piece(Piece piece);
    void set_promoted(Piece promoted);
    void set_capture(bool capture);
    void set_double(bool double_push);
    void set_enpassant(bool enpassant);
    void set_castling(bool castling);

    std::string n_to_sq(Square sq);
    void print_move();


    template<MoveType mt>
    static Move create(Square from, Square to, Piece piece)
    {
        switch(mt)
        {
            case QUIET       : return Move(from, to, piece, NONE, false, false, false, false);
            case(CAPTURE)    : return Move(from, to, piece, NONE, true, false, false, false);
            case(ENPASSANT)  : return Move(from, to, piece, NONE, false, false, true, false);
            case(DOUBLE)     : return Move(from, to, piece, NONE, false, true, false, false);
            case(CASTLE)     : return Move(from, to, piece, NONE, false, false, false, true);
        }
    }

};


class MoveList
{
public:
    std::array<Move, MAX_MOVES> move_list = {};
    int count = 0;

    MoveList();
    void print_all();
    int get_count();
    void add(const Move move);
    void clear();
    Move get_move(int index);
};