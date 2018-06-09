#include <stdio.h>
#include "clientAPI.h"

int main(){
    /*ISTO INICIA UMA SESSÃO COM O SERVIDOR*/
    int s = startCommunication("192.168.207.199");
                                /*IP DO SERVIDOR*/
    if(s < 0){
        /*ISTO É A MENSAGEM DE ERRO PARA CASO DÊ*/
        /*ERRO AO LIGAR AO SERVIDOR*/
        printf("Erro ao conectar com o servidor");
        return -1;
    }
    /*ISTO CRIA O FICHEIRO .log*/
    createLogFile("logFileSend.log", "HELP! I NEED SOMEBODY", 4);

    /*ISTO ENVIA O FICHEIRO .log*/
    if(sendFileTo(s, "logFileSend.log") < 0){
        printf("Erro ao enviar ficheiro");
    }

    while(1)
    ;
}