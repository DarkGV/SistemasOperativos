#include <stdio.h>
#include "clientAPI.h"

int main(){
    int s = startCommunication("127.0.0.1");
    if(s < 0){
        printf("Erro ao conectar com o servidor");
        return -1;
    }
getchar();
    if(sendFileTo(s, "Text") < 0){
        printf("Erro ao enviar ficheiro");
    }
    endCommunication(s);
}
