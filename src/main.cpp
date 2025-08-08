#include "attack.hpp"
#include "fen_strings.hpp"
#include "move.hpp"
#include "move_gen.hpp"
#include "position.hpp"
#include "types.hpp"
#include <cstdlib>
#include <iostream>



int main()
{
    Position pos;
    MoveList move_list;
    
    print_board(pos.state->blockers_for_king);
    print_board(pos.state->pinners);
    print_board(pos.state->checkers_bb);


    pos.perft_divide(2);
    print_piece_board(pos);
    
    return 0;
}