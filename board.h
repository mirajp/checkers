// ECE 469: Artificial Intelligence, Prof. Sable
// Checkers - by Miraj Patel
// board.h - Setuping and displaying the board

#ifndef BOARD_H
#define BOARD_H

#include <string>
#include "game.h"

using namespace std;

class Board {
	private:
		string invalidSqColor, validSqColor, player1Color, player2Color;
		char p1Regular, p1King, p2Regular, p2King, pEmpty, board[8][4];
		char *colorPieces (char piece);
		Game *game;
	public:
		//Board(char p1Regularpiece, char p1Kingpiece, char p2Regularpiece, char p2Kingpiece);
		Board(char, char, char, char);
		
		// These setters take in RGB
		void set_invalidSqColorRGB (string newColor) {
			invalidSqColor = "48;5;" + newColor;
		}
		void set_validSqColorRGB (string newColor) {
			validSqColor = "48;5;" + newColor;
		}
		void set_p1ColorRGB (string newColor) {
			player1Color = "38;5;" + newColor;
		}
		void set_p2ColorRGB (string newColor) {
			player2Color = "38;5;" + newColor;
		}
		
		// These setters take in raw CSI color code: 100+i = bright (high intensity);  40+i = default (normal intensity)
		// I: 0 = black, 1 = red, 2 = green, 3 = yellow, 4 = blue, 5 = magenta, 6 = cyan, 7 = white
		void set_invalidSqColorCSI (string newColor) {
			invalidSqColor = newColor;
		}
		void set_validSqColorCSI (string newColor) {
			validSqColor = newColor;
		}
		// These setters take in raw CSI color code: 90+i = bright (high intensity);  30+i = default (normal intensity)
		void set_p1ColorCSI (string newColor) {
			player1Color = newColor;
		}
		void set_p2ColorCSI (string newColor) {
			player2Color = newColor;
		}
		
		int movePiece(int playerNum, int from, int to);
		
		void printBoard();
};


#endif