#pragma once
#include <cstdint>

using Bitboard = uint64_t;
using Key = uint64_t;
using Square = int8_t;
using Color = int8_t;
using Piece = int8_t;
using U32 = uint32_t;
using U8 = uint8_t;
using Move = uint32_t;

const int negative_infinity = -9999999;
const int positive_infinity = 9999999;
const int SEARCH_DEPTH = 7;

const int NUM_SQUARES = 64;
const int NUM_COLOR = 2;
const int NUM_RANKS = 8;
const int NUM_FILES = 8;
const int NUM_PIECES = 6;
const int NUM_CASTLING = 16;

const int NUM_SLIDERS = 2;
const int NUM_EPSQUARES = 17;

const int MAX_MOVES = 256;
const int MAX_HISTORY = 512;

const Bitboard FullBB = 0xFFFFFFFFFFFFFFFFULL;

const Bitboard FileABB = 0x0101010101010101ULL;
const Bitboard FileBBB = FileABB << 1;
const Bitboard FileCBB = FileABB << 2;
const Bitboard FileDBB = FileABB << 3;
const Bitboard FileEBB = FileABB << 4;
const Bitboard FileFBB = FileABB << 5;
const Bitboard FileGBB = FileABB << 6;
const Bitboard FileHBB = FileABB << 7;

const Bitboard Rank1BB = 0xFF;
const Bitboard Rank2BB = Rank1BB << (8 * 1);
const Bitboard Rank3BB = Rank1BB << (8 * 2);
const Bitboard Rank4BB = Rank1BB << (8 * 3);
const Bitboard Rank5BB = Rank1BB << (8 * 4);
const Bitboard Rank6BB = Rank1BB << (8 * 5);
const Bitboard Rank7BB = Rank1BB << (8 * 6);
const Bitboard Rank8BB = Rank1BB << (8 * 7);

enum PieceType : Piece
{
    KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, NONE
};

enum SLIDER : Piece
{
    queen_slider, rook_slider, bishop_slider,
};

enum PieceList : Piece
{
    K = 0, Q = 1, R = 2, B = 3, N = 4, P = 5,
    k = 6, q = 7, r = 8, b = 9, n = 10, p = 11, no_piece = 12
};

enum Colors : Color
{
    WHITE, BLACK
};

enum Squares : Square
{
    a1,b1,c1,d1,e1,f1,g1,h1,
    a2,b2,c2,d2,e2,f2,g2,h2,
    a3,b3,c3,d3,e3,f3,g3,h3,
    a4,b4,c4,d4,e4,f4,g4,h4,
    a5,b5,c5,d5,e5,f5,g5,h5,
    a6,b6,c6,d6,e6,f6,g6,h6,
    a7,b7,c7,d7,e7,f7,g7,h7,
    a8,b8,c8,d8,e8,f8,g8,h8, no_square
};

enum File : int8_t
{
    FILE_A,
    FILE_B,
    FILE_C,
    FILE_D,
    FILE_E,
    FILE_F,
    FILE_G,
    FILE_H,
};   

enum Rank : int8_t
{
    RANK_1,
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,
};


enum Direction : int8_t
{
    NORTH = 8,
    EAST  = 1,
    SOUTH = -NORTH,
    WEST  = -EAST,

    NORTH_EAST = NORTH + EAST,
    SOUTH_EAST = SOUTH + EAST,
    SOUTH_WEST = SOUTH + WEST,
    NORTH_WEST = NORTH + WEST
};

enum Castling : U8
{ 
    NO_CASTLING = 0, WK = 1, WQ = 2, BK = 4, BQ = 8, WHITE_CASTLING = 3, BLACK_CASTLING = 12, ALL_CASTLING = 15 
};


enum Ep_square : Square
{
    ep_white_a, ep_white_b, ep_white_c, ep_white_d,
    ep_white_e, ep_white_f, ep_white_g, ep_white_h,
    ep_black_a, ep_black_b, ep_black_c, ep_black_d,
    ep_black_e, ep_black_f, ep_black_g, ep_black_h,
    ep_none
};

enum MoveType
{
    NORMAL,
    PROMOTION,
    ENPASSANT,
    CASTLE,
    DOUBLE
};


void print_board(Bitboard bitboard);


inline bool get_bit(const Bitboard &b, Square sq) 
{
    return (b & (1ULL << sq)) != 0;
}

inline void set_bit(Bitboard &b, Square sq) 
{
    b |= (1ULL << sq);
}

inline Square lsb(Bitboard b) 
{
    return __builtin_ctzll(b);
}

inline void pop_bit(Bitboard &b, Square sq)
{
    b &= ~(1ULL << sq);
}

inline Square pop_lsb(Bitboard &b) 
{
    Square sq = lsb(b);
    b &= ~(1ULL << sq);
    return sq;
}

inline int bit_count(Bitboard b) 
{
    return __builtin_popcountll(b);  
} 

inline bool more_than_one(Bitboard b) 
{
    return __builtin_popcountll(b) > 1;
} 

inline constexpr Bitboard square_bb(Square s)
{
    return (1ULL << s);
}


inline U8 rank_of(U8 square)
{
    return (square / 8);
}

inline U8 file_of(U8 square)
{
    return (square % 8);
}