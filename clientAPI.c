#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

int startCommunication(){

	int s = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in svAddr;

	memset(&svAddr, 0, sizeof(svAddr));
	svAddr.sin_addr.s_addr = inet_addr("192.168.1.4");
	svAddr.sin_family = AF_INET;
	svAddr.sin_port = htons(1024);

	if(connect(s, (struct sockaddr*)&svAddr, sizeof(svAddr)) < 0) return -1;
	return s;
}

int sendFileTo(int communication, char* filename){
	struct stat fStat;
	char fileSize[1024];
	int fd = open(filename, O_RDONLY);
	if(fstat(fd, &fStat) < 0) return -1;

	/*Send file size*/
	sprintf(fileSize, "%lld", fStat.st_size);
	if(send(communication, fileSize, sizeof(fileSize), 0) < 0){
		return -1;
	}

	return 0;
}

int endCommunication(int communication){
	if(send(communication, "-1", 1, 0) < 0) return communication;

	close(communication);
	return 0;
}