CC = g++
CFLAGS =-std=c++0x -g -O2 -Wall -Wshadow -Wcast-align \
	-Wredundant-decls \
	-Wpointer-arith -Wwrite-strings -finline-functions
EXEC = crypto

all :
	$(CC) crypto.cpp  $(CFLAGS) -o $(EXEC)

clean :
	rm -rf *.o *~ $(EXEC)
