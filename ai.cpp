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
    // precalc_puttable();

    switch (this->method)
    {
    case Method::STANDARD:
        return -nega_max(eval_standard, board, depth);
    case Method::CONFIDENT:
        return -nega_max(eval_confident, board, depth);
    case Method::PERFECT:
        return -nega_max(eval_perfect, board, depth);
    default:
        return 0;
    }
}