#include "search.hpp"
#include "move.hpp"
#include "move_gen.hpp"
#include "types.hpp"
#include <algorithm>


const int INITIAL_DEPTH = 7;



Search::Search(Position &position) : pos(position)
{

}

Move Search::search_root()
{

    if(pos.is_draw())
    {
        pos.draw_flag = true;
        return null_move;
    }

    MoveList ml;
    generate<LEGAL>(pos , ml);

    if(ml.count == 0)
    {
        if(pos.is_check())
        {
            pos.mate_flag = true;
            return null_move;
        }
        else {
            pos.draw_flag = true;
            return null_move;
        }
    }

    int score;
    int best_score = negative_infinity;

    Move best_move = ml.move_list[0];

    for(int i = 0; i < ml.count; ++i)
    {
        pos.make_move(ml.get_move(i));
        score = -nega_max(INITIAL_DEPTH - 1, negative_infinity, positive_infinity);
        pos.unmake_move();
        if(score > best_score)
        {
            best_score = score;
            best_move = ml.get_move(i);
        }
    }
    
    return best_move;
}


 
Score Search::nega_max(int depth, int alpha, int beta)
{
    if(depth == 0) { return Evalution::evaluate(pos); } 
    
    MoveList ml;
    generate<LEGAL>(pos , ml);

    if(pos.is_draw())
    {
        return 0;
    }

    if(ml.count == 0)
    {
        if(pos.is_check())
        {
            return negative_infinity;
        }
        else {
            return 0;
        }
    }

    ml.score_moves(pos);
    int best_score = negative_infinity;

    for(int i = 0; i < ml.count; ++i)
    {
        next_best_move(ml, i, ml.count);
        pos.make_move(ml.get_move(i));
        int score = -nega_max(depth - 1, -beta, -alpha);
        pos.unmake_move();
        best_score = std::max(best_score, score);
        alpha = std::max(alpha, score);

        if(beta <= alpha)
        {
            break;
        }
    }

    return best_score;
}


Move Search::next_best_move(MoveList& ml, int start, int end)
{
    int best = start;

    for(int i = start + 1; i < end; i++) {
        if (ml.scores[i] > ml.scores[best])
            best = i;
    }
    std::swap(ml.scores[start], ml.scores[best]);
    std::swap(ml.move_list[start], ml.move_list[best]);
    return ml.move_list[best];
}

