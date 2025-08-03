#include "attack.hpp"
#include "fen_strings.hpp"
#include "move.hpp"
#include "move_gen.hpp"
#include "types.hpp"
#include "position.hpp"
#include <iostream>



int main()
{
    Position pos;
    MoveList move_list;
    AttackTables attack;


    while(std::cin.get())
    {
        std::cout << "SIDE TO MOVE: " << (pos.side_to_move == WHITE ? "WHITE" : "BLACK");
        print_piece_board(pos);
        move_list.clear();
    
        if(pos.is_check())
        {
            print_board(pos.state.checkers_bb);
            print_board(pos.color_bb[pos.side_to_move ^ 1]);
            generate<EVASIONS>(pos,move_list);
        }
        else 
        {
            generate<NON_EVASIONS>(pos,move_list);
        }

        std::cout << "MOVE COUNT: " << move_list.count << std::endl;
        move_list.print_all();

        print_piece_board(pos);

        pos.make_move(move_list.move_list[0]);

    }


    return 0;
}