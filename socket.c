#include "link.h"

#define MAXLEN 25
#define MAXBUFFERSIZE 1024

int received;

int handle_client(int handler, char* address){
    struct hostent *hp = gethostbyname(address);
    int cliSock = handler, fd;
    char fileSize[MAXBUFFERSIZE], *fileName = (char*)malloc(14*sizeof(char));
    int nBytes = 0;
    printf("Conected to %s\n", hp->h_name);
    printf("Receiving data...\n");

    if(sprintf(fileName, "Received%d.log", received) < 0) return -1;
    fd = creat(fileName, S_IRUSR | S_IWUSR);

    /*
    *           Receive file size
    *           Not using this value..
    */
    if(recv(cliSock, fileSize, MAXBUFFERSIZE, 0) < 0){
        printf("Error receiving file size\n");
        return -1;
    }
    memset(fileSize, 0, sizeof(fileSize));
    /*Receive file*/
    //printf("Hey im receiving something...\n");
    while((nBytes = recv(cliSock, fileSize, MAXBUFFERSIZE, 0)) > 0){
        //printf("Ptztzt isto e' o ficheiro: %s \n",fileSize);
        if(write(fd, fileSize, nBytes) < 0) return -1;
        memset(fileSize, 0, sizeof(fileSize));
    }
    printf("Primeiro%d\n", received);
    if(nBytes < 0) printf("Error receiving file\n");
    else received++;
    printf("%d\n", received);
    close(cliSock);
    return 0;
}

void* create_listener(){
    received = 1;
    pid_t PID;
    int sock, cliSock;
    struct sockaddr_in svAddr, cliAddr;
    socklen_t clilen;
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(sock < 0){
        printf("Error openening socket...\n");
        received = 0;
        exit(-1);
    }

    memset(&svAddr, 0, sizeof(svAddr));

    svAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    svAddr.sin_family = AF_INET;
    svAddr.sin_port = htons(1234);

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
        cliSock = accept(sock, (struct sockaddr*)&cliAddr, &clilen);

        /*Let's see if everythings fine!*/
        if(cliSock > 0){
            PID = fork();
            if(PID == 0){
                close(sock);
                handle_client(cliSock, inet_ntoa(cliAddr.sin_addr));
                exit(0);
            }
            close(cliSock);
        }
    }
}
