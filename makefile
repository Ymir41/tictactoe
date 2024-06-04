CC      = g++
CFLAGS  = -g
RM      = rm -f
LDFLAGS = -lncurses -lstdc++fs 


default: all

all: TicTacToe

TicTacToe: main.cpp
	$(CC) $(CFLAGS) -o TicTacToe main.cpp $(LDFLAGS)

clean veryclean:
	$(RM) TicTacToe
