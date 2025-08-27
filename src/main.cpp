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
    Position pos(perft_6);

    print_piece_board(pos);

    pos.perft_divide(5);
    //pos.perft(5);
    return 0;
}