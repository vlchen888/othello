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
public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);
    Move *getMove();
    int heuristic(Board *curr, Side side);
    int minimax(Board *curr, Side side, int depthLeft);

    void setBoard(Board *b);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
