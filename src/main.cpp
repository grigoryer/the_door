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

    //test_correct();

    Position pos;
    std::cout << pos.perft(6);
    return 0;
}