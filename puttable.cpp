#include <cassert>
#include "board.hpp"
#include "puttable.hpp"

std::map<line_t, uint64_t> puttable;

const int ID_MAX = 40;

uint64_t move_line(uint64_t line, int id)
{
    assert(0 <= id && id < ID_MAX);

    const int SIZE = 8;

    const uint64_t ul = 0x0103070F1F3F7FFF;
    const uint64_t ur = 0x80C0E0F0F8FCFEFF;
    const uint64_t ll = 0xFF7F3F1F0F070301;
    const uint64_t lr = 0xFFFEFCF8F0E0C080;

    // row
    if (id < 8)
    {
        return line << (SIZE * id);
    }
    // column
    if (id < 16)
    {
        line = (line & 0b00000001) |
               (line & 0b00000010) << (8 * 1 - 1) |
               (line & 0b00000100) << (8 * 2 - 2) |
               (line & 0b00001000) << (8 * 3 - 3) |
               (line & 0b00010000) << (8 * 4 - 4) |
               (line & 0b00100000) << (8 * 5 - 5) |
               (line & 0b01000000) << (8 * 6 - 6) |
               (line & 0b10000000) << (8 * 7 - 7);
        return line << (id - 8);
    }
    // diagonally left
    if (id < 28)
    {
        line = (line & 0b00000001) |
               (line & 0b00000010) << (9 * 1 - 1) |
               (line & 0b00000100) << (9 * 2 - 2) |
               (line & 0b00001000) << (9 * 3 - 3) |
               (line & 0b00010000) << (9 * 4 - 4) |
               (line & 0b00100000) << (9 * 5 - 5) |
               (line & 0b01000000) << (9 * 6 - 6) |
               (line & 0b10000000) << (9 * 7 - 7);
        if (id < 22)
        {
            return (line << (id - 16)) & ur;
        }
        else
        {
            return (line >> (id - 22)) & ll;
        }
    }
    // diagonally right
    if (id < 40)
    {
        line = (line & 0b00000001) << (7 * 1 - 0) |
               (line & 0b00000010) << (7 * 2 - 1) |
               (line & 0b00000100) << (7 * 3 - 2) |
               (line & 0b00001000) << (7 * 4 - 3) |
               (line & 0b00010000) << (7 * 5 - 4) |
               (line & 0b00100000) << (7 * 6 - 5) |
               (line & 0b01000000) << (7 * 7 - 6) |
               (line & 0b10000000) << (7 * 8 - 7);
        if (id < 34)
        {
            return (line << (id - 28)) & lr;
        }
        else
        {
            return (line >> (id - 34)) & ul;
        }
    }

    assert(false);
    return 0;
}

line_t move_line(Board line, int id)
{
    return std::make_pair(move_line(line.get_black(), id), move_line(line.get_white(), id));
}

void precalc_puttable()
{
    for (int bl = 0; bl < 1 << 8; bl++)
        for (int wl = 0; wl < 1 << 8; wl++)
        {
            uint64_t hands = 0;
            if (bl & wl)
                continue;
            Board line = Board((uint64_t)bl, (uint64_t)wl);
            for (int i = 0; i < 8; i++)
            {
                Board put_line = line;
                uint64_t hand = 1ULL << i;
                if (!line.no_stone(hand))
                    continue;
                if (put_line.put(hand))
                {
                    hands |= hand;
                }
            }

            if (hands)
            {
                for (int id = 0; id < ID_MAX; id++)
                {
                    uint64_t all_line = line.get_black() | line.get_white();
                    if (__builtin_popcountll(move_line(all_line, id)) == __builtin_popcountll(all_line))
                        puttable[move_line(line, id)] = move_line(hands, id);
                }
            }
        }
}

line_t filter(Board board, uint64_t mask)
{
    return std::make_pair(board.get_black() & mask, board.get_white() & mask);
}

uint64_t get_puttable(Board board)
{
    assert(!puttable.empty()); // call precalc_puttable
    const uint64_t ul = 0x0103070F1F3F7FFF;
    const uint64_t ur = 0x80C0E0F0F8FCFEFF;
    const uint64_t ll = 0xFF7F3F1F0F070301;
    const uint64_t lr = 0xFFFEFCF8F0E0C080;
    const int SIZE = board.get_size();
    uint64_t mobilities = 0;
    for (int col = 0; col < SIZE; col++)
    {
        line_t filtered = filter(board, 0x0101010101010101 << col);
        if (puttable.find(filtered) != puttable.end())
            mobilities |= puttable.at(filtered);
    }
    for (int row = 0; row < SIZE; row++)
    {
        line_t filtered = filter(board, 0b11111111ULL << (SIZE * row));
        if (puttable.find(filtered) != puttable.end())
            mobilities |= puttable.at(filtered);
    }
    for (int dir = 0; dir < SIZE - 2; dir++)
    {
        line_t filtered = filter(board, (0x0102040810204080 << dir) & lr);
        if (puttable.find(filtered) != puttable.end())
            mobilities |= puttable.at(filtered);
    }
    for (int dir = 1; dir < SIZE - 2; dir++)
    {
        line_t filtered = filter(board, (0x0102040810204080 >> dir) & ul);
        if (puttable.find(filtered) != puttable.end())
            mobilities |= puttable.at(filtered);
    }
    for (int dil = 0; dil < SIZE - 2; dil++)
    {
        line_t filtered = filter(board, (0x8040201008040201 << dil) & ur);
        if (puttable.find(filtered) != puttable.end())
            mobilities |= puttable.at(filtered);
    }

    for (int dil = 1; dil < SIZE - 2; dil++)
    {
        line_t filtered = filter(board, (0x8040201008040201 >> dil) & ll);
        if (puttable.find(filtered) != puttable.end())
            mobilities |= puttable.at(filtered);
    }

    return mobilities;
}