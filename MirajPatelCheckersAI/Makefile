objfiles = checkers.o game.o player.o

checkers.out: $(objfiles)
	g++ $(objfiles) -o checkers.out

checkers.o: checkers.cpp
	g++ -c checkers.cpp

player.o: player.cpp player.h
	g++ -c player.cpp
	
game.o: game.cpp game.h
	g++ -c game.cpp
