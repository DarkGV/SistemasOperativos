#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

time_t pwtimeout;

void clearscreen ();

void menuP ();

void menuC ();

void menuA ();

int testPW ();

void itoa (int a, char* num);

void sendtofifo (int FIFO, char* s);

int main () {
	int fd, pepi, rep, opt = 9, opt2 = 9,n;
	char * FIFO = "cachimbo", *FOFI = "pipeta", *REP = "reports", aux[512];
	mkfifo (FIFO,0666);
	mkfifo (FOFI,0666);
	mkfifo (REP, 0666);
	fd = open (FIFO, O_WRONLY);
	if (fd < 0)
	{
		printf("Couldn't open fifo\n");
		return -1;
	}
	pepi = open (FOFI, O_RDONLY);
	if (pepi < 0)
	{
		printf("Couldn't open fifo\n");
		return -1;
	}
	rep = open (REP, O_RDONLY);
	if (rep < 0)
	{
		printf("Couldn't open fifo\n");
		return -1;
	}
	while (1) {
		if (time(NULL) >= pwtimeout)
			if (!testPW())
				continue;

		if (opt2 == 9) {
			clearscreen();
			menuP();
			scanf("%d", &opt);
		}
		switch (opt)
		{
			case 1: clearscreen();
					menuC();
					scanf("%d", &opt2);
					switch (opt2) {
						case 1: sendtofifo(fd, "start");
								break;
						case 2: sendtofifo(fd, "stop");
								break;
						case 3: sendtofifo(fd, "restart");
								break;
						case 4: sendtofifo(fd, "pause");
								break;
						case 5: sendtofifo(fd, "numc");
								read(pepi, aux, 4);
								printf("Number of clients: %d\n", atoi(aux));
								break;
						case 6: sendtofifo(fd, "terc");
								scanf("%d", &n);
								itoa(n, aux);
								sendtofifo(fd, aux);
								break;
						default: opt2=9;
					}
					break;
			case 2: clearscreen();
					menuA();
					scanf("%d", &opt2);
					switch (opt2) {
						case 1: sendtofifo(fd, "report");
								while (1) {
									n = read(rep, aux, 512);
									if (n < 0)
										break;
									write(STDOUT_FILENO, aux, n);
									if (n < 512)
										break;
								}
								break;
						case 2: sendtofifo(fd, "access");
								n = 0;
								while (1) {
									n = read(pepi, aux, 512);
									if (n < 0)
										break;
									write(STDOUT_FILENO, aux, n);
									if(n < 512)
										break;
								}
								printf("FODASSE\n");
								close(pepi);
								pepi = open (FOFI, O_RDONLY);
								if (pepi < 0)
								{
									printf("Couldn't open fifo\n");
									return -1;
								}
								printf("CONA\n");
								break;
						default: opt2=9;
					}
					break;
			default: exit(0);
		}
	}
	close(pepi);
	close(fd);
}

void clearscreen () {
	int i;
	for (i=0;i<5;i++)
		printf("\n");
	return ;
}

void menuP () {
	printf("1. Controlo\n");
	printf("2. Aceder a dados\n");
	printf("0. Exit\n");
}

void menuC () {
	printf("1. Start Server\n");
	printf("2. Stop Server\n");
	printf("3. Restart Server\n");
	printf("4. Pause Server\n");
	printf("5. Number of clients\n");
	printf("6. Terminate client session\n");
	printf("9. Back\n");
}

void menuA () {
	printf("1. Reports\n");
	printf("2. Access Logged Data\n");
	printf("9. Back\n");
}

int testPW () {
	int fd, n;
	char buff[16], pw[16];

	fd = open("Pw.txt", O_RDONLY);
	if (fd < 0) {
		printf("Couldn't open file\n");
		return 0;
	}
	if (read(fd, buff, 16) < 0) {
		printf("Couldn't read\n");
		return 0;
	}
	printf("Introduza a password:\n");
	scanf("%s", pw);
	if ((n=strlen(pw)) > 16)
		printf("Menor que 16.\n");

	while (strncmp(pw, buff, n) != 0)
	{
		printf("Sorry.\n");
		printf("Introduza a password:\n");
		scanf("%s", pw);
		if ((n=strlen(pw)) > 16)
			printf("Menor que 16.\n");
	}

	printf("OK.\n");
	printf("Authorized for 5 minutes\n");
	pwtimeout = time(NULL);
	pwtimeout += 300;
	close (fd);
	return 1;
}

void itoa (int a, char* num) {
	int aux = a, i;
	for (i=0; i<50; i++)
		num[i] = '\0';
	i = 0;
	while ((aux % 10)) {
		if (i >= 50)
			return ;
		num[i] = (char) ((aux % 10) + 48);
		aux = aux / 10;
		i++;
	}
	return ;
}

void sendtofifo (int FIFO, char* s) {
	int n = strlen(s);
	lseek(FIFO, 0, SEEK_SET);
	write(FIFO, s, n);
}