#pragma once
#include "types.hpp"
#include "position.hpp"
#include <array>


const std::array<int, NUM_PIECES> piece_values = {0, 900, 500, 350, 325, 100};
const std::array<int, NUM_COLOR> moving_side = {1, -1};


class Evalution
{
public:

    static int material_count(Position& pos, Piece piece);
    static int material_score(Position& pos);
    
    static int evaluate(Position& pos);
    Evalution();
};