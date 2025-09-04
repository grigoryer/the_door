#include "fen_strings.hpp"
#include "move.hpp"
#include "move_gen.hpp"
#include "search.hpp"
#include "position.hpp"
#include "eval.hpp"
#include <cstdlib>
#include <iostream>
#include <random>


int main()
{

    Position pos(perft_4);
    Search eng(pos);


    print_piece_board(pos);
    print_state_info(pos);

    while(std::cin.get())
    {
        Move move = eng.search_root();

        if(move == null_move)
        {
            if(pos.draw_flag == true)
            {
                std::cout << "DRAW";
                break;
            }
            else if(pos.mate_flag == true)
            {
                std::cout << "MATE";
                break;
            }
            else {
                std::cout << "NULL MOVE";
                break;
            }
        }

        pos.make_move(move);
        print_move(move);
        
        print_piece_board(pos);
    }

    return 0;
}