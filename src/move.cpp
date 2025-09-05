#include "move.hpp"
#include "eval.hpp"
#include "position.hpp"
#include <cassert>
#include <iostream>


void MoveList::add(const U32 move) {
    move_list[count++] = move;
}

void MoveList::clear()
{
    count = 0;
}

Move MoveList::get_move(int index)
{
    return move_list[index];
}

MoveList::MoveList() 
{  } 


void print_move(Move move)
{
    Square from = move & FROM_MASK;
    Square to   = (move & TO_MASK) >> 6;

    std::cout << files[from % 8] << ranks[from / 8]
              << files[to % 8]   << ranks[to / 8];
}

void MoveList::print_all()
{
    
    std::cout << "\nCount: " << count << "\n";

    for(Move move : move_list)
    {
        print_move(move);
        std::cout << "\n";
    }
}


void MoveList::score_moves(const Position& pos)
{
    for(int i = 0; i < count; i++)
    {   
        int move_score = 0;
        Move move = move_list[i];

        Square to = move_get_to(move);
        Square from = move_get_from(move);
        Piece piece = move_get_piece(move);

        if(move_is_promoted(move))
        {
            Piece promoted_piece = move_get_promoted(move);
            move_score += piece_values[promoted_piece];
        }

        if(move_is_capture(move))
        {
            Piece capture_piece = pos.piece_board[static_cast<int>(from)];
            move_score += (capture_piece - piece);
        }

        //scores[i] = move_score;
    }
}
