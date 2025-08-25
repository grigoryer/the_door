#pragma once
#include "position.hpp"
#include "types.hpp"
#include "eval.hpp"


class Search {

public:
    Position& pos;
    Move best_move_found;
    
    Search(Position& pos) : pos(pos) {}

    Move best_move();
    int search(int depth, int alpha, int beta, Position* pos);

};