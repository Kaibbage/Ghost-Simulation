CFLAGS = -g -std=c99
CC = gcc
LIB = -lpthread

a5: main.o room.o building.o hunter.o ghost.o evidence.o
	$(CC) $(CFLAGS) -o a5 main.o room.o building.o hunter.o ghost.o evidence.o $(LIB)

all: main.o room.o building.o hunter.o ghost.o evidence.o
	make main.o room.o building.o hunter.o ghost.o evidence.o

main.o: main.c defs.h
	$(CC) $(CFLAGS) -c main.c

room.o: room.c defs.h
	$(CC) $(CFLAGS) -c room.c
        
building.o: building.c defs.h
	$(CC) $(CFLAGS) -c building.c
	
hunter.o: hunter.c defs.h
	$(CC) $(CFLAGS) -c hunter.c
	
ghost.o: ghost.c defs.h
	$(CC) $(CFLAGS) -c ghost.c
	
evidence.o: evidence.c defs.h
	$(CC) $(CFLAGS) -c evidence.c
        
clean:
	rm -f *.o *~ a5

