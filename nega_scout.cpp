#include <vector>
#include <tuple>
#include <algorithm>
#include "nega_max.hpp"
#include "nega_scout.hpp"

scout_ret_t nega_scout(const EvalFunc &ef, const Board &board, int limit)
{
    scout_ret_t prescore_hands;
    const int SIZE = board.get_size();
    for (int i = 0; i < SIZE * SIZE; i++)
    {
        uint64_t hand = 1ULL << i;
        if (!board.no_stone(hand))
        {
            continue;
        }
        Board next_board = board;
        if (next_board.put(hand))
        {
            next_board.reverse();
            score_t score = -nega_max(ef, next_board, limit);
            prescore_hands.push_back(std::make_pair(score, hand));
        }
    }

    std::sort(prescore_hands.begin(), prescore_hands.end(), [](auto a, auto b)
              { return a.first > b.first; });

    return prescore_hands;
}