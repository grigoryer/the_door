#include "eval.hpp"
#include "types.hpp"

int evaluate(Position& pos)
{
    int white_eval = count_material(WHITE, pos);
    int black_eval = count_material(BLACK, pos);

    white_eval += eval_pos<WHITE>(pos);
    black_eval += eval_pos<BLACK>(pos);

    int evaluation = white_eval - black_eval;

    int perspective = (pos.side_to_move == WHITE) ? 1 : -1;
    return evaluation * perspective;
}

int count_material(Color color, Position& pos)
{
    int material = 0; 
    material += bit_count(pos.get_piece(color,  KNIGHT)) * piece_values[KNIGHT];

    material += bit_count(pos.get_piece(color,  QUEEN)) * piece_values[QUEEN];

    material += bit_count(pos.get_piece(color, ROOK)) * piece_values[ROOK];

    material += bit_count(pos.get_piece(color,  BISHOP)) * piece_values[BISHOP];

    material += bit_count(pos.get_piece(color, PAWN)) * piece_values[PAWN];
    
    return material;
}