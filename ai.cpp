#include <cassert>
#include <limits>
#include "board.hpp"
#include "nega_scout.hpp"
#include "nega_max.hpp"
#include "evaluation_functions.hpp"
#include "ai.hpp"

#ifdef DEBUG
#include <iostream>
#endif

AI::AI(int presearch_depth, int standard_depth, int confident_depth, int perfect_depth)
{
    this->presearch_depth = presearch_depth;
    this->standard_depth = standard_depth;
    this->confident_depth = confident_depth;
    this->perfect_depth = perfect_depth;
}

score_t AI::get_score(const Board &board) const
{
    int turn = board.turn();
    const int SIZE = board.get_size();
    const int MAX_TURN = SIZE * SIZE - 4 + 1;
    if (turn >= MAX_TURN - perfect_depth)
    {
        return -nega_max(eval_perfect, board, perfect_depth);
    }
    if (turn >= MAX_TURN - confident_depth)
    {
        return -nega_max(eval_confident, board, confident_depth);
    }

    scout_ret_t prescore_hands = nega_scout(eval_standard, board, presearch_depth);
#ifdef DEBUG
    for (auto prescore_hand : prescore_hands)
    {
        std::cerr << prescore_hand.first << " " << prescore_hand.second << std::endl;
    }
#endif
    return -nega_max(eval_standard, board, standard_depth, prescore_hands);
}