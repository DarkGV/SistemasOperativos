#include <stdio.h>
#include "clientAPI.h"

int main(){

    int s = startCommunication("127.0.0.1");

    if(s < 0){
    
        printf("Erro ao conectar com o servidor");
        return -1;
    }
    
    createLogFile("logFileSend.log", "coninha", 4);

    if(sendFileTo(s, "logFileSend.log") < 0){
        printf("Erro ao enviar ficheiro");
    }

    while(1) ;
}
