#pragma once

#include <functional>
#include "board.hpp"

using score_t = int;
using EvalFunc = std::function<score_t(const Board &)>;

extern EvalFunc eval_standard;
extern EvalFunc eval_confident;
extern EvalFunc eval_perfect;