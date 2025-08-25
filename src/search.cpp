#include "search.hpp"
#include "move_gen.hpp"
#include "position.hpp"
#include "types.hpp"
#include <iostream>


int Search::search(int depth, int alpha, int beta, Position* pos)
{
    if (depth == 0) {
        return evaluate(*pos);
    }

    MoveList ml;

    generate<LEGAL>(*pos, ml);

    if(depth == SEARCH_DEPTH)
    {
        best_move_found = ml.get_move(0);
    }

    if (pos->is_mate(&ml)) {
        return negative_infinity; 
    }

    if (pos->is_draw()) {
        return 0;
    }

    int best_eval = negative_infinity;

    for (int i = 0; i < ml.get_count(); ++i)
    {
        Move move = ml.move_list[i];

        pos->make_move(move);
        int eval = -search(depth - 1, -beta, -alpha, pos);
        pos->unmake_move();

        if (eval > best_eval) {
            best_eval = eval;

            if (depth == SEARCH_DEPTH) {
                best_move_found = move;
            }
        }

        alpha = std::max(alpha, eval);
        if (alpha >= beta) {
            break;
        }
    }

    return best_eval;
}

Move Search::best_move()
{
    search(SEARCH_DEPTH, negative_infinity, positive_infinity, &pos);
    return best_move_found;
}