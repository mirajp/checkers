// ECE 469: Artificial Intelligence, Prof. Sable
// Checkers - by Miraj Patel
// game.h - Setuping the game itself and internal functions to play the game (with an 8x4 array representing the 8x8 field)

#ifndef GAME_H
#define GAME_H

#include <string>

using namespace std;

class Game {
	private:
		char p1Regular, p1King, p2Regular, p2King, pEmpty, gameSpace[8][4];
	public:
		Game(char, char, char, char);
		
		//gameSpace[row, column]
		char getPiece(int row, int column);
};

#endif