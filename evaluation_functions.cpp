#include <limits>
#include "board.hpp"
#include "evaluation_functions.hpp"

const score_t MOBILITY_WEIGHT = +1;
const score_t LIBERTY_WEIGHT_INVERSE = -5;
const score_t WING_WEIGHT = -5;
const score_t STABLE_WEIGHT = +2;
const score_t C_WEIGHT = -10;
const score_t X_WEIGHT = -10;

score_t mobility(const Board &board)
{
    score_t ret = 0;
    const int SIZE = board.get_size();
    for (int i = 0; i < SIZE * SIZE; i++)
    {
        uint64_t hand = 1ULL << i;
        if (!board.no_stone(hand))
            continue;
        Board _board = board;
        if (_board.put(hand))
            ret++;
    }
    return ret;
}

score_t liberty(const Board &board)
{
    score_t ret = 0;
    const int SIZE = board.get_size();
    uint64_t bw = board.get_black() | board.get_white();
    const uint64_t s = 0b1110000010100000111;
    const uint64_t l = 0x1F1F1F1F1F1F1F1F;
    const uint64_t r = 0xF8F8F8F8F8F8F8F8;

    for (int i = 0; i < SIZE * SIZE; i++)
    {
        uint64_t hand = 1ULL << i;
        if ((board.get_black() & hand) == 0)
        {
            continue;
        }
        int j = i - 9;
        uint64_t m;
        if (j >= 0)
        {
            m = s << j;
        }
        else
        {
            m = s >> (-j);
        }
        if (i % 8 < 4)
        {
            m &= l;
        }
        else
        {
            m &= r;
        }
        ret += __builtin_popcountll(bw & m);
    }

    return ret;
}

score_t wing(uint64_t stone)
{
    score_t ret = 0;
    uint64_t edge_upper = stone & 0b11111111;
    if (edge_upper == 0b00111110 || edge_upper == 0b01111100 ||
        edge_upper == 0b10111110 || edge_upper == 0b01111101)
        ret++;

    uint64_t edge_left = stone & 0x0101010101010101;
    if (edge_left == 0x0000010101010100 || edge_left == 0x0001010101010000 ||
        edge_left == 0x0100010101010100 || edge_left == 0x0001010101010001)
        ret++;

    uint64_t edge_right = stone & 0x8080808080808080;
    if (edge_right == 0x0000808080808000 || edge_right == 0x0080808080800000 ||
        edge_right == 0x8000808080808000 || edge_right == 0x0080808080800080)
        ret++;

    uint64_t edge_lower = stone & (0b11111111ULL << 56);
    if (edge_lower == (0b00111110ULL << 56) || edge_lower == (0b01111100ULL << 56) ||
        edge_lower == (0b10111110ULL << 56) || edge_lower == (0b01111101ULL << 56))
        ret++;

    return ret;
}

score_t stable(uint64_t stone)
{
    score_t ret = 0;
    uint64_t edge_upper = stone & 0b11111111;
    if (edge_upper == 0b00000011 || edge_upper == 0b11000000)
        ret += 1;
    if (edge_upper == 0b00000111 || edge_upper == 0b11100000)
        ret += 2;
    if (edge_upper == 0b00001111 || edge_upper == 0b11110000)
        ret += 3;
    if (edge_upper == 0b00011111 || edge_upper == 0b11111000)
        ret += 4;
    if (edge_upper == 0b00111111 || edge_upper == 0b11111100)
        ret += 5;
    if (edge_upper == 0b01111111 || edge_upper == 0b11111110 || edge_upper == 0b11111111)
        ret += 6;

    uint64_t edge_left = stone & 0x0101010101010101;
    if (edge_left == 0x0000000000000101 || edge_left == 0x0101000000000000)
        ret += 1;
    if (edge_left == 0x0000000000010101 || edge_left == 0x0101010000000000)
        ret += 2;
    if (edge_left == 0x0000000001010101 || edge_left == 0x0101010100000000)
        ret += 3;
    if (edge_left == 0x0000000101010101 || edge_left == 0x0101010101000000)
        ret += 4;
    if (edge_left == 0x0000010101010101 || edge_left == 0x0101010101010000)
        ret += 5;
    if (edge_left == 0x0001010101010101 || edge_left == 0x0101010101010100 || edge_left == 0x0101010101010101)
        ret += 6;

    uint64_t edge_right = stone & 0x8080808080808080;
    if (edge_right == 0x0000000000008080 || edge_right == 0x8080000000000000)
        ret += 1;
    if (edge_right == 0x0000000000808080 || edge_right == 0x8080800000000000)
        ret += 2;
    if (edge_right == 0x0000000080808080 || edge_right == 0x8080808000000000)
        ret += 3;
    if (edge_right == 0x0000008080808080 || edge_right == 0x8080808080000000)
        ret += 4;
    if (edge_right == 0x0000808080808080 || edge_right == 0x8080808080800000)
        ret += 5;
    if (edge_right == 0x0080808080808080 || edge_right == 0x8080808080808000 || edge_right == 0x8080808080808080)
        ret += 6;

    uint64_t edge_lower = stone & (0b11111111ULL << 56);
    if (edge_lower == (0b00000011ULL << 56) || edge_lower == (0b11000000ULL << 56))
        ret += 1;
    if (edge_lower == (0b00000111ULL << 56) || edge_lower == (0b11100000ULL << 56))
        ret += 2;
    if (edge_lower == (0b00001111ULL << 56) || edge_lower == (0b11110000ULL << 56))
        ret += 3;
    if (edge_lower == (0b00011111ULL << 56) || edge_lower == (0b11111000ULL << 56))
        ret += 4;
    if (edge_lower == (0b00111111ULL << 56) || edge_lower == (0b11111100ULL << 56))
        ret += 5;
    if (edge_lower == (0b01111111ULL << 56) || edge_lower == (0b11111110ULL << 56) || edge_lower == (0b11111111ULL << 56))
        ret += 6;

    return ret;
}

score_t x(uint64_t stone)
{
    score_t ret = 0;
    if ((stone & 0x0000000000000001) == 0)
        ret += __builtin_popcountll(stone & 0x0000000000000200);
    if ((stone & 0x0000000000000080) == 0)
        ret += __builtin_popcountll(stone & 0x0000000000004000);
    if ((stone & 0x0100000000000080) == 0)
        ret += __builtin_popcountll(stone & 0x0002000000000000);
    if ((stone & 0x8000000000000080) == 0)
        ret += __builtin_popcountll(stone & 0x0040000000000000);

    return ret;
}

score_t c(uint64_t stone)
{
    score_t ret = 0;
    if ((stone & 0x0000000000000001) == 0)
        ret += __builtin_popcountll(stone & 0x0000000000000102);
    if ((stone & 0x0000000000000080) == 0)
        ret += __builtin_popcountll(stone & 0x0000000000008040);
    if ((stone & 0x0100000000000080) == 0)
        ret += __builtin_popcountll(stone & 0x0201000000000000);
    if ((stone & 0x8000000000000080) == 0)
        ret += __builtin_popcountll(stone & 0x4080000000000000);

    return ret;
}

score_t eval_standard_black(const Board &board)
{
    uint64_t stone = board.get_black();
    return mobility(board) * MOBILITY_WEIGHT +
           liberty(board) / LIBERTY_WEIGHT_INVERSE +
           wing(stone) * WING_WEIGHT +
           stable(stone) * STABLE_WEIGHT +
           x(stone) * X_WEIGHT +
           c(stone) * C_WEIGHT;
}

EvalFunc eval_standard = [](const Board &board)
{
    if (__builtin_popcountll(board.get_black()) == 0)
        return std::numeric_limits<score_t>::min();
    if (__builtin_popcountll(board.get_white()) == 0)
        return std::numeric_limits<score_t>::max();

    return eval_standard_black(board) - eval_standard_black(board.get_reverse());
};

EvalFunc eval_confident = [](const Board &board)
{
    score_t perfect = eval_perfect(board);
    return (perfect > 0) - (perfect < 0);
};

EvalFunc eval_perfect = [](const Board &board)
{
    return __builtin_popcountll(board.get_black()) - __builtin_popcountll(board.get_white());
};