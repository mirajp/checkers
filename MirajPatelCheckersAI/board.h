// ECE 469: Artificial Intelligence, Prof. Sable
// Checkers - by Miraj Patel
// board.h - Setuping and displaying the board

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include "game.h"

using namespace std;

class Board {
	private:
		char p1Regular, p2Regular, p1King, p2King;
        string invalidSqColor, validSqColor, player1Color, player2Color;
		char *colorPieces (char piece);
		Game *game;
	public:
		//Board(char p1Regularpiece, char p1Kingpiece, char p2Regularpiece, char p2Kingpiece);
		Board(char, char, char, char);
		
		void getMoves(vector<string> &moves, int playerTurn);
		
		int movePiece(int playerNum, int from, int to);
		
		void printBoard();
};


#endif
