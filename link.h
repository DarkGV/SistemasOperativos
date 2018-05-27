#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>

typedef struct{
    pid_t PID;
    char* Name;
}THREAD;

extern int received;

/*
* estrutura com o nome do ficheiro
* recebido.log
*/
void* create_listener();
