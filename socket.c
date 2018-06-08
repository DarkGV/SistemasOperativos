#include "link.h"

#define MAXLEN 25
#define MAXBUFFERSIZE 1024

int received;
int nmrConnections;
int* Connection;
pthread_mutex_t lock;

void* handle_client(void*args){
    /*struct hostent *hp = gethostbyname(address);*/
    int cliSock = *(int*)args, fd;
    char fileSize[MAXBUFFERSIZE], *fileName = (char*)malloc(14*sizeof(char));
    int nBytes = 0;
    /*printf("Conected to %s\n", hp->h_name);
    printf("Receiving data...\n");*/
    pthread_mutex_lock(&lock);
    if(sprintf(fileName, "Received%d.log", received) < 0){
        pthread_mutex_unlock(&lock);
        exit(-1);
    }
    received++;
    pthread_mutex_unlock(&lock);
    fd = creat(fileName, S_IRUSR | S_IWUSR);

    /*
    *           Receive file size
    *           Not using this value..
    */
    if(recv(cliSock, fileSize, MAXBUFFERSIZE, 0) < 0){
        printf("Error receiving file size\n");
        exit(-1);
    }
    memset(fileSize, 0, sizeof(fileSize));
    /*Receive file*/
    while((nBytes = recv(cliSock, fileSize, MAXBUFFERSIZE, 0)) > 0){
        if(write(fd, fileSize, nBytes) < 0) exit(-1);
        memset(fileSize, 0, sizeof(fileSize));
    }
    if(nBytes < 0) printf("Error receiving file\n");
    else received++;
    close(cliSock);
    return NULL;
}

void* create_listener(void*args) {
    received = 0;
    nmrConnections = 0;
    nmrConnections++;
    Connection = (int*)malloc(nmrConnections*sizeof(int));
    pthread_t thread;
    int sock;
    struct sockaddr_in svAddr, cliAddr;
    socklen_t clilen;

    pthread_mutex_init(&lock, NULL);

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(sock < 0){
        printf("Error openening socket...\n");
        exit(-1);
    }

    memset(&svAddr, 0, sizeof(svAddr));

    svAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    svAddr.sin_family = AF_INET;
    svAddr.sin_port = htons(1024);

    if(bind(sock, (struct sockaddr*) &svAddr, sizeof(svAddr)) < 0){
        printf("Error binding socket...\n");
        exit(-1);
    }

    if(listen(sock, MAXLEN) < 0){
        printf("Error listening socket...\n");
        exit(-1);
    }
    for(;;){
        clilen = sizeof(cliAddr);
        Connection[nmrConnections-1] = accept(sock, (struct sockaddr*)&cliAddr, &clilen);
        /*Let's see if everythings fine!*/
        if(Connection[nmrConnections-1] > 0){
            pthread_create(&thread, NULL, handle_client, &Connection[nmrConnections-1]);
            Connection = (int*)realloc(Connection, ++nmrConnections * sizeof(int));
            //close(cliSock);
        }
    }
}
