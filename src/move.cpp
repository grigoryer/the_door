#include "move.hpp"
#include "types.hpp"
#include <cassert>
#include <string>
#include <iostream>

static const U8 to_shift        = 6;
static const U8 piece_shift     = 12;
static const U8 promoted_shift  = 16;
static const U8 capture_shift   = 20;
static const U8 double_shift    = 21;
static const U8 enpassant_shift = 22;
static const U8 castling_shift  = 23;

Move::Move() : move(0)
{  }

Move::Move(Square from, Square to, Piece piece, Piece promoted, 
           bool capture, bool double_push, bool enpassant, bool castling)
    : move 
    (
           static_cast<U32>(from)
        | (static_cast<U32>(to)          << to_shift)
        | (static_cast<U32>(piece)       << piece_shift)
        | (static_cast<U32>(promoted)    << promoted_shift)
        | (static_cast<U32>(capture)     << capture_shift)
        | (static_cast<U32>(double_push) << double_shift)
        | (static_cast<U32>(enpassant)   << enpassant_shift)
        | (static_cast<U32>(castling)    << castling_shift)
    )
{  }


Square Move::get_from() const 
{ 
    return move & FROM_MASK; 
}

Square Move::get_to() const 
{ 
    return (move & TO_MASK) >> to_shift; 
}

Piece Move::get_promoted() const 
{ 
    return (move & PROMOTED_MASK) >> promoted_shift; 
}

bool Move::is_capture() const 
{ 
    return (move & CAPTURE_MASK) != 0; 
}

bool Move::is_double() const 
{ 
    return (move & DOUBLE_MASK) != 0; 
}

bool Move::is_enpassant() const 
{ 
    return (move & ENPASSANT_MASK) != 0; 
}

bool Move::is_castling() const 
{ 
    return (move & CASTLING_MASK) != 0; 
}

Piece Move::get_piece() const 
{ 
    return (move & PIECE_MASK) >> piece_shift; 
}


bool Move::is_promoted() const
{
    return (get_promoted() != NONE);
}

// setters
void Move::set_from(Square from) 
{ 
    move = (move & ~FROM_MASK) | from; 
}

void Move::set_to(Square to) 
{ 
    move = (move & ~TO_MASK) | (to << to_shift); 
}

void Move::set_piece(Piece piece) 
{ 
    move = (move & ~PIECE_MASK) | (piece << piece_shift); 
}


void Move::set_promoted(Piece promoted) 
{ 
    move = (move & ~PROMOTED_MASK) | (promoted << promoted_shift); 
}
void Move::set_capture(bool capture) 
{ 
    move = (move & ~CAPTURE_MASK) | (static_cast<U32>(capture) << capture_shift); 
}

void Move::set_double(bool double_push) 
{ 
    move = (move & ~DOUBLE_MASK) | (static_cast<U32>(double_push) << double_shift); 
}

void Move::set_enpassant(bool enpassant) 
{ 
    move = (move & ~ENPASSANT_MASK) | (static_cast<U32>(enpassant) << enpassant_shift); 
}

void Move::set_castling(bool castling) 
{ 
    move = (move & ~CASTLING_MASK) | (static_cast<U32>(castling) << castling_shift); 
}

void Move::print_move()
{
    std::cout << n_to_sq(get_from()) << n_to_sq(get_to()) << ": 1\n";
}


std::string Move::n_to_sq(Square sq)
{
    // calculate file (column) and rank (row)
    U8 file = sq % NUM_FILES;  // 0-7 maps to a-h
    U8 rank = sq / NUM_RANKS;  // 0-7 maps to 1-8
    
    char file_letter = static_cast<char>('a' + file);
    char rank_number = static_cast<char>('1' + rank);
    
    return std::string(1, file_letter) + rank_number;
}

int MoveList::get_count()
{
    return count;
}

void MoveList::add(const Move move) {
    move_list[count++] = move;
}

void MoveList::clear()
{
    count = 0;
}

void MoveList::print_all()
{
    for(int i = 0; i < count; i++)
    {

        std::cout << (move_list[i].is_capture() == true || move_list[i].is_enpassant()  ? "IS CAPTURE: " : "NOT CAPTURE: ");
        move_list[i].print_move();
    }
}


Move MoveList::get_move(int index)
{
    return move_list[index];
}

MoveList::MoveList() 
{  } 