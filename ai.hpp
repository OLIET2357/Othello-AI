#pragma once

#include "baseai.hpp"
#include "board.hpp"
#include "evaluation_functions.hpp"

enum class Method
{
    STANDARD,
    CONFIDENT,
    PERFECT,
};

class AI : BaseAI
{
private:
    Method method;
    int depth;

public:
    AI(Method method, int depth);
    score_t get_score(const Board &board) const;
};