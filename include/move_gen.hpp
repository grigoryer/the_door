#pragma once
#include "types.hpp"
#include "move.hpp"
#include "position.hpp"


enum GenType 
{
    EVASIONS,
    CAPTURES,
    QUIETS,
    NON_EVASIONS,
    LEGAL
};



template <GenType Type>
void generate(Position& pos, MoveList& move_list);



extern template void generate<QUIETS>(Position& pos, MoveList& move_list);
extern template void generate<CAPTURES>(Position& pos, MoveList& move_list);
extern template void generate<EVASIONS>(Position& pos, MoveList& move_list);

