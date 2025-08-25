#include "move_gen.hpp"
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

void print_state_info(const Position* pos) 
{
    std::cout << "=== State Information ===" << std::endl;
    std::cout << ((pos->side_to_move == WHITE) ? "WHITE\n" : "BLACK\n");
    std::cout << "Castling Rights: " << static_cast<int>(pos->state->castling_rights) << std::endl;
    std::cout << "En Passant Square: " << static_cast<int>(pos->state->ep_num) << std::endl;
    std::cout << "Half Move Clock: " << pos->state->half_move << std::endl;
    std::cout << "Full Move Number: " << pos->state->full_move << std::endl;
    std::cout << "Hash Key: " << (pos->state->hash) << std::endl;
    std::cout << "=========================" << std::endl;
}



int Position::perft(int depth)
{
    if(depth == 0) { return 1; }

    MoveList move_list;
    
    generate<LEGAL>(*this , move_list);

    if(is_mate(&move_list) || is_draw())
    {
        return 0;
    }

    int nodes = 0;

    for(int i = 0; i < move_list.count; i++)
    {
        Move move = move_list.move_list[i];

        make_move(move);
        nodes += perft(depth - 1);
        unmake_move();

    }

    return nodes;
}

void Position::perft_divide(int depth)
{
    if(depth <= 0) return;
    
    MoveList move_list;
    generate<LEGAL>(*this, move_list);
    
    int total_nodes = 0;
    
    for(int i = 0; i < move_list.get_count(); i++)
    {
        Move move = move_list.get_move(i);
        
        make_move(move);
        int nodes = perft(depth - 1);
        unmake_move();
        
        move.print_move();
        std::cout << "" << nodes << "\n";
        total_nodes += nodes;
    }
    
    std::cout << "Total nodes: " << total_nodes << std::endl;
}

int Position::perft_debug(int depth)
{
    if(depth == 0) { return 1; }

    MoveList move_list;
    generate<LEGAL>(*this , move_list);

    int nodes = 0;

    print_piece_board(*this);
    std::cin.get();


    for(int i = 0; i < move_list.count; i++)
    {
        Move move = move_list.move_list[i];

        make_move(move);
        print_piece_board(*this);
        std::cin.get();
        nodes += perft(depth - 1);
        unmake_move();
        print_piece_board(*this);
        std::cin.get();

    }

    return nodes;
}