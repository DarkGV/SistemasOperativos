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
    while(1){
        if(received){
            received = 0;
        }
    }

}

