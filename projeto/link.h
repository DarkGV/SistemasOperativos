#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <netdb.h>

extern int received;

typedef struct{
    pid_t PID;
    char* Name;
}THREAD;

/*
* estrutura com o nome do ficheiro
* recebido.log
*/
void* create_listener();
