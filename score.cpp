#include <iostream>
#include <cassert>
#include <climits>
#include "board.hpp"
#include "ai.hpp"

using namespace std;

int main()
{
    int presearch_depth;
    int standard_depth;
    int confident_depth;
    int perfect_depth;
    cin >> presearch_depth >> standard_depth >> confident_depth >> perfect_depth;
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

    assert(board.put(hand));

    board.reverse();

#ifdef DEBUG
    cerr << board << endl;
#endif

    score_t score = AI(presearch_depth, standard_depth, confident_depth, perfect_depth).get_score(board);
    cout << score << endl;
}
