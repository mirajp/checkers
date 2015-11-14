// ECE 469: Artificial Intelligence, Prof. Sable
// Checkers - by Miraj Patel
// game.h - Setuping the game itself and internal functions to play the game (with an 8x4 array representing the 8x8 field)

#ifndef GAME_H

#define GAME_H

#include <string>
#include <vector>
#include <unistd.h>

#define	NUMPIECES		12
#define	MAXMOVES		32

#define	NE				1
#define	NW				2
#define	SE				3
#define	SW				4

#define JUMPMOVE		1
#define NONJUMPMOVE		2

// These are used for modulo 2 logic to determine which player's piece it is quickly
#define P1REGULAR		1
#define P2REGULAR		2
#define P1KING			3
#define P2KING			4
#define PEMPTY			0

// These are used to represent the board using ASCII characters
#define P1RegularPiece	'x'
#define P2RegularPiece	'o'
#define P1KingPiece		'K'
#define P2KingPiece		'Q'
#define EmptyPiece		' '

#define MOVESTODRAW		50

using namespace std;

extern const string INVALIDSQCOLOR;
extern const string VALIDSQCOLOR;
extern const string PLAYER1COLOR;
extern const string PLAYER2COLOR;


struct Location {
	short row, column;
	
	Location() { row = -1, column = -1;	}
};

struct Move {
	int startPiece, numJumps;
	Location end, jumpedLocations[NUMPIECES];
	
	Move() { startPiece = -1; numJumps = 0;}
};

class Game {
	private:
		char *colorPieces (char piece);
		string nameSquare(int row, int column);
		void addMove(Move &newMove, char type);
		void setEndLocation(Location *start, Location &end, int direction, int distance);
		bool getJumps(Move &firstJump, int (&directions)[4], int numDirections);
		void getNonJumps(Move &firstJump, int (&directions)[4], int numDirections);
		bool takePiece(Location &captured);
		
	public:
		Game();
		// curTurn = 0 => player 2's turn, curTurn = 1 => player 1's turn
		unsigned int curTurn, numMoves, numNonJumpMoves;
		bool onlyShowJumps;
		char board[8][4];
		std::vector<Location> pieces[2];
		Move movesAvailable[MAXMOVES];
		
		// getNextMoves find the next moves for whoever's turn it is
		void getNextMoves();
		void printNextMoves();
		void chooseMove(int choice);
		
		void printBoard();
};


void copyGame(Game &destGame, Game &srcGame);

#endif
