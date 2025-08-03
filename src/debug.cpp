#include "types.hpp"
#include "position.hpp"
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



void print_piece_board(Position& pos) 
{

    constexpr std::array<char, NUM_PIECES * 2 + 1> piece_to_char = {
        'K', 'Q', 'R', 'B', 'N', 'P',
        'k', 'q', 'r', 'b', 'n', 'p', '.'
    };
    
    std::cout << "\n";

    for (int rank = RANK_8; rank >= 0; rank--) 
    {  
        std::cout << rank + 1 << "  ";
        for (int file = 0; file <= FILE_H; file++) 
        {
            int square = (rank * NUM_RANKS) + file;
            Piece piece = pos.piece_board[square];
            std::cout << piece_to_char[piece] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n   a b c d e f g h\n";
}

void print_state_info(const StateInfo& state) 
{
    std::cout << "=== State Information ===" << std::endl;
    std::cout << "Castling Rights: " << static_cast<int>(state.castling_rights) << std::endl;
    std::cout << "En Passant Square: " << static_cast<int>(state.ep_num) << std::endl;
    std::cout << "Half Move Clock: " << state.half_move << std::endl;
    std::cout << "Full Move Number: " << state.full_move << std::endl;
    std::cout << "Captured Piece: " << static_cast<int>(state.captured_piece) << std::endl;
    std::cout << "Hash Key: " << state.hash << std::endl;
    std::cout << "=========================" << std::endl;
}
