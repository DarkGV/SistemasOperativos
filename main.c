/*
*Aplicação principal: 
* Thread para criar a ponte entre o client e o servidor 
*   (Aplicação principal é o sv)
*
*/
#include "link.h"

int main()
{
    pthread_t thread;
    pthread_create(&thread, NULL, create_listener, NULL);
    int status, fd;
    
    while(1){
    	status = (rand() % 11);
        if(received > 0){
            fd = open ("Received%d.log",received-1 ,O_RDONLY);
            printf("Opening file: Received%d.log\n", received);
        }
        sleep(status);
    }

}