#include "player.hpp"

// Victor was here!

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    board = new Board();
    // Set colors
    mySide = side;
    oppSide = flip(side);
    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {

    // Process opponent's move by updating board
    board->doMove(opponentsMove, oppSide);
    // Find and do my move, and return
    Move *myMove = getMove();
    //std::cerr << "myMove: " << myMove << std::endl;
    board->doMove(myMove, mySide);
    return myMove;
}

/**
 * @brief Using the current board state, choses the next move to make
 * using a minimax algorithm!
 */
Move *Player::getMove() {

    int minimaxDepth;
    if(testingMinimax) {
        minimaxDepth = 2;
    } else {
        minimaxDepth = 3;
    }
    int bestScore = INT_MAX;
    Move *bestMove = nullptr;
    // Update with getValidMoves
    bitset<64> validMoves = board->getValidMoves(mySide);
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(validMoves[i + 8*j]) {
                Move *move = new Move(i, j);
                // Inefficient memory usage again
                Board *tmp = board->copy();
                tmp->doMove(move, mySide);
                int oppScore = minimax(tmp, flip(mySide), minimaxDepth - 1);
                // Update best move found
                if(oppScore < bestScore) {
                    bestScore = oppScore;
                    delete bestMove;
                    bestMove = move;
                } else {
                    delete move;
                }
                delete tmp;
            }
        }

    }
    
    return bestMove;
}

/**
 * @brief The heuristic for the minimax tree
 */
int Player::heuristic(Board *curr, Side side) {
    return curr->count(side) - curr->count(flip(side));
}

/**
 * @brief Given a board state and a side, return a heuristic
 *        representing the strength evaluation of the board state for
 *        this side.
 */
int Player::minimax(Board *curr, Side side, int depthLeft) {
    // Very inefficient iterative solution
    if(depthLeft == 0) {
        return heuristic(curr, side);
    }
    int minmax = INT_MAX;

    bitset<64> validMoves = curr->getValidMoves(side);
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(validMoves[i + 8*j]) {
                Move move(i, j);
                Board *tmp = curr->copy();
                tmp->doMove(&move, side);
                minmax = min(minmax, minimax(tmp, flip(side), depthLeft - 1));
                // Very inefficient memory usage
                delete tmp;
            }
        }
    }
    // Check if you can pass
    if(curr->checkMove(nullptr, side)) {
        minmax = min(minmax, minimax(curr, flip(side), depthLeft - 1));
    }

    // minmax is the smallest possible gain the adversary can have
    // so return -minmax for the largest possible gain we can have
    // (which is the strength of our position)
    return -1*minmax;
}

/**
 * @brief For testing minimax
 */
void Player::setBoard(Board *b) {
    delete board;
    board = b;
}




