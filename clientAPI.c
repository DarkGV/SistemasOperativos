#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
	char* filename;
	pid_t PID;
	char* progName;
	char* dt;
	int logLevel;
	int sessionNumber;
	int sessionSeqNumber;
	char* Message;
}LoggingData;

LoggingData* readFileInfo(char* filename){
	time_t t = time(NULL);
	LoggingData* ldp = (LoggingData*)malloc(sizeof(LoggingData));
	ldp->filename = filename;
	ldp->PID = getpid();
	ldp->progName = getenv("__");
	strftime(ldp->dt, 10, "%c", localtime(&t));
	int fd = open(filename, O_RDONLY);	
}
