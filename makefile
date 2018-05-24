CC=gcc
FLAGS=-c -Wall
LIBS=-lm
OBS = main.o socket.o

all:teste

socket.o: link.h socket.c
		$(CC) $(FLAGS) socket.c

main.o: link.h main.c
		$(CC) $(FLAGS) main.c

teste: $(OBS)
		$(CC) -o teste $(OBS) $(LIBS)