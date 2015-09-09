objfiles = checkers.o board.o game.o

checkers.out: $(objfiles)
	g++ $(objfiles) -o playcheckers.out

checkers.o: checkers.cpp
	g++ -c -Wall checkers.cpp

board.o: board.cpp board.h
	g++ -c -Wall board.cpp

game.o: game.cpp game.h
	g++ -c -Wall game.cpp
