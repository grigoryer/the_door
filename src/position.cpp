#include "position.hpp"
#include "fen_strings.hpp"
#include <iostream>
#include "move.hpp"
#include "types.hpp"
#include <array>
#include <cassert>
#include <sstream>



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

    state->castling_rights = NO_CASTLING;
    
    for(int i = 0; i < castling.length(); i++)
    {
        char ch = castling[i];
        switch(ch) 
        {
            case 'K': state->castling_rights |= WK; break;
            case 'Q': state->castling_rights |= WQ; break;
            case 'k': state->castling_rights |= BK; break;
            case 'q': state->castling_rights |= BQ; break;
            case '-': break;
        }
    }

    //parse en passant if any.
    if(en_passant != "-")
    {
        int file = en_passant[0] - 'a';
        int rank = en_passant[1] - '0';

        if(rank == RANK_4) 
        {
            state->ep_num = static_cast<U8>(file); // 0-7
        }
        else if(rank == RANK_7) 
        {
            state->ep_num = static_cast<U8>(NUM_FILES + file); 
        }
    }
    else
    {
        state->ep_num = ep_none; //none value
    }

    if(half_move == "-" || parts[fen_half_move] == ""){ state->half_move = 0; }
    else { state->half_move = std::stoi(half_move); }

    if(move == "-" || parts[fen_full_move] == "" ){ state->full_move = 0; }
    else{ state->full_move = std::stoi(move); }
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

    set_check_squares(side_to_move);
    set_check_squares(side_to_move ^ 1 );

    init_piece_board();
    state->hash  = init_hash();
}

void Position::init_between()
{
    for(int s1 = 0; s1 < NUM_SQUARES; s1++)
    {
        for(int s2 = 0; s2 < NUM_SQUARES; s2++)
        {
            between_array[s1][s2] = generate_between(s1, s2);
            through_array[s1][s2] = generate_through(s1, s2);
        }
    }
}

void Position::init_piece_board()
{
    piece_board.fill(no_piece);
    for (Piece piece = KING; piece <= PAWN; piece++)
    {
        Bitboard white_bb = get_piece(WHITE, piece);
        while (white_bb != 0)
        {
            Square square = lsb(white_bb);
            piece_board[square] = piece; // K–P
            pop_bit(white_bb, square);
        }

        Bitboard black_bb = get_piece(BLACK, piece);
        while (black_bb != 0)
        {
            Square square = lsb(black_bb);
            piece_board[square] = (piece + NUM_PIECES); // k–p
            pop_bit(black_bb, square);
        }
    }

}

Key Position::init_hash() 
{
    Key key = 0;

    for (Color color = 0; color < NUM_COLOR; color++) 
    {
        for (int piece = KING; piece <= PAWN; piece++) 
        {
            PieceType pt = static_cast<PieceType>(piece);
            Bitboard bitboard = get_piece(color, piece);
            
            while (bitboard != 0) 
            {
                int square = lsb(bitboard);
                key ^= zobrist.piece(color, piece, square);
                pop_bit(bitboard,square);
            }
        }
    }

    key ^= zobrist.castling(state->castling_rights);
    key ^= zobrist.color(side_to_move);
    key ^= zobrist.en_passant(state->ep_num);

    return key;    
}


void Position::set_king_square(Color color)
{
    state->king_square[color] = lsb(get_piece(color, KING));
}

void Position::set_check_squares(Color color)
{  
    set_king_square(color);
    Square ksq = get_king_square(color);
    Bitboard occ = 0ULL;

    state->check_squares[color][PAWN]   = AttackTables::pawn_attacks[color][ksq];
    state->check_squares[color][KNIGHT] = attacks_bb<KNIGHT>(ksq, occ);
    state->check_squares[color][BISHOP] = attacks_bb<BISHOP>(ksq, occ);
    state->check_squares[color][ROOK]   = attacks_bb<ROOK>(ksq, occ);
    state->check_squares[color][QUEEN]  = attacks_bb<QUEEN>(ksq, occ);
}

bool Position::is_square_attacked(Square sq, Color color)
{
    if (get_piece(color ^ 1, PAWN)    & AttackTables::pawn_attacks[color][sq])  { return true; }
    if (get_piece(color ^ 1, KNIGHT)  & attacks_bb<KNIGHT>(sq, occupancy))      { return true; }
    if (get_piece(color ^ 1, BISHOP)  & attacks_bb<BISHOP>(sq, occupancy))      { return true; }
    if (get_piece(color ^ 1, ROOK)    & attacks_bb<ROOK>(sq, occupancy))        { return true; }
    if (get_piece(color ^ 1, QUEEN)   & attacks_bb<QUEEN>(sq, occupancy))       { return true; }
    if (get_piece(color ^ 1, KING)    & attacks_bb<KING>(sq, occupancy))        { return true; }

    return false;
}

bool Position::is_square_attacked(Square sq, Color color, Bitboard occ)
{
    if (get_piece(color ^ 1, PAWN)    & AttackTables::pawn_attacks[color][sq])      { return true; }
    if (get_piece(color ^ 1, KNIGHT)  & attacks_bb<KNIGHT>(sq, occ))     { return true; }
    if (get_piece(color ^ 1, BISHOP)  & attacks_bb<BISHOP>(sq, occ))     { return true; }
    if (get_piece(color ^ 1, ROOK)    & attacks_bb<ROOK>(sq, occ))       { return true; }
    if (get_piece(color ^ 1, QUEEN)   & attacks_bb<QUEEN>(sq, occ))      { return true; }
    if (get_piece(color ^ 1, KING)    & attacks_bb<KING>(sq, occ))       { return true; }

    return false;
}


bool Position::is_check() const
{
    return bit_count(state->checkers_bb) != 0;
}

bool Position::is_double_check() const
{
    return bit_count(state->checkers_bb) > 1;
}

Bitboard Position::set_checkers_blockers(Color color)
{  
    Color us = color;
    Color enemy = color ^ 1;

    state->checkers_bb = 0ULL;
    state->blockers_for_king[us] = 0ULL;

    for(Piece piece = QUEEN; piece <= BISHOP; piece++ )
    {
        Bitboard attacker = get_piece(enemy, piece);
        attacker &= state->check_squares[us][piece];

        if(attacker != 0)
        {
            while (attacker)
            {
                Square from = pop_lsb(attacker);
                Bitboard bb = between_bb(from, get_king_square(us)) & occupancy;

                int count = bit_count(bb);

                if(count == 0)
                {
                    state->checkers_bb |= square_bb(from);
                }
                else if(count == 1 && ((bb & color_bb[enemy]) == 0))
                {
                    state->blockers_for_king[us] |= (bb & color_bb[us]);
                }
                
            }
        }
    }

    for(Piece piece = KNIGHT; piece <= PAWN; piece++ )
    {
        Bitboard attacker = get_piece(enemy, piece);
        state->checkers_bb |= (state->check_squares[us][piece] & attacker);
    }
    return state->checkers_bb;
}


U8 Position::can_castle(Color color) const
{
    U8 output = NO_CASTLING;

    if(color == WHITE)
    {
        if((WK & state->castling_rights) == WK && !get_bit(occupancy, f1) && !get_bit(occupancy, g1))
        { output |= WK; }
        if ((WQ & state->castling_rights) == WQ && !get_bit(occupancy, b1) && !get_bit(occupancy, c1) && !get_bit(occupancy, d1))
        { output |= WQ; }
        return output;
    }
    if(color == BLACK)
    {
        if((BK & state->castling_rights) == BK && !get_bit(occupancy, f8) && !get_bit(occupancy, g8))
        { output |= BK; }
        if ((BQ & state->castling_rights) == BQ && !get_bit(occupancy, b8) && !get_bit(occupancy, c8) && !get_bit(occupancy, d8))
        { output |= BQ; }
        return output;
    }

    return output;
}



StateInfo& Position::get_state()
{
    return state_history[state_index];
}

Piece Position::list_to_type(Square sq)
{
    Piece encoded_piece = piece_board[sq]; 
    return (encoded_piece < NUM_PIECES) ? (encoded_piece) : (encoded_piece - NUM_PIECES);
}

Piece Position::type_to_list(Piece piece, Color color)
{
    return piece + (color == WHITE ? 0 : NUM_PIECES);
}

bool Position::is_legal(Move m)
{
    Color us = side_to_move;
    Color enemy = us ^ 1;
    Square ksq = get_king_square(us);
    Square from = move_get_from(m);
    Square to = move_get_to(m);

    if(move_is_enpassant(m))
    {
        Bitboard occ = occupancy;
        Square captured_pawn_sq = StateInfo::ep_num_to_square(state->ep_num) + (us == WHITE ? SOUTH : NORTH);

        occ ^= (1ULL << from);              // remove moving pawn
        occ ^= (1ULL << captured_pawn_sq);  // remove captured pawn  
        occ |= (1ULL << to);                // add moving pawn to destination

        const Bitboard bishop_attackers = get_piece(enemy, BISHOP) | get_piece(enemy, QUEEN);
        const Bitboard rook_attackers = get_piece(enemy, ROOK) | get_piece(enemy, QUEEN);

        return (!(attacks_bb<BISHOP>(ksq, occ) & bishop_attackers) 
                && !(attacks_bb<ROOK>(ksq, occ) & rook_attackers));
    }

    if(move_is_castling(m))
    {
        Direction shift = (to > from ? EAST : WEST);
        Square cur = from;

        while(cur != to)
        {
            cur += shift; 
            if(is_square_attacked(cur, us)) { return false; }
        }
        return true;
    }

    if(move_get_piece(m) == KING)
    {
        Bitboard occ = occupancy;
        occ ^= (1ULL << from); 
        return (!is_square_attacked(to,  us, occ));
    }

    //first condition. check if it is blocker for king.

    //second condition. its a blocker os check if move is on line with pinner.

    if ((state->blockers_for_king[us] & (1ULL << from)) == 0)
    {
        return true;
    }

    return (through_bb(from,  to) & get_piece(us,  KING));
}

bool Position::is_mate(MoveList *ml)
{
    return (ml->count == 0 && is_check());
}

bool Position::enough_material(Color us)
{   
    if((bit_count(color_bb[us])) > 2) { return true; }

    if(get_piece(us, PAWN) != 0) { return true; }
    if(get_piece(us, QUEEN) != 0) { return true; }
    if(get_piece(us, ROOK) != 0) { return true; }

    int minor_count = bit_count(get_piece(us, BISHOP))
                    + bit_count(get_piece(us, KNIGHT));

    return (minor_count > 1) == true;
}   

bool Position::is_draw()
{
    if(state->half_move > 99) { return true; }

    StateInfo* state_iterator = &state_history[state_index - 1];
    int counter = 0;

    while(!move_is_capture(state_iterator->move) && move_get_piece(state_iterator->move) != PAWN)
    {
        if(state_iterator->hash == state->hash)
        {
            counter++;
        }
        state_iterator--;
    }

    if(counter >= 3) { return true; }

    if(enough_material(side_to_move) == false && enough_material(side_to_move ^ 1) == false)
    {
        return true;
    }
}