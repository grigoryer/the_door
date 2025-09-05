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

    Position pos("8/8/8/8/3k4/2b1n3/8/3K4 w - - 0 1");
    Search eng(pos);


    print_piece_board(pos);
    print_state_info(pos);

    /*while(true)
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

*/
    std::cout << sizeof(pos) << "\n";
    std::cout << sizeof(eng) << "\n";
    std::cout << sizeof(*eng.tt.table) << "\n";
    return 0;
}