#include "move.hpp"
#include "move_gen.hpp"
#include "search.hpp"
#include "position.hpp"
#include "types.hpp"
#include <cstdlib>
#include <iostream>
#include <random>


int main()
{
    Position pos;
    Search engine(pos);

    std::string line;
    print_piece_board(pos);
    print_state_info(&pos);

    while (std::getline(std::cin, line))
    {

        if(line == "n")
        {
            pos.unmake_move();
        }
        else {

            pos.make_move(engine.best_move());
            if(pos.is_draw())
            {
                std::cout << "DRAW";
                break;
            }
            
            std::cout << "\n\nSelected Move: "; engine.best_move_found.print_move();
        }

            print_piece_board(pos);
            print_state_info(&pos);       
    }


            print_piece_board(pos);
            print_state_info(&pos);       

    return 0;
}