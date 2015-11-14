// ECE 469: Artificial Intelligence, Prof. Sable
// Checkers - by Miraj Patel
// player.cpp - Defines player class (player's pieces and next available moves)

#include "player.h"
#include <cstdlib>
#include <cstdio>
#include <memory.h>
#include <iostream>
#include <sstream>
#include <sys/time.h>
#include <unistd.h>

#define	NEGINFINITY -1000000000
#define	INFINITY	1000000000
#define MAXDEPTH	20

using namespace std;

bool outOfTime;
int bestMove, maxDepthReached;
struct timeval startTime, curTime;
double millisecStart, millisecNow;

extern double timeLimit;

Player::Player() {
	playerNumber = 0;
}

// get the number of project moves it would take to reach the end location
int getDistance(Location (&one), Location (&two)) {
	int distance = 0;
	int startRow = one.row, endRow = two.row, startColumn = one.column, endColumn = two.column;
	while (startRow != endRow && endColumn != startColumn) {
		if (startRow % 2 == 0) {
			if (startRow == endRow) {
				startRow++;
				
				if (endColumn > startColumn) {
					startColumn++;
				}
				distance++;
			}
			else {
				if (startRow < endRow) {
					startRow++;
				}
				else {
					startRow--;
				}
				if (endColumn > startColumn) {
					startColumn++;
				}
				distance++;
			}
		}
		else {
			if (startRow == endRow) {
				if (startRow < 7) {
					startRow++;
				}
				else {
					startRow--;
				}
				if (endColumn < startColumn) {
					startColumn--;
				}
				distance++;
			}
			else {
				if (startRow < 7) {
					startRow++;
				}
				else {
					startRow--;
				}
				if (endColumn < startColumn) {
					startColumn--;
				}
				distance++;	
			}
		}
	}
	return distance;
}

int Player::evaluateState(Game (&game)) {
	int score;
	int piecesValue = 0, positionValue = 0, differenceValue = 0, advantageValue = 0;
    int playerAdvantage = -1;
	int pieceRow, pieceColumn;
	int p1AdvDistance = 0, p2AdvDistance = 0;
	
	if (game.pieces[1].size() > game.pieces[0].size())
		playerAdvantage = 1;
	else if (game.pieces[1].size() < game.pieces[0].size())
		playerAdvantage = 0;
	
	for (int p1Iter = 0; p1Iter < game.pieces[1].size(); p1Iter++) {
		pieceRow = game.pieces[1][p1Iter].row;
		pieceColumn = game.pieces[1][p1Iter].column;
		if (game.board[pieceRow][pieceColumn] > 2) {
			// Kings
			piecesValue += 5;
			
			// If behind in pieces and about to lose, retreat to double corners
			if ((playerAdvantage == 0) && (game.pieces[1].size() < 3)) {
				if ((pieceRow > 5 && pieceColumn == 3) || (pieceRow < 2 && pieceColumn == 0)) {
					advantageValue += 9;
				}
			}
			// Else: If player 1 has advantage, smaller bonus for being near the double corner (closer the better)
			else if ((playerAdvantage == 1) && (game.pieces[0].size() < 3)) {
				// If Player 2 occupying top left corner
				if (((game.board[0][0] > 0) && (game.board[0][0] % 2 == 0)) || ((game.board[1][0] > 0) && (game.board[1][0] % 2 == 0))) {
					
					// Right on the corner
					if ((pieceRow < 2) && (pieceColumn == 0)) {
						advantageValue += 7;
					}
					// One move away from the corner
					if ((pieceRow == 2 && pieceColumn == 0) || (pieceRow == 1 && pieceColumn == 1)) {
						advantageValue += 5;
					}
					// Two moves away from the corner
					else if ((pieceRow == 3 && pieceColumn == 0) || (pieceRow == 3 && pieceColumn == 1)) {
						advantageValue += 3;
					}
					else if ((pieceRow == 2 && pieceColumn == 1) || (pieceRow == 0 && pieceColumn == 1)) {
						advantageValue += 3;
					}
					
				}
				// If Player 2 occupying bottom right corner
				else if (((game.board[6][3] > 0) && (game.board[6][3] % 2 == 0)) || ((game.board[7][3] > 0) && (game.board[7][3] % 2 == 0))) {
					
					// Right on the corner
					if ((pieceRow > 5) && (pieceColumn == 3)) {
						advantageValue += 7;
					}
					// One move away from the corner
					if ((pieceRow == 6 && pieceColumn == 2) || (pieceRow == 5 && pieceColumn == 3)) {
						advantageValue += 5;
					}
					// Two moves away from the corner
					else if ((pieceRow == 4 && pieceColumn == 3) || (pieceRow == 4 && pieceColumn == 2)) {
						advantageValue += 3;
					}
					else if ((pieceRow == 5 && pieceColumn == 2) || (pieceRow == 7 && pieceColumn == 2)) {
						advantageValue += 3;
					}
				}
				
				
			}
		}
		else {
			// Regular pieces
			piecesValue += 3;
			
			// Staying in back row to prevent opponent kinging
			if (pieceRow == 7) {
				positionValue += 9;
			}
			else {
				// Closer to being king, row = 0, the better
				positionValue += (7 - pieceRow);
			}
		}
	}
	
	for (int p2Iter = 0; p2Iter < game.pieces[0].size(); p2Iter++) {
		pieceRow = game.pieces[0][p2Iter].row;
		pieceColumn = game.pieces[0][p2Iter].column;
		if (game.board[pieceRow][pieceColumn] > 2) {
			// Kings
			piecesValue -= 5;
			
			// If behind in pieces and about to lose, retreat to double corners
			if ((playerAdvantage == 1) && (game.pieces[0].size() < 3)) {
				if ((pieceRow > 5 && pieceColumn == 3) || (pieceRow < 2 && pieceColumn == 0)) {
					advantageValue -= 9;
				}
			}
			// Else: If player 2 has advantage, smaller bonus for being near the double corner (closer the better)
			else if ((playerAdvantage == 0) && (game.pieces[1].size() < 3)) {
				// If Player 1 occupying top left corner
				if (((game.board[0][0] > 0) && (game.board[0][0] % 2 == 1)) || ((game.board[1][0] > 0) && (game.board[1][0] % 2 == 1))) {
					
					// Right on the corner
					if ((pieceRow < 2) && (pieceColumn == 0)) {
						advantageValue -= 7;
					}
					// One move away from the corner
					if ((pieceRow == 2 && pieceColumn == 0) || (pieceRow == 1 && pieceColumn == 1)) {
						advantageValue -= 5;
					}
					// Two moves away from the corner
					else if ((pieceRow == 3 && pieceColumn == 0) || (pieceRow == 3 && pieceColumn == 1)) {
						advantageValue -= 3;
					}
					else if ((pieceRow == 2 && pieceColumn == 1) || (pieceRow == 0 && pieceColumn == 1)) {
						advantageValue -= 3;
					}
					
				}
				// If Player 1 occupying bottom right corner
				else if (((game.board[6][3] > 0) && (game.board[6][3] % 2 == 1)) || ((game.board[7][3] > 0) && (game.board[7][3] % 2 == 1))) {
					
					// Right on the corner
					if ((pieceRow > 5) && (pieceColumn == 3)) {
						advantageValue -= 7;
					}
					// One move away from the corner
					if ((pieceRow == 6 && pieceColumn == 2) || (pieceRow == 5 && pieceColumn == 3)) {
						advantageValue -= 5;
					}
					// Two moves away from the corner
					else if ((pieceRow == 4 && pieceColumn == 3) || (pieceRow == 4 && pieceColumn == 2)) {
						advantageValue -= 3;
					}
					else if ((pieceRow == 5 && pieceColumn == 2) || (pieceRow == 7 && pieceColumn == 2)) {
						advantageValue -= 3;
					}
				}
			}
		}
		else {
			// Regular pieces
			piecesValue -= 3;
			
			// Staying in back row to prevent opponent kinging
			if (pieceRow == 0) {
				positionValue -= 9;
			}
			else {
				// Closer to being king, row = 7, the better
				positionValue -= pieceRow;
			}
		}
	}
	
	piecesValue = piecesValue * 1000000;
	positionValue = positionValue * 100000;
	// Difference in number of pieces
	differenceValue = (game.pieces[1].size() - game.pieces[0].size()) * 1000;
	advantageValue = advantageValue * 10;
	score = piecesValue + positionValue + differenceValue + advantageValue;
	return score;
}

void cloneGame(Game (&destGame), Game (&srcGame), int moveNum) {
	destGame.curTurn = srcGame.curTurn;
	
	for (int rowIter = 0; rowIter < 8; rowIter++) {
		for (int columnIter = 0; columnIter < 4; columnIter++) {
			destGame.board[rowIter][columnIter] = srcGame.board[rowIter][columnIter];
		}
	}
	
	for (int p2PieceIter = 0; p2PieceIter < srcGame.pieces[0].size(); p2PieceIter++) {
		Location p2Piece;
		p2Piece.row = srcGame.pieces[0][p2PieceIter].row;
		p2Piece.column = srcGame.pieces[0][p2PieceIter].column;
		destGame.pieces[0].push_back(p2Piece);
	}
	
	for (int p1PieceIter = 0; p1PieceIter < srcGame.pieces[1].size(); p1PieceIter++) {
		Location p1Piece;
		p1Piece.row = srcGame.pieces[1][p1PieceIter].row;
		p1Piece.column = srcGame.pieces[1][p1PieceIter].column;
		destGame.pieces[1].push_back(p1Piece);
	}
	
	destGame.movesAvailable[0].startPiece = srcGame.movesAvailable[moveNum-1].startPiece;
	destGame.movesAvailable[0].end.row = srcGame.movesAvailable[moveNum-1].end.row;
	destGame.movesAvailable[0].end.column = srcGame.movesAvailable[moveNum-1].end.column;
	destGame.movesAvailable[0].numJumps = srcGame.movesAvailable[moveNum-1].numJumps;
	
	for (int jumpIter = 0; jumpIter < srcGame.movesAvailable[moveNum-1].numJumps; jumpIter++) {
		destGame.movesAvailable[0].jumpedLocations[jumpIter].row = srcGame.movesAvailable[moveNum-1].jumpedLocations[jumpIter].row;
		destGame.movesAvailable[0].jumpedLocations[jumpIter].column = srcGame.movesAvailable[moveNum-1].jumpedLocations[jumpIter].column;
	}
	
}

bool terminalState(Game (&game)) {
	if ((game.numMoves == 0) || (game.numNonJumpMoves == MOVESTODRAW)) {
		return true;
	}
	return false;
}

int utility(Game (&game), int depth) {
	if (game.numNonJumpMoves == MOVESTODRAW)
		return 0;
	if (game.curTurn) {
		// Player 1's turn and has 0 moves available, higher depth = closer to winning
		return (-100000000*(depth+1));
	}
	else {
		// Player 2's turn and has 0 moves available
		return (100000000*(depth+1));
	}
}

int Player::alphabeta(Game (&gameNode), int depth, int alpha, int beta) {	
	if (!outOfTime) {
		gettimeofday(&curTime, NULL);
		millisecNow = (curTime.tv_sec*1000) + (curTime.tv_usec/1000);
		
		if (((millisecNow - millisecStart)/timeLimit) > 0.75) {
			outOfTime = true;
			return 0;
		}
		
		gameNode.getNextMoves();
		
		// Check for terminal state: if either player has 0 moves = someone wins, or if tie, and return that utility score
		if (terminalState(gameNode)) {
			return (utility(gameNode, depth));
		}
		if (depth == 0) {
			//cout << "\t\tReturning " << this->evaluateState(gameNode) << endl;
			return (this->evaluateState(gameNode));
		}
		
		if (gameNode.curTurn) {
			int bestValue = alpha;
			
			for (int child = 1; child <= gameNode.numMoves; child++) {
				Game childGame;
				cloneGame(childGame, gameNode, child);
				childGame.chooseMove(1);
				int ret = alphabeta(childGame, (depth-1), bestValue, beta);
				if (ret > bestValue) {
					bestValue = ret;
					if (depth == maxDepthReached) {
						bestMove = child;
					}
				}
				else if ((ret == bestValue) && (depth == maxDepthReached)) {
					if (rand() % 2) {
						bestMove = child;
					}
				}
				if (beta <= bestValue) {
					// Beta cut-off = prune remaining branches
					break;
				}
			}
			return bestValue;
		}
		else {
			int bestValue = beta;
			
			for (int child = 1; child <= gameNode.numMoves; child++) {
				//cout << "Depth = " << depth << ", child = " << child << " of " << gameNode.numMoves << ", alpha = " << alpha << ", beta = " << beta << endl;
				Game childGame;
				cloneGame(childGame, gameNode, child);
				childGame.chooseMove(1);
				
				int ret = alphabeta(childGame, (depth-1), alpha, bestValue);
				if (ret < bestValue) {
					bestValue = ret;
					if (depth == maxDepthReached) {
						bestMove = child;
					}
				}
				else if ((ret == bestValue) && (depth == maxDepthReached)) {
					if (rand() % 2) {
						bestMove = child;
					}
				}
				if (bestValue <= alpha) {
					// Alpha cut-off = prune remaining branches
					break;
				}
			}
			return bestValue;
		}
	}
	else {
		return 0;
	}
}

int Player::AIChooseMove(Game (&game)) {
	// Assumes the 1st layer of moves have been given to it already
	gettimeofday(&startTime, NULL);
	millisecStart = (startTime.tv_sec*1000) + (startTime.tv_usec/1000);
	bestMove = -1;
	srand(time(NULL));
	
	int bestValue, completedBestMove;
	int startingDepth = 5;
	
	if (timeLimit > 4000)
		startingDepth = 7;
	
	if (game.pieces[0].size() + game.pieces[1].size() <= 10) {
		startingDepth += 1;
	}
	if (game.pieces[0].size() + game.pieces[1].size() <= 5) {
		startingDepth += 1;
	}
	
	outOfTime = false;
	
	for (int depth = startingDepth; depth < MAXDEPTH; depth++) {
	//for (int depth = 2; depth < 3; depth++) {
		maxDepthReached = depth;
		bestValue = alphabeta(game, depth, NEGINFINITY, INFINITY);
		
		if (!outOfTime) {
			completedBestMove = bestMove;
		}
        else {
            maxDepthReached--;
        }
		
		gettimeofday(&curTime, NULL);
		millisecNow = (curTime.tv_sec*1000) + (curTime.tv_usec/1000);
		
		if (outOfTime || (((millisecNow - millisecStart)/timeLimit) > 0.5)) {
			//cout << "Diff in milliseconds = " << (millisecNow - millisecStart) << endl;
			break;
		}
	}
	
	int millisecDiff = millisecNow - millisecStart;
	cout << "After " << (millisecDiff/1000) << " seconds: reached depth " << maxDepthReached << "." << endl;
    if (outOfTime)
        cout << "Ran out of time searching depth " << (maxDepthReached+1) << "." << endl;
	cout << "AI chooses move #" << completedBestMove << "." << endl;
	
	game.getNextMoves();
	return completedBestMove;
}
