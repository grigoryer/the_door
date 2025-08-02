#include "attack.hpp"
#include "fen_strings.hpp"
#include "move.hpp"
#include "types.hpp"
#include "position.hpp"
#include <iostream>
#include "move_gen.hpp"



int main()
{
    Position pos(perft_5);
    MoveList move_list;

    print_board(pos.color_bb[WHITE]);


    move_list.clear();

    if(pos.is_check()){
        generate<EVASIONS>(pos, move_list);
    }
    else
    {
        generate<NON_EVASIONS>(pos, move_list);
    }

    std::cout <<  "AMOUNT OF MOVES: " << move_list.count << std::endl;
    move_list.print_all();

    return 0;
}