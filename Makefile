CC = g++
CFLAGS =-g -O2 -Wall -Wshadow -Wcast-align \
	-Waggregate-return \
	-Wredundant-decls \
	-Wpointer-arith -Wwrite-strings -finline-functions
EXEC = crypto

all :
	$(CC) crypto.cpp  $(CFLAGS) -o $(EXEC)

clean :
	rm -rf *.o *~ $(EXEC)