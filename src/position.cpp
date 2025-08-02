#include "position.hpp"
#include <array>
#include <cassert>
#include <sstream>
#include "attack.hpp"
#include "fen_strings.hpp"
#include "types.hpp"
#include <iostream>



void Position::fen_parser(const std::string& fen)
{
    std::fill(piece_board.begin(), piece_board.end(), NONE);

    static constexpr int fen_position = 0;
    static constexpr int fen_color = 1;
    static constexpr int fen_castling = 2;
    static constexpr int fen_en_passant = 3;
    static constexpr int fen_half_move = 4;
    static constexpr int fen_full_move = 5;


    std::array<std::string, 6> parts;
    std::istringstream iss(fen);
    std::string word;

    int i = 0;

    while (iss >> word ) {
        parts[i++] = word;
    }

    //split up string into parts
    int rank = 0; 
    int file = 0;

    std::string position = parts[fen_position];
    std::string color = parts[fen_color];
    std::string castling = parts[fen_castling];
    std::string en_passant = parts[fen_en_passant];
    std::string half_move = parts[fen_half_move];
    std::string move = parts[fen_full_move];

    // handle the first position part of the fen string and add bits where specified.
    for(int i = 0; i < position.length(); i++)
    {
        char ch = position[i];

        if(ch == '/')
        {
            rank++;
            file = 0;
        }
        else if (std::isdigit(ch) != 0) 
        {
            file += (ch - '0');
        }
        else
        {
            Color isWhite = isupper(ch) ? WHITE : BLACK;
            int sq = ((RANK_8 - rank) * NUM_RANKS) + file;

            isWhite == WHITE ?  set_bit(color_bb[WHITE],sq) : set_bit(color_bb[BLACK],sq);

            ch = (char)tolower(ch);

            switch(ch)
            {
                case 'k': set_bit(piece_bb[KING], sq); break;
                case 'q': set_bit(piece_bb[QUEEN], sq); break;
                case 'r': set_bit(piece_bb[ROOK], sq); break;
                case 'b': set_bit(piece_bb[BISHOP], sq); break;
                case 'n': set_bit(piece_bb[KNIGHT], sq); break;
                case 'p': set_bit(piece_bb[PAWN], sq); break;
            }

            file++;   
        }
    }

    side_to_move = (color == "w" ) ?  WHITE : BLACK;

    state.castling_rights = NO_CASTLING;
    
    for(int i = 0; i < castling.length(); i++)
    {
        char ch = castling[i];
        switch(ch) 
        {
            case 'K': state.castling_rights |= WK; break;
            case 'Q': state.castling_rights |= WQ; break;
            case 'k': state.castling_rights |= BK; break;
            case 'q': state.castling_rights |= BQ; break;
            case '-': break;
        }
    }

    //parse en passant if any.
    if(en_passant != "-")
    {
        int file = en_passant[0] - 'a';
        int rank = en_passant[1] - '0';

        if(rank == RANK_5) 
        {
            state.ep_num = file; // 0-7
        }
        else if(rank == RANK_2) 
        {
            state.ep_num = NUM_FILES + file; 
        }
    }
    else
    {
        state.ep_num = ep_none; //none value
    }

    if(half_move == "-" || parts[fen_half_move] == ""){ state.half_move = 0; }
    else { state.half_move = std::stoi(half_move); }

    if(move == "-" || parts[fen_full_move] == "" ){ state.full_move = 0; }
    else{ state.full_move = std::stoi(move); }
}


Position::Position()
{
    fen_parser(STARTING_FEN);
    init();
}


Position::Position(const std::string& fen)
{
    fen_parser(fen);
    init();
}


void Position::init()
{
    init_between();
    AttackTables attacks;
    occupancy = color_bb[WHITE] | color_bb[BLACK];

    set_check_info(side_to_move);
}

void Position::init_between()
{
    for(int s1 = 0; s1 < NUM_SQUARES; s1++)
    {
        for(int s2 = 0; s2 < NUM_SQUARES; s2++)
        {
            between_array[s1][s2] = generate_between(s1,s2);
        }
    }
}

Square Position::get_king_square(Color color) const
{
    assert(get_piece(color, KING) != 0);
    return lsb(get_piece(color, KING));
}


Bitboard Position::get_piece(Color color, PieceType piece) const
{
    return piece_bb[piece] & color_bb[color];
}

void Position::set_check_squares(Color color)
{   
    Square ksq = get_king_square(color);

    state.check_squares[color][PAWN]   = AttackTables::pawn_attacks[color][ksq];
    state.check_squares[color][KNIGHT] = attacks_bb<KNIGHT>(ksq, occupancy);
    state.check_squares[color][BISHOP] = attacks_bb<BISHOP>(ksq, occupancy);
    state.check_squares[color][ROOK]   = attacks_bb<ROOK>(ksq, occupancy);
    state.check_squares[color][QUEEN]  = attacks_bb<QUEEN>(ksq, occupancy);
}


void Position::set_pins_info(Color color)
{
    Color enemy = color ^ 1;
    state.pinners[enemy] = 0ULL;
    state.blockers_for_king[color] = 0ULL;

    Square ksq = get_king_square(color);
    Bitboard snipers = get_piece(enemy, QUEEN) | get_piece(enemy, ROOK) | get_piece(enemy, BISHOP);

    while(snipers)
    {
        Square sniper = lsb(snipers);
        Bitboard bb = between_bb(sniper, ksq) & color_bb[color];
        
        if(bit_count(bb) == 1)
        {
            state.blockers_for_king[color] |= bb;
            state.pinners[enemy] |= (1ULL << sniper);
        }
        
        pop_lsb(snipers);
    }
}

void Position::set_check_info(Color color)
{
    set_check_squares(color);
    set_pins_info(color);
    set_checkers();
}


bool Position::is_check() const
{
    return bit_count(state.checkers_bb) != 0;
}


bool Position::is_double_check() const
{
    return bit_count(state.checkers_bb) > 1;
}

Bitboard Position::set_checkers()
{  
    state.checkers_bb = 0ULL;
    Color us = side_to_move;
    Color enemy = side_to_move ^ 1;

    for(Piece piece = KING; piece <= PAWN; piece++ )
    {
        Bitboard attacker = get_piece(enemy, (PieceType)piece);
        if((state.check_squares[us][piece] & attacker) != 0)
        {
            state.checkers_bb |= state.check_squares[us][piece] & attacker;
        }
    }
    return state.checkers_bb;
}

U8 Position::can_castle(Color color) const
{
    U8 output = NO_CASTLING;
    if(color == WHITE)
    {
        if((WK & state.castling_rights) == WK && !get_bit(occupancy, f1) && !get_bit(occupancy, g1))
        { output |= WK; }
        if ((WQ & state.castling_rights) == WQ && !get_bit(occupancy, b1) && !get_bit(occupancy, c1) && !get_bit(occupancy, d1))
        { output |= WQ; }
        return output;
    }
    if(color == BLACK)
    {
        if((BK & state.castling_rights) == BK && !get_bit(occupancy, f8) && !get_bit(occupancy, g8))
        { output |= BK; }
        if ((BQ & state.castling_rights) == BQ && !get_bit(occupancy, b8) && !get_bit(occupancy, c8) && !get_bit(occupancy, d8))
        { output |= BQ; }
        return output;
    }
}