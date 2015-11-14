// ECE 469: Artificial Intelligence, Prof. Sable
// Checkers - by Miraj Patel
// board.cpp - Setuping and displaying the board

#include "board.h"
#include <vector>
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

// The constructor sets the visuals of the board and makes a new game
Board::Board(char p1Regularpiece, char p1Kingpiece, char p2Regularpiece, char p2Kingpiece) {
	p1Regular = p1Regularpiece;
	p1King = p1Kingpiece;
	p2Regular = p2Regularpiece;
	p2King = p2Kingpiece;
	
	game = new Game(p1Regular, p1King, p2Regular, p2King);
	
	invalidSqColor = "48;5;0";
	validSqColor = "48;5;7";
	player1Color = "38;5;9";
	player2Color = "38;5;20";
}

// https://en.wikipedia.org/wiki/ANSI_escape_code#graphics
char *Board::colorPieces(char piece) {
	char *buf = (char *) malloc(32);
	
	if (piece == p1Regular || piece == p1King) {
		// Blue = 4
		sprintf(buf, "\033[%s;1;%sm %c \033[0m", validSqColor.c_str(), player1Color.c_str(), piece);
		//sprintf(buf, "\033[%s;38;5;1;45m %c \033[0m", validSqColor.c_str(), piece);
	}
	else if (piece == p2Regular || piece == p2King) {
		// Red = 1
		sprintf(buf, "\033[%s;1;%sm %c \033[0m", validSqColor.c_str(), player2Color.c_str(), piece);
	}
	else {
		sprintf(buf, "\033[%s;1;92m %c \033[0m", validSqColor.c_str(), piece);
	}
	
	return buf;
}

// Takes in playerTurn as 0->P2, 1->P1
void Board::getMoves(vector<string> &moves, int playerTurn) {
	if (playerTurn)
		game->getMovesP1(moves);
	else 
		game->getMovesP2(moves);
}

int movePiece(int playerNum, int from, int to) {
	cout << "Player " << playerNum << " moving " << from << " to " << to << endl;
	return 0;
}


void Board::printBoard() {
	std::cout << "   \e(0\x6C\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6B\e(B" << std::endl;
	for (int i = 0; i < 8; i++) {
		// Label the rows
		std::cout << " \033[1;92m" << (8-i) << "\033[0m ";
		
		// This makes A8, A6, A4, and A2 corner invalid squares
		if (i % 2 == 0) {
			std::cout << ("\e(0\x78\e(B\033[" + invalidSqColor + ";1;92m   \033[0m");
		}
		
		for (int j = 0; j < 3; j++) {
			std::cout << "\e(0\x78\e(B" << colorPieces(game->getPiece(i, j)) << "\e(0\x78\e(B" << ("\033[" + invalidSqColor + ";1;92m   \033[0m");
		}
		std::cout << "\e(0\x78\e(B" << colorPieces(game->getPiece(i, 3)) << "\e(0\x78\e(B";
		
		// This makes H7, H5, H3, and H1 corner invalid squares -- companion to making A8, A6, A4, A2 invalid
		if (i % 2 == 1) {
			std::cout << ("\033[" + invalidSqColor + ";1;92m   \033[0m\e(0\x78\e(B");
		}
		std::cout << std::endl;
		
		if (i==7)
			std::cout << "   \e(0\x6D\e(B";
		else
			std::cout << "   \e(0\x74\e(B";
		if (i < 7)
			std::cout << "\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B";
		else
			std::cout << "\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B";
		
		if (i==7)
			std::cout << "\e(0\x6A\e(B";
		else
			std::cout << "\e(0\x75\e(B";
		std::cout << std::endl;
	}
	
	// Label the columns
	std::cout << "\033[1;92m     A   B   C   D   E   F   G   H  \033[0m\n" << endl;
}
