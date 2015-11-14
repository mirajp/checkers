// ECE 469: Artificial Intelligence, Prof. Sable
// Checkers - by Miraj Patel
// player.h - Defines player class (player's pieces and next available moves)

#ifndef PLAYER_H

#define PLAYER_H

#include "game.h"
#include <cstddef>
#include <vector>
#include <iostream>

//struct Move {
	//int startPiece;
	//Location end;
	//Location jumpedLocations[NUMPIECES];
	//int numJumps;
	
	//Move() { startPiece = -1; numJumps = 0;}
	
	/*
	Move & operator=(const Move &source) {
		startPiece = source.startPiece;
		end.row = source.end.row;
		end.column = source.end.column;
		numJumps = source.numJumps;
		for (int i = 0; i < NUMPIECES; i++) {
			jumpedLocations[i].row = source.jumpedLocations[i].row;
			jumpedLocations[i].column = source.jumpedLocations[i].column;
		}
	}
	*/
//};

class Player {
	public:
		unsigned int playerNumber;
		
		Player();
		int evaluateState(Game (&game));
		int AIChooseMove(Game (&game));
		int alphabeta(Game (&gameNode), int depth, int alpha, int beta);
};

#endif