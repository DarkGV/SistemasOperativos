/*
*Aplicação principal: 
* Thread para criar a ponte entre o client e o servidor 
*   (Aplicação principal é o sv)
*
*/
#include "link.h"

int received = 0;

int main()
{
    pthread_t thread;
    pthread_create(&thread, NULL, create_listener, NULL);
    int status, fd;
    
    while(1){
    	printf("OLA?\n");
    	status = (rand() % 11);
        while(received > 0){
        	fd = open ("Received%d.log",received ,O_RDONLY);
        	printf("%d\n", received);
            received--;
        }
        sleep(status);
    }

}