#pragma once

#include "baseai.hpp"
#include "board.hpp"
#include "evaluation_functions.hpp"

class AI : BaseAI
{
private:
    int presearch_depth;
    int standard_depth;
    int confident_depth;
    int perfect_depth;

public:
    AI(int presearch_depth, int standard_depth, int confident_depth, int perfect_depth);
    score_t get_score(const Board &board) const;
};