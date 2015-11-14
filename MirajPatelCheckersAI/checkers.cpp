// ECE 469: Artificial Intelligence, Prof. Sable
// Checkers - by Miraj Patel
// checkers.cpp - The main program

#include "game.h"
#include "player.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>


// these colors are extern constants inside game.h
const string INVALIDSQCOLOR =	"48;5;0";
const string VALIDSQCOLOR 	=	"48;5;7";
const string PLAYER1COLOR 	=	"38;5;9";
const string PLAYER2COLOR 	=	"38;5;20";

#define	PLAYERMODE		1
#define	COMPUTERMODE	0

using namespace std;

Game checkersGame;
Player playerOne, playerTwo;

// timeLimit here is represented in milliseconds (take # of seconds from user and multiply by 1000)
double timeLimit = 10000;


void resetGame() {
	int pieceIter = 0;
	playerOne.playerNumber = 1;
	playerTwo.playerNumber = 0;
	
	while (checkersGame.pieces[1].size()) {
		checkersGame.pieces[1].pop_back();
	}
	while (checkersGame.pieces[0].size()) {
		checkersGame.pieces[0].pop_back();
	}
	
	for (int pieceIter = 0; pieceIter < NUMPIECES; pieceIter++) {
		Location p1Piece, p2Piece;
		checkersGame.pieces[0].push_back(p2Piece);
		checkersGame.pieces[1].push_back(p1Piece);
	}
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			checkersGame.board[i][j] = P2REGULAR;
			checkersGame.board[7-i][j] = P1REGULAR;
			
			checkersGame.pieces[0][pieceIter].row = i;
			checkersGame.pieces[0][pieceIter].column = j; 
			checkersGame.pieces[1][pieceIter].row = 7-i;
			checkersGame.pieces[1][pieceIter].column = j;
			pieceIter++;
		}
	}
	
	for (int j = 0; j < 4; j++) {
		checkersGame.board[3][j] = PEMPTY;
		checkersGame.board[4][j] = PEMPTY;
	}
}

bool loadGame(string filename) {
	ifstream infile;
	int data, numberP1 = 0, numberP2 = 0;
	infile.open(filename.c_str());
	if (!infile) {
		cout << "--- Unable to open file '" << filename << "' ---" << endl;
		return false;
	}
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 4; j++) {
			if (infile.eof()) {
				cout << "--- Unexpected EOF ---" << endl;
				resetGame();
				return false;
			}
			infile >> data;
			switch(data) {
				case 0:
					checkersGame.board[i][j] = PEMPTY;
					break;
				case 1:
					checkersGame.board[i][j] = P1REGULAR;
					checkersGame.pieces[1][numberP1].row = i;
					checkersGame.pieces[1][numberP1].column = j;
					numberP1++;
					break;
				case 2:
					checkersGame.board[i][j] = P2REGULAR;
					checkersGame.pieces[0][numberP2].row = i;
					checkersGame.pieces[0][numberP2].column = j;
					numberP2++;
					break;
				case 3:
					checkersGame.board[i][j] = P1KING;
					checkersGame.pieces[1][numberP1].row = i;
					checkersGame.pieces[1][numberP1].column = j;
					numberP1++;
					break;
				case 4:
					checkersGame.board[i][j] = P2KING;
					checkersGame.pieces[0][numberP2].row = i;
					checkersGame.pieces[0][numberP2].column = j;
					numberP2++;
					break;
				default:
					cout << "--- Unknown data: " << data << " read in. ---" << endl;
					break;
			}
		}
	}
	
	for (int i = numberP1; i < NUMPIECES; i++) {
		checkersGame.pieces[1].pop_back();
	}
	for (int i = numberP2; i < NUMPIECES; i++) {
		checkersGame.pieces[0].pop_back();
	}
	
	if (infile.eof()) {
		resetGame();
		return false;
	}
	else {
		// Get current turn from 2nd last line
		infile >> data;
		cout << "\tTurn goes to Player " << data << endl;
		if (data == 2) {
			checkersGame.curTurn = 0;
		}
		else {
			checkersGame.curTurn = 1;
		}
		
	}
	
	if (infile.eof() == false) {
		// Get computer's time limit from last line
		infile >> data;
		timeLimit = data * 1000;
		cout << "\tNew time limit for the computer is " << (timeLimit/1000) << " seconds" << endl;
	}
	
	infile.close();
	return true;
}


int main() {
	string inputLine;
	int playerChoice = 0;
	bool gameOver = false;
	bool stalemate = false;
	int player1Mode = PLAYERMODE;
	int player2Mode = PLAYERMODE;
	
	resetGame();
	
	cout << "Will player #1 be a computer? (Y/N):" << endl;
	getline(cin, inputLine);
	if (inputLine == "Y" || inputLine == "y" || inputLine == "Yes" || inputLine == "yes" || inputLine == "YES") {
		player1Mode = COMPUTERMODE;
	}
	cout << "Will player #2 be a computer? (Y/N):" << endl;
	getline(cin, inputLine);
	if (inputLine == "Y" || inputLine == "y" || inputLine == "Yes" || inputLine == "yes" || inputLine == "YES") {
		player2Mode = COMPUTERMODE;
	}
	
	if (player1Mode == COMPUTERMODE || player2Mode == COMPUTERMODE) {
		cout << "Enter the time limit (in seconds) for the computer/AI:" << endl;
		getline(cin, inputLine);
		playerChoice = atoi(inputLine.c_str());
		while (playerChoice <= 0) {
			cout << "Please input an integer greater than 0 for the time limit:" << endl;
			getline(cin, inputLine);
			playerChoice = atoi(inputLine.c_str());
		}
		timeLimit = (playerChoice * 1000);
	}
	
	cout << "Specify a 'sampleboard' textfile? (Press enter to skip, or enter filename)" << endl;
	getline(cin, inputLine);
	if (inputLine.length() > 0) {
		if (loadGame(inputLine)) {
			cout << "Successfully read in sample board." << endl;
		}
		else {
			cout << "File '" << inputLine << "' improperly structured. Game reset to default startup." << endl;
		}
	}
	
	while(!gameOver) {
		cout << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
		checkersGame.printBoard();
		if (checkersGame.curTurn) {
			// Player 1's turn
			checkersGame.printNextMoves();
			if (checkersGame.numMoves && player1Mode) {
				cout << "Enter the # corresponding to the move you (Player 1) would like to make:" << endl;
				getline(cin, inputLine);
				playerChoice = atoi(inputLine.c_str());
				while (playerChoice > checkersGame.numMoves || playerChoice < 1) {
					cout << "* Number (" << playerChoice << ") out of range! Please pick a number between 1 and " << checkersGame.numMoves << ":" << endl;
					getline(cin, inputLine);
					playerChoice = atoi(inputLine.c_str());
				}
				checkersGame.chooseMove(playerChoice);
			}
			else if (checkersGame.numMoves){
				if (checkersGame.numMoves > 1) {
					playerChoice = playerOne.AIChooseMove(checkersGame);
					checkersGame.chooseMove(playerChoice);
				}
				else {
					cout << "P1-Computer forced to choose move # 1" << endl;
					checkersGame.chooseMove(1);
				}
			}
			else {
				gameOver = true;
			}
		}
		else {
			// Player 2's turn
			checkersGame.printNextMoves();
			if (checkersGame.numMoves && player2Mode) {
				cout << "Enter the # corresponding to the move you (Player 2) would like to make:" << endl;
				getline(cin, inputLine);
				playerChoice = atoi(inputLine.c_str());
				while (playerChoice > checkersGame.numMoves || playerChoice < 1) {
					cout << "* Number (" << playerChoice << ") out of range! Please pick a number between 1 and " << checkersGame.numMoves << ":" << endl;
					getline(cin, inputLine);
					playerChoice = atoi(inputLine.c_str());
				}
				checkersGame.chooseMove(playerChoice);
			}
			else if (checkersGame.numMoves){
				if (checkersGame.numMoves > 1) {				
					playerChoice = playerTwo.AIChooseMove(checkersGame);
					checkersGame.chooseMove(playerChoice);
				}
				else {
					cout << "P2-Computer forced to choose move # 1" << endl;
					checkersGame.chooseMove(1);
				}
			}
			else {
				gameOver = true;
			}
		}
		
		if (checkersGame.numNonJumpMoves == MOVESTODRAW) {
			stalemate = true;
			gameOver = true;
		}
	}
	
	cout << "Game over!";
	if (stalemate) {
		cout << " Since no piece has been captured in the last " << MOVESTODRAW << " moves, this game ends in a draw." << endl;
	}
	else {
		if (checkersGame.curTurn == 0) {
			// Current turn being player 2 means player 2 ran out of moves = player 1 wins
			cout << " The winner is: Player 1." << endl;
		}
		else {
			cout << " The winner is: Player 2." << endl;
		}
	}
	
	return 0;
}
