#include <iostream>
#include <cassert>
#include <climits>
#include "board.hpp"
#include "ai.hpp"

using namespace std;

int main()
{
    char method_char;
    cin >> method_char;
    int depth;
    cin >> depth;
    int SIZE;
    cin >> SIZE;
    Board::set_size(SIZE);

    uint64_t black = 0, white = 0;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            int c;
            cin >> c;

            switch (c)
            {
            case 1:
                black |= 1ULL << (i * SIZE + j);
                break;
            case -1:
                white |= 1ULL << (i * SIZE + j);
                break;
            }
        }
    }

    Board board = Board(black, white);

#ifdef DEBUG
    cerr << board << endl;
#endif

    int move_x;
    int move_y;
    cin >> move_x >> move_y;

    uint64_t hand = 1ULL << (move_y * SIZE + move_x);

    assert(board.put(hand)); // Can't put there

    board.reverse();

#ifdef DEBUG
    cerr << board << endl;
#endif

    Method method;
    switch (method_char)
    {
    case 'S':
    case 's':
        method = Method::STANDARD;
        break;
    case 'C':
    case 'c':
        method = Method::CONFIDENT;
        break;
    case 'P':
    case 'p':
        method = Method::PERFECT;
        break;
    default:
        assert(false); // Invalid method character
        return 1;
    }

    score_t score = AI(method, depth).get_score(board);
    cout << score << endl;
}
