#include "link.h"

int received = 0;
int nmrConnections = 0;
int* Connection = NULL;
int flag = -1;
int work = 0;
int threading = 1;
int flagStop = 0;

void itoa (int a, char* num);

void handleRequest (int fd);

void handleLogging (int fd, int* archived);

char* readFifo (int FIFO);

void* flags (void* pipe);

int main()
{
    pthread_t thread, fofi;
    int fd, n, archived = 0, aux1, pipe, pepi, rep, log, n1, alloc, sv = 0;
    long int success = 0;
    char name[150], aux[50], buff[7], aux2[512];
    char * FIFO = "cachimbo", *FOFI = "pipeta", *REP = "reports";

    mkfifo(FIFO,0666);
    mkfifo(FOFI,0666);
    mkfifo(REP, 0666);
    success = pthread_create(&thread, NULL, create_listener, NULL);
    pipe = open (FIFO, O_RDONLY);
	if (pipe < 0)
	{
		printf("Couldn't open fifo\n");
		return -1;
	}
	pepi = open (FOFI, O_WRONLY);
	if (pepi < 0)
	{
		printf("Couldn't open fifo\n");
		return -1;
	}
	rep = open(REP, O_WRONLY);
	if (rep < 0)
	{
		printf("Couldn't open fifo\n");
		return -1;
	}
	if (success == 0)
	{
		alloc = 1;
		write (rep, "Listener success\n",17);
	}
	pthread_create(&fofi, NULL, flags, &pipe);
    while(1){
    	if (sv == 0 && flag != 1)
    		continue; 
    	if (work == 1) {
    		work = 0;
    		if (flag == 2) {
    			received = 0;
    			archived = 0;
    			nmrConnections = 0;
    			Connection = NULL;
    			flagStop = 1;
    			sv = 0;
    			continue;
    		}
    		else if (flag == 3) {
    			received = 0;
    			archived = 0;
    			nmrConnections = 0;
    			Connection = NULL;
    			flagStop = 1;
    			while (flagStop == 1) ;
    		}
    		else if (flag == 4){
    			sv = 0;
    			continue;
    		} 
    		else if (flag == 5){
    			itoa ((nmrConnections-1), aux);
    			lseek(pepi, 0, SEEK_SET);
    			write(pepi, aux, 4);
    		}
    		else if (flag == 6){
    			read(pipe, aux, 4);
    			n = atoi(aux);
    			Connection[n-1] = -1;
    		}
    		else if (flag == 7){
    			sv = 0;
    			continue;
    		}
    		else if (flag == 8){
    			log = open ("Data.log", O_RDWR);
    			if (log < 0) {
					printf("Error opening Data.log\n");
					return -1;
				}
				while ( (n1=read (log, aux2, 512)) > 0 )
					if (write(pepi, aux2, n1) < 0) {
						printf("Error writting fifo\n");
						break;
					}
				close(log);
    		}
    		else if (flag == 1) {
    			if (alloc == 0) {
    				success = pthread_create(&thread, NULL, create_listener, NULL);
    				if (success == 0)
					{
						alloc = 1;
						write (rep, "Listener success\n",17);
					}
    			}
    			sv = 1;
    		}
    	}
    	printf("ESPERA...ESPERA...\n");
    	name[0] = '\0';
        if (received != 0){    	
        	itoa (received, aux);
        	strcat(name, "Received");
        	strcat(name, aux);
        	strcat(name, ".log");
        	write (rep, name, 16);
        	write (rep, "\n", 1);
        	printf("NAME - %s RECEIVED - %d\n", name, received);
        	fd = open (name, O_RDONLY);
        	if (fd < 0)
        	{
        		printf("Error Opening File\n");
        		remove(name);
        		received--;
        		continue;
        	}
        	if ((n = read (fd, buff, 7)) > 0)
        	{
        		printf("%s\n", buff);
        		if (strncmp(buff,"Request",7) == 0)
        			handleRequest(fd);
        		if (strncmp(buff,"Logging",7) == 0) {
        			aux1 = archived;
        			handleLogging(fd, &archived);
        			if (aux1 != archived)
        			{
        				if (remove(name) < 0)
        				{
        					printf("Error deleting file\n");
        				}
        			}
        		}
        	}
            received--;
            close(fd);
        }
        sleep(5);
    }
    close(pepi);
    close(pipe);
    close(rep);
}

void itoa (int a, char* num) {
	int aux = a, i;
	for (i=0; i<50; i++)
		num[i] = '\0';
	i = 0;
	while ((aux % 10)) {
		if (i >= 50)
			return ;
		if (aux == 0) {
			num[i] = '0';
			return;
		}
		num[i] = (char) ((aux % 10) + 48);
		aux = aux / 10;
		i++;
	}
	return ;
}

void handleRequest (int fd) {

	return ;
}

void handleLogging (int fd, int *archived) {
	int log, n;
	char buff[1024];
	printf("TOU A LIDAR COM O LOGGING\n");
	log = open ("Data.log", O_RDWR);
	if (log < 0) {
		printf("Error opening Data.log\n");
		return ;
	}
	printf("ABRI O FICHEIRO DOS LOGS\n");
	lseek(log, 0, SEEK_END);
	lseek(fd, 7, SEEK_SET);
	printf("TOU POSICIONADO PARA ESCREVER E LER\n");
	while (1) {
		n = read(fd, buff, 1024);
		if (n < 0)
			break;
		printf("OLAOLA\n");
		if (write (log, buff, n) < 0) {
			printf("Error writing to file\n");
			return ;
		}
		if (n < 1024)
			break;
	}
	(*archived)++;
	close (log);

	return ;
}

void* flags (void* pipe) {
	int pip = * (int*) pipe;
	char aux[256];
	while (threading) {
		read(pip, aux, 256);
		if (strncmp(aux, "start", 5) == 0) {
			flag = 1;
			work = 1;
		}
		else if (strncmp(aux, "stop", 4) == 0) {
			flag = 2;
			work = 1;
		}
		else if (strncmp(aux, "restart", 7) == 0) {
			flag = 3;
			work = 1;
		}
		else if (strncmp(aux, "pause", 5) == 0) {
			flag = 4;
			work = 1;
		}
		else if (strncmp(aux, "numc", 4) == 0) {
			flag = 5;
			work = 1;
		}
		else if (strncmp(aux, "terc", 4) == 0) {
			flag = 6;
			work = 1;
		}
		else if (strncmp(aux, "report", 6) == 0) {
			flag = 7;
			work = 1;
		}
		else if (strncmp(aux, "access", 6) == 0) {
			flag = 8;
			work = 1;
		}
	}
	return pipe;
}