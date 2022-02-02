#pragma once

#include "evaluation_functions.hpp"
#include "board.hpp"
#include "nega_scout.hpp"

score_t nega_max(const EvalFunc &ef, const Board &board, int limit);
score_t nega_max(const EvalFunc &ef, const Board &board, int limit, scout_ret_t prescore_hands);