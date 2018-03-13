A README for this project.

Division of labor:
Victor worked setting up the framework for the bot, and implementing the
minimax algorithm and alpha-beta pruning. He also tested some different
strategies for time use.

Tim worked on debugging	and machine learning the heuristic.

Some extra improvements we made:
-Added method to return bitboard of legal moves to speed up search.
 Should extend the amount we can search.

-Implemented iterative deepening, to make sure we do not run out of time
 when searching.

-Added some strategy for time use. Added a parameter to tune how much
 time to allot to moves throughout the course of the game. Within each
 move's alloted time, searches through increasing tree depths until
 time runs out, and disregards the tree that it is in the middle of
 searching (falls back to a completed tree). Again, this helps because
 it helps us make the most of the time we are given.

-Self-play mcts (according to alphazero paper) with linear regression model 
 to obtain heuristic. Relevant paper:	https://arxiv.org/abs/1712.01815

