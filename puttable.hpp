#pragma once

#include <map>
#include <utility>
#include "board.hpp"

using line_t = std::pair<uint64_t, uint64_t>;

void precalc_puttable();
uint64_t get_puttable(Board board);