#pragma once

#include <iostream>

enum class Direction;

class Board
{
private:
    static int SIZE;
    uint64_t black, white;

    uint64_t transfer(uint64_t m, Direction d) const;
    uint64_t get_rev(uint64_t mov, Direction direction) const;

    friend std::ostream &operator<<(std::ostream &os, const Board &board);

public:
    static void set_size(int SIZE);
    Board(uint64_t black, uint64_t white);
    int get_size() const;
    uint64_t get_black() const;
    uint64_t get_white() const;
    int turn() const;
    bool no_stone(uint64_t hand) const;
    bool isfull() const;
    Board get_reverse() const;
    bool put(uint64_t hand);
    void reverse();
    std::ostream &to_stream(std::ostream &os) const;
};