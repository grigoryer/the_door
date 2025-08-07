#include "attack.hpp"
#include "fen_strings.hpp"
#include "move.hpp"
#include "move_gen.hpp"
#include "position.hpp"
#include "types.hpp"
#include <cstdlib>
#include <iostream>



int main()
{
    Position pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2NQ21p/PPPBBPPP/R3K2R w KQkq - ");
    MoveList move_list;
    

    pos.perft_divide(3);
    print_piece_board(pos);
    return 0;
}