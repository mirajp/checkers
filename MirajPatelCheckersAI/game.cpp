// ECE 469: Artificial Intelligence, Prof. Sable
// Checkers - by Miraj Patel
// game.cpp - Setuping the game itself and internal functions to play the game (with an 8x4 array representing the 8x8 field)

#include "game.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <memory.h>

using namespace std;

// The constructor makes a new game and two players
Game::Game() {
	// Turn = 1 means Player 1's turn, Turn = 0 means Player 2's turn
	curTurn = 1;
	numNonJumpMoves = 0;
}

string Game::nameSquare(int row, int column) {
	string name = "";
	stringstream ss;
	ss << ((row*-1) + 8);
	string rowStr = ss.str();
	
	if (row % 2 == 0) {
		switch(column) {
			case 0:
				name = "B";
				break;
			case 1:
				name = "D";
				break;
			case 2:
				name = "F";
				break;
			case 3:
				name = "H";
				break;
			default:
				name = "UNKNOWN COLUMN";
				break;
		}
	}
	else {
		switch(column) {
			case 0:
				name = "A";
				break;
			case 1:
				name = "C";
				break;
			case 2:
				name = "E";
				break;
			case 3:
				name = "G";
				break;
			default:
				name = "UNKNOWN COLUMN";
				break;
		}
	}
	
	name = name + rowStr;
	return name;
}


void Game::addMove(Move &newMove, char type) {
	if (type == JUMPMOVE && (this->onlyShowJumps == false)) {
		this->onlyShowJumps = true;
		this->numMoves = 0;
	}
	this->movesAvailable[this->numMoves] = newMove;
	this->numMoves++;
}

// Remove's the opposite player's piece given a location
bool Game::takePiece(Location &captured) {
	for (int pieceIter = 0; pieceIter < this->pieces[((this->curTurn + 1) % 2)].size(); pieceIter++) {
		if (this->pieces[((this->curTurn + 1) % 2)][pieceIter].row == captured.row && this->pieces[((this->curTurn + 1) % 2)][pieceIter].column == captured.column) {
			this->pieces[((this->curTurn + 1) % 2)].erase(this->pieces[((this->curTurn + 1) % 2)].begin()+pieceIter);
			return true;
		}
	}
	return false;
}

// Only takes distance values of 0 and 1
void Game::setEndLocation(Location *start, Location &end, int direction, int distance) {
	if (distance > 2 || distance < 1)
		return;
	
	switch(direction) {
		case NE:
			if (start->row > distance-1) {
				end.row = start->row - distance;
				if ((start->row % 2 == 1) && (distance == 1)) {
					//cout << "Applied NE direction" << endl;
					end.column = start->column;
				}
				else if (start->column < 3) {
					//cout << "Applied NE direction" << endl;
					end.column = start->column + 1;
				}
			}
			return;
		case NW:
			if (start->row > distance-1) {
				end.row = start->row - distance;
				if ((start->row % 2 == 0) && (distance == 1)) {
					//cout << "Applied NW direction" << endl;
					end.column = start->column;
				}
				else if (start->column > 0) {
					//cout << "Applied NW direction" << endl;
					end.column = start->column - 1;
				}
			}
			return;
		case SE:
			if (start->row + distance < 8) {
				end.row = start->row + distance;
				if ((start->row % 2 == 1) && (distance == 1)) {
					//cout << "Applied SE direction" << endl;
					end.column = start->column;
				}
				else if (start->column < 3) {
					//cout << "Applied SE direction" << endl;
					end.column = start->column + 1;
				}
			}
			return;
		case SW:
			if (start->row + distance < 8) {
				end.row = start->row + distance;
				if ((start->row % 2 == 0) && (distance == 1)) {
					//cout << "Applied SW direction" << endl;
					end.column = start->column;
				}
				else if (start->column > 0) {
					//cout << "Applied SW direction" << endl;
					end.column = start->column - 1;
				}
			}
			return;
		default:
			//cout << "Unknown direction" << endl;
			return;
	}
}

bool Game::getJumps(Move &firstJump, int (&directions)[4], int numDirections) {
	bool foundAnotherJump = false;
	for (int i = 0; i < numDirections; i++) {
		Move newMove = firstJump;
		Location startPosition = newMove.end;
		Location capturePosition, endPosition;
		
		setEndLocation(&startPosition, capturePosition, directions[i], 1);
		// If valid position found distance 1 away
		if (capturePosition.column >= 0) {
			// If the position is occupied by opposing player's piece
			if (this->board[capturePosition.row][capturePosition.column] % 2 != this->curTurn && (this->board[capturePosition.row][capturePosition.column] > PEMPTY)) {
				// Make sure this piece was not already counted in this jump sequence/chain
				bool pieceInChain = false;
				for (int j = 0; j < newMove.numJumps; j++) {
					if ((newMove.jumpedLocations[j].row == capturePosition.row) && (newMove.jumpedLocations[j].column == capturePosition.column)) {
						pieceInChain = true;
						break;
					}						
				}
				if (pieceInChain == false) {
					setEndLocation(&startPosition, endPosition, directions[i], 2);
					// If the end position is a valid empty square
					if (((endPosition.column != -1) && (this->board[endPosition.row][endPosition.column] == PEMPTY)) || (((this->pieces[this->curTurn][newMove.startPiece]).row == endPosition.row) && ((this->pieces[this->curTurn][newMove.startPiece]).column == endPosition.column))) {
						foundAnotherJump = true;
						newMove.end = endPosition;
						newMove.jumpedLocations[newMove.numJumps] = capturePosition;
						newMove.numJumps++;
						
						if (getJumps(newMove, directions, numDirections) == false) {
							//cout << "Adding move because no additional jumps available." << endl;
							addMove(newMove, JUMPMOVE);
						}
						/*
						else {
							//cout << "Extended move (multi jump) found - no need to add this partial jump." << endl;
						}
						*/
					}
				}
				/*
				else {
					//cout << "\t\tThis piece (" << nameSquare(capturePosition.row, capturePosition.column) << ") was indeed already captured." << endl;
				}
				*/
			}
		}
		/*
		else {
			//cout << "Nothing to capture" << endl;
		}
		*/
	}
	return foundAnotherJump;
}

void Game::getNonJumps(Move &firstJump, int (&directions)[4], int numDirections) {
	for (int i = 0; i < numDirections; i++) {
		Move newMove = firstJump;
		Location *startPosition, endPosition;
		startPosition = &(this->pieces[this->curTurn][newMove.startPiece]);
		
		setEndLocation(startPosition, endPosition, directions[i], 1);
		// If valid position found distance 1 away
		if (endPosition.column >= 0) {
			if ((this->board[endPosition.row][endPosition.column] == PEMPTY) && (onlyShowJumps == false)){
				newMove.end = endPosition;
				addMove(newMove, NONJUMPMOVE);
			}
		}
	}
}

void Game::getNextMoves() {
	this->numMoves = 0;
	this->onlyShowJumps = false;
	int numDirections = 0;
	int directions[4] = {NE,NW,SE,SW};
	Location startPosition;
	
	//cout << "\tThis player(" << this->curTurn << ") has " << this->pieces[this->curTurn].size() << " pieces..." << endl;
	for (int pieceIter = 0; pieceIter < this->pieces[this->curTurn].size(); pieceIter++) {
		startPosition = this->pieces[this->curTurn][pieceIter];
		
		// If piece's value is greater than 2, it's a king, else it's a regular piece
		if (this->board[startPosition.row][startPosition.column] > 2) {
			//cout << "This piece is a king at [" << startPosition.row << "," << startPosition.column << "]" << endl;
			directions[0] = NE;
			directions[1] = NW;
			directions[2] = SE;
			directions[3] = SW;
			numDirections = 4;
		}
		else if (this->curTurn == 0) {
			// If Player 2, then only directions are SE and SW
			//cout << "This piece is a P2regular at [" << startPosition.row << "," << startPosition.column << "]" << endl;
			directions[0] = SE;
			directions[1] = SW;
			numDirections = 2;
		}
		else {
			// Else for Player 1, the directions are NE and NW
			//cout << "This piece is a P1regular at [" << startPosition.row << "," << startPosition.column << "]" << endl;
			directions[0] = NE;
			directions[1] = NW;
			numDirections = 2;
		}
		
		Move firstJump;
		firstJump.startPiece = pieceIter;
		firstJump.end = startPosition;
		
		if ((getJumps(firstJump, directions, numDirections) == false) && (this->onlyShowJumps == false)) {
			getNonJumps(firstJump, directions, numDirections);
		}
	}
	
}

void Game::chooseMove(int choice) {
	Move move = this->movesAvailable[choice-1];
	char originalPiece = this->board[(this->pieces[this->curTurn][move.startPiece]).row][(this->pieces[this->curTurn][move.startPiece]).column];
	
    for (int jumpIter = 0; jumpIter < move.numJumps; jumpIter++) {
        if (takePiece(move.jumpedLocations[jumpIter]) == false) {
			cout << "Error: could not find opponent's captured piece at [" << move.jumpedLocations[jumpIter].row << ", " << move.jumpedLocations[jumpIter].column << "]" << endl;
		}
		else {
			this->board[(move.jumpedLocations[jumpIter]).row][(move.jumpedLocations[jumpIter]).column] = PEMPTY;
		}
    }
	// If non-jump move and non-king piece being moved (this is a counter that measures closeness to declaring a tie)
	if (move.numJumps == 0 && originalPiece > 2) {
		numNonJumpMoves++;
	}
	else {
		numNonJumpMoves = 0;
	}
	
	this->board[(this->pieces[this->curTurn][move.startPiece]).row][(this->pieces[this->curTurn][move.startPiece]).column] = PEMPTY;
	if (move.end.row == 0 && originalPiece == P1REGULAR) {
		// Promote to king if p1's regular piece makes to the top of the board
		this->board[move.end.row][move.end.column] = P1KING;
	}
	else if (move.end.row == 7 && originalPiece == P2REGULAR) {
		// Promote to king if p2's regular piece makes to the bottom of the board
		this->board[move.end.row][move.end.column] = P2KING;
	}
	else {
		this->board[move.end.row][move.end.column] = originalPiece;
	}
	
	this->pieces[this->curTurn][move.startPiece].row = move.end.row;
	this->pieces[this->curTurn][move.startPiece].column = move.end.column;
	
	if (this->curTurn) {
		this->curTurn = 0;
	}
	else {
		this->curTurn = 1;
	}
}

void Game::printNextMoves() {
	getNextMoves();
	
	if (this->curTurn) {
		cout << "Player 1: ";
	}
	else {
		cout << "Player 2: ";
	}
	
	cout << "Total of " << this->numMoves << " moves available." << endl;
	for (int i = 0; i < this->numMoves; i++) {
		cout << (i+1) << ". " << nameSquare(this->pieces[this->curTurn][movesAvailable[i].startPiece].row, this->pieces[this->curTurn][movesAvailable[i].startPiece].column) << " => ";
		cout << nameSquare(movesAvailable[i].end.row, movesAvailable[i].end.column);
		if (this->movesAvailable[i].numJumps > 0) {
			cout << "\t" << movesAvailable[i].numJumps << " jump(s): ";
			
			for (int j = 0; j < movesAvailable[i].numJumps; j++) {
				cout << nameSquare(movesAvailable[i].jumpedLocations[j].row, movesAvailable[i].jumpedLocations[j].column) << "  ";
			}
		}
		cout << endl;
	}
}

char *Game::colorPieces(char piece) {
	char *buf = (char *) malloc(30);
	
	if (piece == P1REGULAR) {
		sprintf(buf, "\033[%s;1;%sm %c \033[0m", VALIDSQCOLOR.c_str(), PLAYER1COLOR.c_str(), P1RegularPiece);
	}
	else if (piece == P1KING) {
		sprintf(buf, "\033[%s;1;%sm %c \033[0m", VALIDSQCOLOR.c_str(), PLAYER1COLOR.c_str(), P1KingPiece);
	}
	else if (piece == P2REGULAR) {
		sprintf(buf, "\033[%s;1;%sm %c \033[0m", VALIDSQCOLOR.c_str(), PLAYER2COLOR.c_str(), P2RegularPiece);
	}
	else if (piece == P2KING) {
		sprintf(buf, "\033[%s;1;%sm %c \033[0m", VALIDSQCOLOR.c_str(), PLAYER2COLOR.c_str(), P2KingPiece);
	}
	else {
		sprintf(buf, "\033[%s;1;92m %c \033[0m", VALIDSQCOLOR.c_str(), EmptyPiece);
	}
	
	return buf;
}

void Game::printBoard() {
	cout << "   \e(0\x6C\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6B\e(B" << endl;
	for (int i = 0; i < 8; i++) {
		// Label the rows
		cout << " \033[1;92m" << (8-i) << "\033[0m ";
		
		// This makes A8, A6, A4, and A2 corner invalid squares
		if (i % 2 == 0) {
			cout << ("\e(0\x78\e(B\033[" + INVALIDSQCOLOR + ";1;92m   \033[0m");
		}
		
		for (int j = 0; j < 3; j++) {
			char *buf = colorPieces(board[i][j]);
			cout << "\e(0\x78\e(B" << buf << "\e(0\x78\e(B" << ("\033[" + INVALIDSQCOLOR + ";1;92m   \033[0m");
			free(buf);
		}
		
		char *buf = colorPieces(board[i][3]);
		cout << "\e(0\x78\e(B" << buf << "\e(0\x78\e(B";
		free (buf);
		
		// This makes H7, H5, H3, and H1 corner invalid squares -- companion to making A8, A6, A4, A2 invalid
		if (i % 2 == 1) {
			cout << ("\033[" + INVALIDSQCOLOR + ";1;92m   \033[0m\e(0\x78\e(B");
		}
		cout << endl;
		
		if (i==7)
			cout << "   \e(0\x6D\e(B";
		else
			cout << "   \e(0\x74\e(B";
		if (i < 7)
			cout << "\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B";
		else
			cout << "\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B";
		
		if (i==7)
			cout << "\e(0\x6A\e(B";
		else
			cout << "\e(0\x75\e(B";
		cout << endl;
	}
	
	// Label the columns
	cout << "\033[1;92m     A   B   C   D   E   F   G   H  \033[0m\n" << endl;
	char regPiece[30], kingPiece[30];
	sprintf(regPiece, "\033[1;%sm%c\033[0m", PLAYER1COLOR.c_str(), P1RegularPiece);
	sprintf(kingPiece, "\033[1;%sm%c\033[0m", PLAYER1COLOR.c_str(), P1KingPiece);
	cout << "Player 1 Pawn: " << regPiece << " King: " << kingPiece << endl;
	sprintf(regPiece, "\033[1;%sm%c\033[0m", PLAYER2COLOR.c_str(), P2RegularPiece);
	sprintf(kingPiece, "\033[1;%sm%c\033[0m", PLAYER2COLOR.c_str(), P2KingPiece);
	cout << "Player 2 Pawn: " << regPiece << " King: " << kingPiece << endl;
}