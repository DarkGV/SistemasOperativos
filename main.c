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
    int status, fd = 0;
    while(1){
    	status = (rand() % 11);
        printf("Conexoes: %d", nmrConnections);
        getchar();
        sleep(status);
    }
}