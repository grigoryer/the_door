#include "attack.hpp"
#include "fen_strings.hpp"
#include "move.hpp"
#include "move_gen.hpp"
#include "types.hpp"
#include "position.hpp"
#include <cstdlib>
#include <iostream>
#include <random>



int main()
{
    Position pos(perft_3);
    MoveList move_list;
    AttackTables attack;
    print_piece_board(pos);
    srand(33);

    while(std::cin.get())
    {
        generate<LEGAL>(pos, move_list);
        move_list.print_all();
        int move = rand() % move_list.count;
        move_list.move_list[move].print_move();
        pos.make_move(move_list.move_list[move]);
        print_piece_board(pos);
        print_state_info(&pos);
    }
    return 0;
}