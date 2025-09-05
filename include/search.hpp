#pragma once
#include "move.hpp"
#include "types.hpp"
#include "position.hpp"
#include "eval.hpp"
#include <array>

enum NodeType : U8
{
    EXACT, UPPER, LOWER 
};

struct TTEntry
{
    Key hash;
    U8 depth;
    Score score;
    NodeType type;
    Move best_move = null_move;
};


struct TranspositionTable
{
    std::unique_ptr<std::array<TTEntry, 5000000>> table;
    size_t count;

    void add_entry(Key hash, U8 depth, Score score, NodeType type, Move best_move);
};

class Search
{
public:

    Position &pos;
    Evalution eval;
    TranspositionTable tt;

    Score nega_max(int depth, int alpha, int beta);
    Move search_root();
    Move next_best_move(MoveList& ml, int start, int end);

    int quiescence_search(int alpha, int beta);

    Search();
    Search(Position &position);
};