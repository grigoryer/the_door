#pragma once
#include "position.hpp"
#include "types.hpp"
#include <array>


const std::array<int, NUM_PIECES> piece_values = {0, 900, 500, 350, 325, 100};

const std::array<int, NUM_SQUARES> rook_val_map = {
  0,  0,  0,  0,  0,  0,  0,  0,
  15,  15, 15, 15, 15, 15,  15,  15,
  0,  0, 15, 15, 15, 15,  0,  0,
  0,  0, 15, 15, 15, 15,  0,  0,
  0,  0, 15, 15, 15, 15,  0,  0,
  0,  0, 15, 15, 15, 15,  0,  0,
  15,  15, 30, 30, 30, 30,  15,  15,
  0,  0, 15, 15, 15, 15,  0,  0
};

const std::array<int, NUM_SQUARES> bishop_val_map = {
 -15,  0,  0, 15, 15,  0,  0, -15,
   0, 15, 15, 30, 30, 15, 15,  0,
   0, 15, 30, 30, 30, 30, 15,  0,
  15, 30, 30, 45, 45, 30, 30, 15,
  15, 30, 30, 45, 45, 30, 30, 15,
   0, 15, 30, 30, 30, 30, 15,  0,
   0, 15, 15, 30, 30, 15, 15,  0,
 -15,  0,  0, 15, 15,  0,  0, -15
};

const std::array<int, NUM_SQUARES> knight_val_map = {
 -30, -15,  -5,  -5,  -5,  -5, -15, -30,
 -15,   0, 15, 15, 15, 15,   0, -15,
   -10,  15, 30, 30, 30, 30,  15,   -10,
   -10,  15, 30, 45, 45, 30,  15,   -10,
   -10,  15, 30, 45, 45, 30,  15,   -10,
-10,  15, 30, 30, 30, 30,  15,   -10,
 -15,   0, 15, 15, 15, 15,   0, -15,
 -30, -15,  -5,  -5, -5, -5, -5, -30
};

const std::array<int, NUM_SQUARES> pawn_val_map = {
  0,  0,  0,  0,  0,  0,  0,  0,   // rank 1
  5,  5,  5,  5,  5,  5,  5,  5,   // rank 2
 10, 10, 10, 10, 10, 10, 10, 10,   // rank 3
 15, 15, 15, 40, 40, 15, 15, 15,   // rank 4
 20, 20, 20, 40, 40, 20, 20, 20,   // rank 5
 25, 25, 25, 25, 25, 25, 25, 25,   // rank 6
 30, 30, 30, 30, 30, 30, 30, 30,   // rank 7
 35, 35, 35, 35, 35, 35, 35, 35    // rank 8
};

const std::array<int, NUM_SQUARES> queen_val_map = {
   0,  0, 15, 15, 15, 15,  0,  0,
   0, 15, 30, 30, 30, 30, 15,  0,
  15, 30, 45, 45, 45, 45, 30, 15,
  15, 30, 45, 60, 60, 45, 30, 15,
  15, 30, 45, 60, 60, 45, 30, 15,
  15, 30, 45, 45, 45, 45, 30, 15,
   0, 15, 30, 30, 30, 30, 15,  0,
   0,  0, 15, 15, 15, 15,  0,  0
};

const std::array<int, NUM_SQUARES> king_val_map = {
  35, 30, 15,  0,  0, 15, 30, 35,
  30, 15,  0,-15,-15,  0, 15, 30,
  15,  0,-15,-30,-30,-15,  0, 15,
   0,-15,-30,-45,-45,-30,-15,  0,
   0,-15,-30,-45,-45,-30,-15,  0,
  15,  0,-15,-30,-30,-15,  0, 15,
  30, 15,  0,-15,-15,  0, 15, 30,
  35, 30, 15,  0,  0, 15, 30, 35
};


int evaluate(Position& pos);

int eval_pos(Color color, Position& pos);


int count_material(Color color, Position& pos);



template <Color color>
int eval_pos(Position& pos)
{
    int pos_score = 0;

    for(Piece piece = KING; piece <= PAWN; piece++)
    {
        Bitboard bb = pos.get_piece(color, piece);
        Square sq = pop_lsb(bb);

        if constexpr (BLACK)
        {
            int file = sq % 8;
            int rank = sq / 8;
            return (7 - rank) * 8 + file;
        }

        switch (piece) 
        {
            case(KING) : pos_score += king_val_map[sq]; break;
            case(QUEEN) : pos_score += queen_val_map[sq]; break;
            case(ROOK) : pos_score += rook_val_map[sq]; break;
            case(BISHOP) : pos_score += bishop_val_map[sq]; break;
            case(KNIGHT) : pos_score += knight_val_map[sq]; break;
            case(PAWN) : pos_score += pawn_val_map[sq]; break;
        }
    }

    return pos_score;
}
