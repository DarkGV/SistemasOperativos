#include <stdio.h>
#include "clientAPI.h"

int main(){
    int s = startCommunication("192.168.207.199");
    if(s < 0){
        printf("Erro ao conectar com o servidor");
        return -1;
    }
    if(sendFileTo(s, "main.c") < 0){
        printf("Erro ao enviar ficheiro");
    }
    endCommunication(s);
}