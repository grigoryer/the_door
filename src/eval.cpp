#include "eval.hpp"
#include "types.hpp"

Evalution::Evalution()
{
    // Default constructor - can be empty if no initialization needed
}
 
int Evalution::material_count(Position& pos, Piece piece)
{   
    return (bit_count(pos.get_piece(WHITE, piece)) - bit_count(pos.get_piece(BLACK, piece)));
}

int Evalution::material_score(Position& pos)
{
    int score = 0;
    
    score += material_count(pos, QUEEN)  * piece_values[QUEEN];
    score += material_count(pos, ROOK)   * piece_values[ROOK];
    score += material_count(pos, BISHOP) * piece_values[BISHOP];
    score += material_count(pos, KNIGHT) * piece_values[KNIGHT];
    score += material_count(pos, PAWN)   * piece_values[PAWN];
    
    return score;
}

int Evalution::evaluate(Position& pos)
{
    int score = 0;
    score = material_score(pos);
    
    return score * moving_side[pos.side_to_move];
}