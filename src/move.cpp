#include "move.hpp"
#include "types.hpp"
#include <cassert>
#include <string>
#include <iostream>


int MoveList::get_count()
{
    return count;
}

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
              << files[to % 8]   << ranks[to / 8] << ": ";
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