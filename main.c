/*
*Aplicação principal: 
* Thread para criar a ponte entre o client e o servidor 
*   (Aplicação principal é o sv)
*
*/
#include "link.h"

int main()
{
    daemon(1, 1);
    pthread_t thread;
    pthread_create(&thread, NULL, create_listener, NULL);
    int status, fd = 0;
    while(1){
    	status = (rand() % 11);
        if(received > 0) printf("Received: %d\n", received);
        sleep(status);
    }
}