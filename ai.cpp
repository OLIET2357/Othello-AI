#include "board.hpp"
#include "nega_scout.hpp"
#include "nega_max.hpp"
#include "evaluation_functions.hpp"
#include "puttable.hpp"
#include "ai.hpp"

AI::AI(Method method, int depth)
{
    this->method = method;
    this->depth = depth;
}

score_t AI::get_score(const Board &board) const
{
    const int PRESEARCH_DEPTH = 4;

    // precalc_puttable();

    switch (this->method)
    {
    case Method::STANDARD:
    {
        const scout_ret_t prescore_hands = nega_scout(eval_standard, board, PRESEARCH_DEPTH);
        return -nega_max(eval_standard, board, depth, prescore_hands);
    }
    case Method::CONFIDENT:
        return -nega_max(eval_confident, board, depth);
    case Method::PERFECT:
        return -nega_max(eval_perfect, board, depth);
    default:
        return 0;
    }
}