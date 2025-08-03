#include "position.hpp"
#include "types.hpp"


bool is_allinged(Square s1, Square s2)
{
    if(s1 == s2) { return false; }

    U8 s1_rank = rank_of(s1);
    U8 s2_rank = rank_of(s2);

    U8 s1_file = file_of(s1);
    U8 s2_file = file_of(s2);

    if(s1_rank == s2_rank || s1_file == s2_file) { return true; }

    if(abs(s1_rank - s2_rank) == abs(s1_file - s2_file)) { return true; }

    return false;
}


Bitboard Position::between_bb(Square s1, Square s2) const
{
    return between_array[s1][s2];
}


Bitboard Position::generate_between(Square s1, Square s2)
{
    if(!is_allinged(s1, s2)) { return 0ULL; }


    U8 s1_rank = rank_of(s1);
    U8 s2_rank = rank_of(s2);

    U8 s1_file = file_of(s1);
    U8 s2_file = file_of(s2);

    
    int rank_dir = (s2_rank > s1_rank) ? 1 : (s2_rank < s1_rank) ? -1 : 0;
    int file_dir = (s2_file > s1_file) ? 1 : (s2_file < s1_file) ? -1 : 0;

    Bitboard line = 0ULL;
    int current_rank = s1_rank + rank_dir;
    int current_file = s1_file + file_dir;

    while (current_rank != s2_rank || current_file != s2_file) {
        int square = (current_rank * 8) + current_file;
        line |= (1ULL << square);
        
        current_rank += rank_dir;
        current_file += file_dir;
    }
    
    return line;
}


Bitboard Position::line_bb(Square s1, Square s2) const
{
    Bitboard line = between_bb(s1,s2);
    set_bit(line,  s1);
    set_bit(line,  s2);
    return line;
}

Square StateInfo::ep_num_to_square() const 
{   
    assert(ep_num >= 0 && ep_num < ep_none);
    if (ep_num <= ep_white_h) { return a3 + ep_num; }
    return (a6 + (ep_num - ep_black_a));
}

Square StateInfo::square_to_ep_num(Square sq) 
{   
    assert(sq >= 0 && sq < NUM_SQUARES);
    Square rank = sq / NUM_RANKS;
    assert(rank == RANK_3 || rank == RANK_6);
    Square file = sq % NUM_FILES;
    return file + (rank == RANK_3 ? 0 : NUM_FILES);
}



StateInfo::StateInfo() 
{}

StateInfo::StateInfo(const StateInfo& old) : 
    castling_rights(old.castling_rights),
    ep_num(old.ep_num),
    captured_piece(old.captured_piece),
    half_move(old.half_move),
    full_move(old.full_move),
    hash(old.hash)
{  }