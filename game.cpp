// ECE 469: Artificial Intelligence, Prof. Sable
// Checkers - by Miraj Patel
// game.cpp - Setuping the game itself and internal functions to play the game (with an 8x4 array representing the 8x8 field)

#include "game.h"

#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

// The constructor makes a new game
// The constructor sets the visuals of the board (gameSpace) and makes a new game
Game::Game(char p1Regularpiece, char p1Kingpiece, char p2Regularpiece, char p2Kingpiece) {
	p1Regular = p1Regularpiece;
	p1King = p1Kingpiece;
	p2Regular = p2Regularpiece;
	p2King = p2Kingpiece;
	pEmpty = ' ';
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			gameSpace[i][j] = p2Regular;
			gameSpace[7-i][j] = p1Regular;
		}
	}
	
	for (int j = 0; j < 4; j++) {
		gameSpace[3][j] = pEmpty;
		gameSpace[4][j] = pEmpty;
	}
	
}

char Game::getPiece(int row, int column) {
	return gameSpace[row][column];
}