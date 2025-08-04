#include "attack.hpp"
#include "fen_strings.hpp"
#include "move.hpp"
#include "move_gen.hpp"
#include "types.hpp"
#include "position.hpp"
#include <iostream>



int main()
{
    Position pos(STARTING_FEN);
    MoveList move_list;
    AttackTables attack;
    print_piece_board(pos);

    while(true)
    {
        std::string input;
        std::cin >> input;
        if(input == "n")
            {
            std::cout << "===============MAKING MOVE============================\n";
            std::cout << "SIDE TO MOVE: " << (pos.side_to_move == WHITE ? "WHITE\n" : "BLACK\n");
            print_state_info(pos.state);
            print_piece_board(pos);
            move_list.clear();
        
            if(pos.is_check())
            {
                print_board(pos.state->checkers_bb);
                print_board(pos.color_bb[pos.side_to_move ^ 1]);
                generate<EVASIONS>(pos,move_list);
            }
            else 
            {
                generate<CAPTURES>(pos,move_list);
                generate<QUIETS>(pos,move_list);
            }

            int m = 0;
            while(!pos.is_legal(move_list.move_list[m]))
            {
                move_list.print_all();
                std::cout << "MOVE ILLEGAL: NEW COUNTER: " << ++m << "\n";
            }


            std::cout << "MOVE SELECTED: "; move_list.move_list[m].print_move();
            pos.make_move(move_list.move_list[m]);
            std::cout << "\n\n\n";
            std::cout << "SIDE TO MOVE: " << (pos.side_to_move == WHITE ? "WHITE\n" : "BLACK\n");
            print_state_info(pos.state);
            print_piece_board(pos);
            std::cout << "===========================================\n";
        }
        else if(input == "k")
        {
            std::cout << "=================UNMAKING MOVE==========================\n";
            pos.unmake_move();
            std::cout << "SIDE TO MOVE: " << (pos.side_to_move == WHITE ? "WHITE\n" : "BLACK\n");
            print_state_info(pos.state);
            print_piece_board(pos);
            std::cout << "===========================================\n";
        }
    }
    return 0;
}