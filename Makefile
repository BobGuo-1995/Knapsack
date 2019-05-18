.SUFFIXES: .cpp

CC = g++
CFLAGS = -g -DDEBUG -std=c++14
LDIR = 
LIBS =

OBJ1 = knapsack1.o
OBJ2 = knapsack2.o

all: knapsack1 knapsack2

knapsack1: $(OBJ1)
	$(CC) $(CFLAGS) -o knapsack1 $(OBJ1) $(LDIR) $(LIBS)

knapsack2: $(OBJ2)
	$(CC) $(CFLAGS) -o knapsack2 $(OBJ2) $(LDIR) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) -c $?

clean:
	rm -f knapsack1 knapsack2 $(OBJ1) $(OBJ2)
