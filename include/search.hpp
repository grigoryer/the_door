#pragma once
#include "types.hpp"
#include "position.hpp"
#include "eval.hpp"


class Search
{
public:

    Position &pos;
    Evalution eval;

    int nega_max(int depth, int alpha, int beta);
    Move search_root();

    Search();
    Search(Position &position);
};