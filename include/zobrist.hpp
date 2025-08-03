#pragma once
#include "types.hpp"
#include <array>

struct ZobristRandoms
{
    std::array<std::array<std::array<Key,NUM_SQUARES>,NUM_PIECES>,NUM_COLOR> rnd_pieces;
    std::array<Key, NUM_CASTLING> rnd_castling;
    std::array<Key, NUM_COLOR> rnd_color;
    std::array<Key, NUM_EPSQUARES> rnd_ep;

    ZobristRandoms();

    Key piece(Color color, Piece piece, Square square) const;

    Key castling(U8 castling_permissions) const;
    
    Key color(Color color) const;
    
    Key en_passant(Square en_passant_square) const;

};
