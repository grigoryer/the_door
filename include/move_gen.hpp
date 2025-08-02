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
void generate(const Position& pos, MoveList& move_list);




extern template void generate<QUIETS>(const Position& pos, MoveList& move_list);
extern template void generate<CAPTURES>(const Position& pos, MoveList& move_list);
extern template void generate<EVASIONS>(const Position& pos, MoveList& move_list);
extern template void generate<LEGAL>(const Position& pos, MoveList& move_list);
extern template void generate<NON_EVASIONS>(const Position& pos, MoveList& move_list);

