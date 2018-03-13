#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"

#define flip(side) (side == BLACK ? WHITE : BLACK)
#define min(a, b) (a < b ? a : b)

using namespace std;

class Player {
private:
    Board* board;
    Side mySide;
    Side oppSide;
    int bestX;
    int bestY;
    clock_t start;
    bool outOfTime;
public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);
    Move *getMove(int msLeft);
    int heuristic(Board *curr, Side side);
    int minimax(Board *curr, Side side, int depthLeft);
    double alphabeta(Board *curr, bool root, Side side, int depth,
            int alpha, int beta, int msLeft);

    void setBoard(Board *b);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
