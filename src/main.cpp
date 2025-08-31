#include "fen_strings.hpp"
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


    //test_correct();
    std::cout << pos.perft(5);
    // MoveList ml;
    //generate<LEGAL>(pos, ml);
    //print_piece_board(pos);
    //print_board(pos.state->blockers_for_king[WHITE]);

   //std::cout << sizeof(StateInfo);
    return 0;
}