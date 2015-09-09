// ECE 469: Artificial Intelligence, Prof. Sable
// Checkers - by Miraj Patel
// checkers.cpp - The main program

#include "board.h"
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

int curTurn;
Board theBoard ('p', 'K', 'm', 'Q');
vector <string> moves;

int translateSquare(string square) {
	
	return 0;
}

int getInteger(const std::string & s)
{
	if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

	char * p ;
	strtol(s.c_str(), &p, 10) ;

	if (!(*p == 0)) {
	   return -1;
	}
	else {
		return atoi(s.c_str());
	}
}

void tryMove(int from, int to) {
	cout << "Trying to move from " << from << ", to " << to << endl;
	return;
}

int checkMove(string move) {
	string from, to;
	int index = getInteger(move);
	if (index != -1) {
		if (index < (signed int) moves.size()) {
			move = moves[index-1];
		}
		else {
			cout << "Out of bounds!" << endl;
			// Move choice is out of bounds of the array
			return -2;
		}
	}
	else {
		if (move.find("->") != string::npos) {
			from = move.substr(0, move.find("->"));
			to = move.substr(move.find("->")+2);
			cout << "From: " << from << endl;
			cout << "To: " << to << endl;
		}
		else {
			// Not a correctly formatted input
			return -1;
		}
	}
	return 0;
}

void displayRGBOptions() {
	printf("      ");
	for (int i = 0; i <= 35; i++) {
		printf("%2d ", i);
	}
	printf("\n\n %3d  ", 0);
	for (int i = 0; i <= 15; i++) {
		printf("\033[48;5;%dm  \033[m ", i);
	}
	int j;
	for (int i = 0; i <= 6; i++) {
		j = i*36 + 16;
		printf("\n\n %3d  ", j);
		for (int k = 0; k <= 35; k++) {	
			printf("\033[48;5;%dm  \033[m ", j+k);
		}
	}
	printf("\n\n");
}

void changeDisplay() {
	int choice = 0;
	string rgb = "";
	
	//theBoard.printBoard();
		
	cout << endl << "What would you like to change the color of?\nEnter the number corresponding to your choice:" << endl;
	while(choice != 5) {
		cout << "1. Color of Player 1's 'pieces' (text)" << endl;
		cout << "2. Color of Player 2's 'pieces' (text)" << endl;
		cout << "3. Color of an invalid square a piece can be on" << endl;
		cout << "4. Color of a valid square a piece can be on" << endl;
		cout << "5. Exit color selection" << endl;
		
		cin >> choice;
		if (choice == 5) {
			cout << "Exiting." << endl;
			return;
		}
		
		if (choice == 1) {
			//displayRGBOptions();
			cout << "Pick an RGB value from above by adding the row and column # corresponding to the color square:" << endl;
			cin >> rgb;
			theBoard.set_p1ColorRGB(rgb);
			choice = 0;
			rgb.clear();
			//theBoard.printBoard();
			
			cout << endl << "What else would you like to change the color of?\nEnter the number corresponding to your choice:" << endl;
		}
		else if (choice == 2) {
			//displayRGBOptions();
			cout << "Pick an RGB value from above by adding the row and column # corresponding to the color square:" << endl;
			cin >> rgb;
			theBoard.set_p2ColorRGB(rgb);
			choice = 0;
			rgb.clear();
			//theBoard.printBoard();
			
			cout << endl << "What else would you like to change the color of?\nEnter the number corresponding to your choice:" << endl;
		}
		else if (choice == 3) {
			//displayRGBOptions();
			cout << "Pick an RGB value from above by adding the row and column # corresponding to the color square:" << endl;
			cin >> rgb;
			theBoard.set_invalidSqColorRGB(rgb);
			choice = 0;
			rgb.clear();
			//theBoard.printBoard();
			
			cout << endl << "What else would you like to change the color of?\nEnter the number corresponding to your choice:" << endl;
		}
		else if (choice == 4) {
			//displayRGBOptions();
			cout << "Pick an RGB value from above by adding the row and column # corresponding to the color square:" << endl;
			cin >> rgb;
			theBoard.set_validSqColorRGB(rgb);
			choice = 0;
			rgb.clear();
			//theBoard.printBoard();
			
			cout << endl << "What else would you like to change the color of?\nEnter the number corresponding to your choice:" << endl;
		}
		else {
			cout << "Unrecognized choice # entered..." << endl;
		}
	}
	return;
}

int main() {
	string userResponse = "";
	curTurn = 1;
	int userTurn = 0;
	int retVal = 0;
	bool gameOver = false;
	cout << endl;
	
	cout << "Would you like to change some colors around? (Yes/No)" << endl;
	cin >> userResponse;
	if (userResponse.find("Y") != string::npos || userResponse.find("y") != string::npos) {
		changeDisplay();
	}
	
	while (userTurn == 0) {
		cout << endl << "Which mode would you like to play in? (Keep in mind Player 1 goes first, whose pieces start at row 1)" << endl;
		cout << "1. Computer 1 (Pale Blue) vs. Computer 2 (Jane the Ansible)" << endl;
		cout << "2. Computer 1 (Pale Blue) vs. Human 2 (Challenger)" << endl;
		cout << "3. Human 1 (Challenger) vs. Computer 2 (Pale Blue)" << endl;
		cout << "4. Human 1 (Slowpoke1) vs. Human 2 (Slowpoke2)" << endl;
		cout << "5. To quit." << endl;
		
		cin >> userResponse;
		cout << endl;
		if (userResponse.compare("1") == 0) {
			cout << "Computer vs Computer mode!" << endl;
			userTurn = -1;
		}
		else if (userResponse.compare("2") == 0) {
			cout << "Computer vs Human mode!" << endl;
			userTurn = 2;
		}
		else if (userResponse.compare("3") == 0) {
			cout << "Human vs Computer mode!" << endl;
			userTurn = 1;
		}
		else if (userResponse.compare("4") == 0) {
			cout << "Human vs Human mode!" << endl;
			userTurn = 1;
		}
		else if (userResponse.compare("5") == 0) {
			cout << "Exiting." << endl;
			return 0;
		}
		else {
			cout << "Unrecognized game mode choice entered... Please try again:" << endl;
		}
	}
	
	//theBoard.printBoard();
	cout << "Some instructions:\nMake a move by structured like 'from->to', for example: A3->B4.\n\tOR" << endl;
	cout << "If instead you would like to see all available moves you can make, simply enter 'a'\n\n" << endl;
	//userResponse.clear();
	while(!gameOver) {
		cout << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
		if ( ((curTurn % 2 == 0) && (userTurn == 1)) || ((curTurn % 2 == 1) && (userTurn == 2)) ) {
			cout << "Computer makes a nonsense move..." << endl << endl;
			curTurn++;
		}
		else {
			cout << "It's your turn!  --  What move would you like to make? ('a' to list all)?" << endl;
			cin >> userResponse;
			cout << "You chose: " << userResponse << endl << endl;
			retVal = checkMove(userResponse);
			while(retVal < 0) {
				if (retVal == -2) {
					cout << "Your choice is out of bounds of available moves listed!" << endl;
				}
				else if (retVal == -1) {
					cout << "Your choice was not formatted correctly (missing '->')!" << endl;
				}
				cout << "Please choose again:" << endl;
				cin >> userResponse;
				retVal = checkMove(userResponse);
			}
			
			curTurn++;
		}
		
		if (curTurn == 10) {
			gameOver = true;
		}
	}
	
	cout << "Game over after " << curTurn << " turns! The winner is: no one" << endl;
	
	return 0;
}