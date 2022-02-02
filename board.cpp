#include <iostream>
#include <string>
#include <cassert>
#include "board.hpp"

enum class Direction
{
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_UPPER,
    DIRECTION_LOWER,
    DIRECTION_UPPER_LEFT,
    DIRECTION_LOWER_RIGHT,
    DIRECTION_UPPER_RIGHT,
    DIRECTION_LOWER_LEFT,
};

int Board::SIZE;

void Board::set_size(int SIZE)
{
    Board::SIZE = SIZE;
}

bool Board::no_stone(uint64_t hand) const
{
    return ((this->black | this->white) & hand) == 0;
}

uint64_t Board::transfer(uint64_t m, Direction d) const
{
    switch (d)
    {
    case Direction::DIRECTION_LEFT:
        return (m & 0xFEFEFEFEFEFEFEFE) >> 1;
    case Direction::DIRECTION_RIGHT:
        return (m & 0x7F7F7F7F7F7F7F7F) << 1;
    case Direction::DIRECTION_UPPER:
        return (m & 0xFFFFFFFFFFFFFF00) >> SIZE;
    case Direction::DIRECTION_LOWER:
        return (m & 0x00FFFFFFFFFFFFFF) << SIZE;
    case Direction::DIRECTION_UPPER_LEFT:
        return (m & 0xFEFEFEFEFEFEFE00) >> (SIZE + 1);
    case Direction::DIRECTION_LOWER_RIGHT:
        return (m & 0x007F7F7F7F7F7F7F) << (SIZE + 1);
    case Direction::DIRECTION_UPPER_RIGHT:
        return (m & 0x7F7F7F7F7F7F7F00) >> (SIZE - 1);
    case Direction::DIRECTION_LOWER_LEFT:
        return (m & 0x00FEFEFEFEFEFEFE) << (SIZE - 1);
    default:
        assert(false);
    }
    return 0;
}

uint64_t Board::get_rev(uint64_t mov, Direction direction) const
{
    uint64_t rev = 0;
    uint64_t mask = transfer(mov, direction);
    while (mask != 0 && (mask & this->white) != 0)
    {
        rev |= mask;
        mask = transfer(mask, direction);
    }
    if ((mask & this->black) == 0)
        return 0;
    else
        return rev;
}

bool Board::put(uint64_t hand)
{
    assert(no_stone(hand));
    uint64_t revs = 0;
    bool flipped = false;
    for (int d = 0; d < 8; d++)
    {
        uint64_t rev = get_rev(hand, static_cast<Direction>(d));
        revs |= rev;
        if (rev != 0)
        {
            flipped = true;
        }
    }

    this->black ^= revs | hand;
    this->white ^= revs;
    return flipped;
}

Board::Board(uint64_t black, uint64_t white)
{
    assert(SIZE != 0);
    assert(SIZE <= 8);
    this->black = black;
    this->white = white;
}

int Board::get_size() const
{
    return this->SIZE;
}

uint64_t Board::get_black() const
{
    return this->black;
}

uint64_t Board::get_white() const
{
    return this->white;
}

int Board::turn() const
{
    int stones = __builtin_popcountll(this->black) + __builtin_popcountll(this->white);
    return stones - 4 + 1;
}

bool Board::isfull() const
{
    return __builtin_popcountll(this->black) + __builtin_popcountll(this->white) == SIZE * SIZE;
}

void Board::reverse()
{
    std::swap(this->black, this->white);
}

Board Board::get_reverse() const
{
    return Board(this->white, this->black);
}

std::ostream &Board::to_stream(std::ostream &os) const
{
    const std::string BLACK_PIECE = "X";
    const std::string WHITE_PIECE = "O";
    const std::string EMPTY_PIECE = ".";

    os << " ";
    for (int i = 0; i < SIZE; i++)
    {
        os << (char)('a' + i);
    }
    os << "\n";

    for (int i = 0; i < SIZE; i++)
    {
        os << i + 1;
        for (int j = 0; j < SIZE; j++)
        {
            if ((this->black >> (i * SIZE + j)) & 1)
            {
                os << BLACK_PIECE;
            }
            else if ((this->white >> (i * SIZE + j)) & 1)
            {
                os << WHITE_PIECE;
            }
            else
            {
                os << EMPTY_PIECE;
            }
        }
        os << "\n";
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Board &board)
{
    board.to_stream(os);
    return os;
}