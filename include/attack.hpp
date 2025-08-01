#pragma once
#include "types.hpp"
#include <array>

enum KnightDirection : int8_t
{
    NE_LONG   = 17,
    NE_SHORT  = 10,
    NW_SHORT  = 6,
    NW_LONG   = 15,

    SE_LONG   = -NW_LONG,
    SE_SHORT  = -NW_SHORT,
    SW_SHORT  = -NE_SHORT,
    SW_LONG   = -NE_LONG,
};

template<Direction D>
constexpr Bitboard shift(Bitboard b) {

    return D == NORTH         ? b << 8
         : D == SOUTH         ? b >> 8
         : D == NORTH + NORTH ? b << 16
         : D == SOUTH + SOUTH ? b >> 16
         : D == EAST          ? (b & ~FileHBB) << 1
         : D == WEST          ? (b & ~FileABB) >> 1
         : D == NORTH_EAST    ? (b & ~FileHBB) << 9
         : D == NORTH_WEST    ? (b & ~FileABB) << 7
         : D == SOUTH_EAST    ? (b & ~FileHBB) >> 7
         : D == SOUTH_WEST    ? (b & ~FileABB) >> 9
         : 0;
}

template<KnightDirection D>
constexpr Bitboard shift_knight(Bitboard b) {

    return D == NE_LONG   ? (b & ~FileHBB) << 17
         : D == NW_LONG   ? (b & ~FileABB) << 15
         : D == SE_LONG   ? (b & ~FileHBB) >> 15
         : D == SW_LONG   ? (b & ~FileABB) >> 17
         : D == SE_SHORT  ? (b & ~(FileGBB | FileHBB)) >> 6
         : D == SW_SHORT  ? (b & ~(FileABB | FileBBB)) >> 10
         : D == NE_SHORT  ? (b & ~(FileGBB | FileHBB)) << 10
         : D == NW_SHORT  ? (b & ~(FileABB | FileBBB)) << 6
         : 0;
}

//max relevant occuapncy per piece 2^10 and 2^12
const int BISHOP_BLOCKER = 1024;
const int ROOK_BLOCKER = 4096;

//define attack tables
struct AttackTables 
{
    static std::array<std::array<Bitboard, NUM_SQUARES>, NUM_COLOR> pawn_attacks;
    static std::array<Bitboard, NUM_SQUARES> knight_attacks;
    static std::array<Bitboard, NUM_SQUARES> king_attacks;
    //store bitboards of possible attack for each square, depedning on relevant occupancy
    static std::array<std::array<Bitboard, ROOK_BLOCKER>, NUM_SQUARES> rook_attacks;
    static std::array<std::array<Bitboard, BISHOP_BLOCKER>, NUM_SQUARES> bishop_attacks;

    //leaper pieces generation
    static Bitboard gen_pawn_attacks(Color color, Square square);
    static Bitboard gen_knight_attacks(Square square);
    static Bitboard gen_king_attacks(Square square);
    void init_leaper_pieces();

    //slider pieces generation
    static Bitboard mask_rook_attacks(Square square);
    static Bitboard mask_bishop_attacks(Square square);
    static Bitboard gen_occupancy(int index, Bitboard attack_mask);

    static Bitboard gen_rook_attacks_onfly(Square square, Bitboard occupancy);
    static Bitboard gen_bishop_attacks_onfly(Square square, Bitboard occupancy);

    //get functions
    static Bitboard get_rook_attacks(Square square, Bitboard occupancy);
    static Bitboard get_bishop_attacks(Square square, Bitboard occupancy);
    static Bitboard get_queen_attacks(Square square, Bitboard occupancy);

    void init_slider_pieces();
    AttackTables();
};
