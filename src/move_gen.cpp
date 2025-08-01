#include "move_gen.hpp"
#include "move.hpp"






template<GenType Type>
void generate_all(const Position& pos, MoveList& move_list)
{
    








}



template<> void MoveGenerator<ALL>::generate(const Position& pos, MoveList& move_list) { generate_all<ALL>(pos,move_list); }
template<> void MoveGenerator<CAPTURES>::generate(const Position& pos, MoveList& move_list) { generate_all<CAPTURES>(pos,move_list); }
template<> void MoveGenerator<NON_EVASIONS>::generate(const Position& pos, MoveList& move_list) { generate_all<NON_EVASIONS>(pos,move_list); }
template<> void MoveGenerator<EVASIONS>::generate(const Position& pos, MoveList& move_list) { generate_all<EVASIONS>(pos,move_list); }
template<> void MoveGenerator<QUIETS>::generate(const Position& pos, MoveList& move_list) { generate_all<QUIETS>(pos,move_list); }
