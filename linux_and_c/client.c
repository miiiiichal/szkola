/*
 ============================================================================
 Name        : client.c
 Author      : mj
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>

#define ADDR_SIZE sizeof(struct sockaddr_in)
#define BUFF_SIZE 1024

int main(int argc, char *argv[])
{
    int sockfd;
    int conn;
int i;
int end=0;
char buff[BUFF_SIZE];
char buff_read[BUFF_SIZE];


	struct sockaddr_in saddr = {AF_INET, 8001 };
	saddr.sin_addr.s_addr= inet_addr("127.0.0.1");

	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1){
		perror("socket failed");
		exit(-1);
	}
	if(connect(sockfd, (struct sockaddr *) &saddr, ADDR_SIZE)==-1){
		perror("connect failed");
		exit(-1);
	}

	while(1){
		printf("start\nPodaj polecenie :\n");
		if(end==1){
			printf("wyjście na życzenie klienta - KONIEC\n");
		//	strcpy(buff,"wyjście na życzenie klienta - KONIEC");
			close(sockfd);
			exit(0);
		}
		memset(&buff,0,sizeof(buff));
		end=0;
		if(fgets(buff,sizeof(buff), stdin)==NULL){
			perror("fgets err");
			exit(0);
		}
		printf("przeczytanie z wejscia: %s\n",buff);
//		if(buff[strlen(buff)]=='\0'){
//			printf("tak tak tak zakończone końcem\n");
//		}
 		if(strncmp("<exit>",(char *) buff,strlen("<exit>"))==0){
//			printf("równe z <exit>:\n");
			end=1;
		}
		//printf("po sprawdzeniu czy to exit\n");

		write(sockfd,buff, strlen(buff));

//		printf("po napisaniu do serwera\n");

		printf("przed czytaniem z serwera\n");
	// na razie nie czytamy bo serwer nie wysyła i klient się zawiesza

		if(end!=1){
			if(read(sockfd,buff_read,sizeof(buff_read))>0){
				printf("serwera responses: %s\n",buff_read);
				memset(&buff_read,0,sizeof(buff_read));
			}
			else{
				printf("serwer nic nie odpowiedział\n");
			}
		}

		printf("koniec while -> next one \n");
	}
}
