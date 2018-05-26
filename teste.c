#include <stdio.h>
#include "clientAPI.h"

int main(){
    int s = startCommunication();
    if(sendFileTo(s, "main.c") < 0){
        printf("Erro ao enviar ficheiro");
    }
    endCommunication(s);
}