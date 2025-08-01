#include "types.hpp"
#include <iostream>


void print_board(Bitboard bitboard)
{
    std::cout << "\n";
    for (int rank = 7; rank >= 0; --rank)
    {
        std::cout << rank + 1 << " "; 
        for (int file = 0; file < NUM_FILES; ++file)
        {
            Square square = rank * 8 + file;
            char c = get_bit(bitboard, square) ? '1' : '.';
            std::cout << c << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "  a b c d e f g h\n\n";
    std::cout << "#: " << bitboard << '\n';
}

