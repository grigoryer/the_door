#pragma once
#include "types.hpp"
#include "move.hpp"
#include "position.hpp"


enum GenType 
{
    ALL,
    CAPTURES,
    QUIETS,
    EVASIONS,
    NON_EVASIONS,
    LEGAL
};


template<GenType Type>
class MoveGenerator 
{
    static void generate(const Position& pos, MoveList& move_list);
};