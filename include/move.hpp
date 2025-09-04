#pragma once
#include "types.hpp"
#include <array>
#include <algorithm>
#include <iostream>


// Helper arrays for converting square index to file/rank
inline const char files[] = "abcdefgh";
inline const char ranks[] = "12345678";

static const U8 to_shift        = 6;
static const U8 piece_shift     = 12;
static const U8 promoted_shift  = 16;
static const U8 capture_shift   = 20;
static const U8 double_shift    = 21;
static const U8 enpassant_shift = 22;
static const U8 castling_shift  = 23;

// Bit masks for move encoding
constexpr U32 FROM_MASK       = 0x3F;        // 6 bits
constexpr U32 TO_MASK         = 0xFC0;       // 6 bits (shifted by 6)
constexpr U32 PIECE_MASK      = 0xF000;      // 4 bits (shifted by 12)
constexpr U32 PROMOTED_MASK   = 0xF0000;     // 4 bits (shifted by 16)
constexpr U32 CAPTURE_MASK    = 0x100000;    // 1 bit
constexpr U32 DOUBLE_MASK     = 0x200000;    // 1 bit
constexpr U32 ENPASSANT_MASK  = 0x400000;    // 1 bit
constexpr U32 CASTLING_MASK   = 0x800000;    // 1 bit

const Move null_move = 0xffffffff;

inline Move create_move(Square from, Square to, Piece piece, MoveType mt, bool capture = false, Piece promoted = NONE) {

    Move move = from | (to << to_shift) | (piece << piece_shift) | (promoted << promoted_shift) | (capture << capture_shift);
    
    switch(mt) {
        case DOUBLE:  move |= DOUBLE_MASK; break;
        case ENPASSANT: move |= ENPASSANT_MASK; break;
        case CASTLE: move |= CASTLING_MASK; break;
        default: break; 
    }

    return move;
}

inline Square move_get_from(U32 move)       { return move & FROM_MASK; }
inline Square move_get_to(U32 move)         { return (move & TO_MASK) >> to_shift; }
inline Piece move_get_piece(U32 move)       { return (move & PIECE_MASK) >> piece_shift; }
inline Piece move_get_promoted(U32 move)    { return (move & PROMOTED_MASK) >> promoted_shift; }

inline bool move_is_capture(U32 move)       { return move & CAPTURE_MASK; }
inline bool move_is_double(U32 move)        { return move & DOUBLE_MASK; }
inline bool move_is_enpassant(U32 move)     { return move & ENPASSANT_MASK; }
inline bool move_is_castling(U32 move)      { return move & CASTLING_MASK; }
inline bool move_is_promoted(U32 move)      { return move_get_promoted(move) != NONE; }


void print_move(Move move);

class MoveList
{
public:
    std::array<U32, MAX_MOVES> move_list;
    int count = 0;

    MoveList();
    void print_all();
    int get_count();
    void add(const U32 move);
    Move get_move(int index);
    void clear();
};