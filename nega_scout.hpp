#pragma once

#include <vector>
#include <utility>
#include "board.hpp"
#include "evaluation_functions.hpp"

using scout_ret_t = std::vector<std::pair<score_t, uint64_t>>;

scout_ret_t nega_scout(const EvalFunc &ef, const Board &board, int limit);
