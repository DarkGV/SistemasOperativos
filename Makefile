CC=cc
FLAGS=-c -Wall
LIBS=-lm -lpthread -lcrypto -lssl
OPENSSL=-I/usr/local/opt/openssl/include
OBS=main.o socket.o clientAPI.o

all :  logserver

socket.o : link.h socket.c
	$(CC) $(FLAGS) socket.c	
main.o : link.h main.c
	$(CC) $(FLAGS) main.c	
clientAPI.o: clientAPI.c
	$(CC) $(FLAGS) clientAPI.c $(OPENSSL)
logserver : $(OBS)
	$(CC) -o logserver $(OBS) $(LIBS)

clean limpar:
	rm -f logserver *.o
	rm -f *~
	echo "Cleaning trash"
