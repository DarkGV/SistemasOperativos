CC=cc
FLAGS=-c -Wall
LIBS=-lm -lpthread 
OBS=main.o socket.o

all :  logserver

socket.o : link.h socket.c
	$(CC) $(FLAGS) socket.c	
main.o : link.h main.c
	$(CC) $(FLAGS) main.c
logserver : $(OBS)
	$(CC) -o logserver $(OBS) $(LIBS)

clean limpar:
	rm -f logserver *.o
	rm -f *~
	echo "Cleaning trash"
