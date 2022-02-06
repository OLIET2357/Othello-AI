#include <limits>
#include "board.hpp"
#include "nega_scout.hpp"
#include "nega_max.hpp"

score_t nega_max(const EvalFunc &ef, const Board &board, int limit, score_t alpha, score_t beta)
{
    if (limit == 0 || board.isfull())
    {
        return ef(board);
    }
    score_t score_max = std::numeric_limits<score_t>::min();
    bool pass = true;
    const int SIZE = board.get_size();
    for (int i = 0; i < SIZE * SIZE; i++)
    {
        uint64_t hand = 1ULL << i;
        if (!board.no_stone(hand))
        {
            continue;
        }

        Board next_board = board;

        bool flipped = next_board.put(hand);
        if (flipped)
        {
            pass = false;

            next_board.reverse();

            score_t score = -nega_max(ef, next_board, limit - 1, -beta, -alpha);
            if (score >= beta)
            {
                return score;
            }
            if (score > score_max)
            {
                score_max = score;
                if (score_max > alpha)
                {
                    alpha = score_max;
                }
            }
        }
    }
    if (pass)
    {
        return -nega_max(ef, board.get_reverse(), limit - 1, -beta, -alpha);
    }

    return score_max;
}

score_t nega_max(const EvalFunc &ef, const Board &board, int limit)
{
    return nega_max(ef, board, limit, std::numeric_limits<score_t>::min() + 1, std::numeric_limits<score_t>::max());
}

score_t nega_max(const EvalFunc &ef, const Board &board, int limit, const scout_ret_t &prescore_hands)
{
    score_t alpha = std::numeric_limits<score_t>::min() + 1;
    score_t beta = std::numeric_limits<score_t>::max();
    score_t score_max = std::numeric_limits<score_t>::min();
    if (prescore_hands.size() == 0)
    {
        return -nega_max(ef, board.get_reverse(), limit - 1, -beta, -alpha);
    }
    for (auto prescore_hand : prescore_hands)
    {
        uint64_t hand = prescore_hand.second;
        Board next_board = board;
        next_board.put(hand);
        next_board.reverse();
        score_t score = -nega_max(ef, next_board, limit - 1, -beta, -alpha);
        if (score >= beta)
        {
            return score;
        }
        if (score > score_max)
        {
            score_max = score;
            if (score_max > alpha)
            {
                alpha = score_max;
            }
        }
    }
    return score_max;
}