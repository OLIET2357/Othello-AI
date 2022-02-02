#pragma once

#include "board.hpp"
#include "evaluation_functions.hpp"

class BaseAI
{
public:
    score_t get_score(const Board &board) const;
};