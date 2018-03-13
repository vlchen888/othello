#include "player.hpp"
#include <climits>
#include "time.h"
#define USE_DOT_PRODUCT_HEURISTIC 1
#define MAX_DEPTH 30
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
    bestX = -1;
    bestY = -1;
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

    if (msLeft == -1) {
        msLeft = 10000;
    }

    // For scaling the amount of time each move is allotted. Should be
    // less than 2 to ensure that we do not run out of time.
    double param = 2;
    // Process opponent's move by updating board
    board->doMove(opponentsMove, oppSide);
    // Find and do my move, and return
    Move *myMove = getMove(int(param * msLeft / board->getNumMovesLeft()));
    std::cerr << "Time left: " << msLeft<< std::endl;
    board->doMove(myMove, mySide);
    return myMove;
}

/**
 * @brief Using the current board state, choses the next move to make
 * using a minimax algorithm!
 */
Move *Player::getMove(int msLeft) {

    std::cerr << "Time allotted: " << msLeft << std::endl;

    int minimaxDepth;
    if(testingMinimax) {
        minimaxDepth = 2;
    } else {
        minimaxDepth = 6;
    }
    // Now an instance variable, so we can pause the search inside the tree search 
    start = clock();
    clock_t elapsed = 0;
    minimaxDepth = 1;

    outOfTime = false;
    int currX = -1, currY = -1;
    while (minimaxDepth < MAX_DEPTH) {
        alphabeta(board, true, mySide, minimaxDepth, -1*(1<<29), 1<<29, msLeft);
        if(outOfTime) {
            break;
        }
        currX = bestX;
        currY = bestY;

        minimaxDepth += 1;
        elapsed = clock() - start;
        std::cerr << "Depth " << minimaxDepth << ", time elapsed: " << elapsed/CLOCKS_PER_SEC*1000 << std::endl;
    }
    if(currX == -1 && currY == -1) {
        return nullptr;
    }
    Move *returnMove = new Move(currX, currY);
    return returnMove;
}

/**
 * @brief The heuristic for the minimax tree
 */
int Player::heuristic(Board *curr, Side side) {
    if (USE_DOT_PRODUCT_HEURISTIC) {
        /*int heuristic_matrix[8][8] = {{4, -3, 2, 2, 2, 2, -3, 4},
                                {-3, -4, -1, -1, -1, -1, -4, -3},
                                {2, -1, 1, 0, 0, 1, -1, 2}, 
                                {2, -1, 0, 1, 1, 0, -1, 2},
                                {2, -1, 0, 1, 1, 0, -1, 2},
                                {2, -1, 1, 0, 0, 1, -1, 2}, 
                                {-3, -4, -1, -1, -1, -1, -4, -3},
                                {4, -3, 2, 2, 2, 2, -3, 4}};*/
        int heuristic_matrix[8][8] = {{424157,-42576,41797,33302,8324,26761,-45030,420591},
                                      {-47932,-55635,22561,-93,4857,-4899,-71650,-59483},
                                      {36078,22579,-17520,15674,11716,-18482,14945,39948},
                                      {31963,8028,31354,-9876,-15214,-6063,15413,27244},
                                      {22102,9408,13181,-16336,-13678,8721,16881,32272},
                                      {40561,19297,-1109,6537,7194,-10083,18860,64033},
                                      {-53181,-37002,12794,17028,12417,-10222,-47324,-46482},
                                      {432860,-53916,26743,37664,34003,32778,-54982,438858}};
        int score = 0;
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                if (curr->get(side, x, y)) {
                    score += heuristic_matrix[x][y];
                } else if (curr->get(flip(side), x, y)) {
                    score -= heuristic_matrix[x][y];
                }
            }
        }
        return score;
    } 
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

double Player::alphabeta(Board *curr, bool root,
        Side side, int depth, int alpha, int beta, int msLeft) {
    if(depth == 0) {
        return heuristic(curr, side);
    }

    if(curr->isDone()) {
        if(curr->count(side) > curr->count(flip(side))) {
            return 1<<29; // arbitrary large positive number
        }
        return -(1<<29); // arbitrary large negative number
    }

    // Terminating search early. Maybe don't call this all the time
    // to save time
    int elapsed = clock() - start;
    if(elapsed*1000/CLOCKS_PER_SEC > msLeft) {
        //std::cerr << "Termination time: " << elapsed << std::endl;
        outOfTime = true;
    }

    if(outOfTime) {
        return alpha;
    }
    // Check if you need to pass
    if(!curr->hasMoves(side)) {
        int score = -1*alphabeta(curr, false, flip(side), depth-1,
                -1*beta, -1*alpha, msLeft);
        if(score > alpha) {
            alpha = score; // better move
            if(root) {
                bestX = -1;
                bestY = -1;
            }
        }
        if(score >= beta) {
            return beta; // cutoff
        }
        return alpha;
    }


    bitset<64> validMoves = curr->getValidMoves(side);
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(validMoves[i + 8*j]) {
                Move move(i, j);
                Board *tmp = curr->copy();
                tmp->doMove(&move, side);
                int score = -1*alphabeta(tmp, false, flip(side), depth-1,
                        -1*beta, -1*alpha, msLeft);
                if(score > alpha) {
                    alpha = score; // better move
                    if(root) {
                        bestX = i;
                        bestY = j;
                    }
                }
                if(score >= beta) {
                    //std::cerr << "Pruned! remaining depth " << depth << std::endl;
                    return beta; // cutoff
                }
                // Very inefficient memory usage
                delete tmp;
            }
        }
    }

    return alpha;

}

/**
 * @brief For testing minimax
 */
void Player::setBoard(Board *b) {
    delete board;
    board = b;
}




