# Makefile
LDFLAGS=-lncurses -lm  -Wall -Wextra -ansi -g

all:  morpion
morpion: menu.o gamelogic.o explosion.o game.o header.h
		cc $(LDFLAGS)  menu.o  gamelogic.o game.o explosion.o -o morpion
menu.o: menu.c
				cc $(LDFLAGS) -c menu.c   -o menu.o
gamelogic.o: gamelogic.c
				cc $(LDFLAGS) -c gamelogic.c   -o gamelogic.o
game.o: game.c
				cc $(LDFLAGS) -c game.c   -o game.o
explosion.o: explosion.c
				cc $(LDFLAGS) -c explosion.c   -o explosion.o
clean:
				rm -f *.o core
mrproper: clean
				rm -f mon_executable
