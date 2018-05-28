#ifndef _CLIENTAPI_H_
#define _CLIENTAPI_H_
#endif

/*
* startComunication(char* address)
* address - de server address which client will communicate.
* default - 127.0.0.1
*returns:
*A socket descriptor handle if no error, -1 if error occurs
*/
extern int startCommunication(char*);

/*
*sendFile(int, char*)
*int socket - the socket connection returned by startCommunication
*char* filename - the log file name to send to the server
*returns:
*0 if no error, -1 if error occurs
*/
extern int sendFileTo(int, char*);

/*
*endCommunication(int)
*int socket - the socket communications descriptor to close
*returns:
*0 if no errors, the socket descriptor if error occurs
*/
extern int endCommunication(int);