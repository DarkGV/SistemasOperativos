#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
//#include <openssl/aes.h>
#include "clientAPI.h"

#define MAXBUFFSIZE 1024

int createLogFile(char* logName ,char* data, int logLevel){
	time_t now;
	struct tm* timeInfo;
	int fd = creat(logName, S_IRWXU);

	LogFile myLog;

	myLog.pid = getppid();
	char buffer[1024];
	sprintf(buffer, "%d\n", myLog.pid);
	if(write(fd, buffer, strlen(buffer)) < 0) return -1;

	myLog.program_name = &(getenv("_"))[2];
	sprintf(buffer, "%s\n", myLog.program_name);
	if(write(fd, buffer, strlen(buffer)) < 0) return -1;

	time(&now);
	timeInfo = localtime(&now);
	myLog.date = asctime(timeInfo);
	sprintf(buffer, "%s", myLog.date);
	if(write(fd, buffer, strlen(buffer)) < 0) return -1;

	myLog.level = logLevel;
	sprintf(buffer, "%d\n", myLog.level);
	if(write(fd, buffer, strlen(buffer)) < 0) return -1;

	myLog.Data = data;
	sprintf(buffer, "%s\n", myLog.Data);
	if(write(fd, buffer, strlen(buffer)) < 0) return -1;

	close(fd);
	return 0;
}

int startCommunication(char* addr){

	int s = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in svAddr;

	memset(&svAddr, 0, sizeof(svAddr));
	if(addr != NULL) svAddr.sin_addr.s_addr = inet_addr(addr);
	else svAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	svAddr.sin_family = AF_INET;
	svAddr.sin_port = htons(1024);

	if(connect(s, (struct sockaddr*)&svAddr, sizeof(svAddr)) < 0) return -1;
	return s;
}

int sendFileTo(int communication, char* filename){
	struct stat fStat;
	char fileSize[MAXBUFFSIZE];
	int nBytes;
	int fd = open(filename, O_RDONLY);
	if(fstat(fd, &fStat) < 0) return -1;

	/*Send file size*/
	sprintf(fileSize, "%lld", fStat.st_size);
	if(send(communication, fileSize, sizeof(fileSize), 0) < 0){
		return -1;
	}

	memset(fileSize, 0, sizeof(fileSize));
	while((nBytes = read(fd, fileSize, MAXBUFFSIZE)) > 0){
		if(send(communication, fileSize, nBytes, 0) < 0) return -1;
		memset(fileSize, 0, sizeof(fileSize));
	}

	return 0;
}

int endCommunication(int communication){
	if(send(communication, "-1", 1, 0) < 0) return communication;

	close(communication);
	return 0;
}
