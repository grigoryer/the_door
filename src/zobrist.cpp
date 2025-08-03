#include "zobrist.hpp"
#include "types.hpp"
#include <random>
#include <ctime>

ZobristRandoms::ZobristRandoms() //NOLINT
{
    std::mt19937_64 random(time(0));


    for(int i = 0; i < NUM_COLOR; ++i)
    {
        rnd_color[i] = random();

        for(int j = 0; j < NUM_PIECES; ++j)
        {
            for(int k = 0; k < NUM_SQUARES; ++k)
            {
                rnd_pieces[i][j][k] = random();
            }
        }
    }

    for(int i = 0; i < NUM_CASTLING; ++i)
    {
        rnd_castling[i] = random();
    }

    for(int i = 0; i < NUM_EPSQUARES; ++i)
    {
        rnd_ep[i] = random();
    }
}

Key ZobristRandoms::en_passant(Square en_passant_square) const {
    return rnd_ep[en_passant_square]; 
}

Key ZobristRandoms::piece(Color side, Piece piece, Square square) const {
    return rnd_pieces[side][piece][square];
}

Key ZobristRandoms::castling(U8 castling_permissions) const {
    return rnd_castling[castling_permissions];
}
    
Key ZobristRandoms::color(Color color) const {
    return rnd_color[color];
}