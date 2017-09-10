/*
 ============================================================================
 Name        : srv_test.c
 Author      : mj
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFF_SIZE 512

struct myIfData{
	char name[40];
	char ip4[INET_ADDRSTRLEN];
	char ip6[INET6_ADDRSTRLEN];
	char mac[ETH_ALEN];
	char status[5];
};

int getIfData(struct myIfData **ifData, int *ifNum){

	printf("getIfData\n");
	int result = 1, counter=0;
	struct ifaddrs * sIfAddr;

	struct ifreq ifr;
	struct myIfData *data;

	if(getifaddrs(&sIfAddr)==0){

		while(sIfAddr->ifa_next!=NULL){
			int index=0,equal =NULL,iter=0, posInData =-1;
			printf("INTERFEJS : ");
			printf("%s\n",sIfAddr->ifa_name);

			for(iter=0;iter<counter;iter++){
				equal =(strcmp(&(data[iter].name), sIfAddr->ifa_name)!=0 ? 0 : 1);
				if(equal){
					//data[a]->name = sIfAddr->ifa_name;
					posInData =iter;
					break;
				}
			}
			if(posInData==-1){
				counter++;
				data = realloc(data,counter*sizeof(struct myIfData));
				memset((data+counter-1),0,sizeof(struct myIfData));
				posInData=counter-1;

				strcpy(&(data[posInData].name),sIfAddr->ifa_name);
			}
			strcpy(&(data[posInData].status),"UP_\n");
			strcpy(&(data[posInData].mac),"MAC_KWAK\n");
			//data[posInData].name = sIfAddr->ifa_name;
			if(sIfAddr->ifa_addr!=NULL){
				int family = sIfAddr->ifa_addr->sa_family;
				if(family== AF_INET){
					//data[posInData].ip4=sIfAddr->ifa_addr->sa_data;
					//inet_ntop(family, sIfAddr->ifa_addr->sa_data, data[posInData].ip4,INET_ADDRSTRLEN);
					struct sockaddr_in *sa = (struct sockaddr_in *) sIfAddr->ifa_addr;
					strcpy(&(data[posInData].ip4),inet_ntoa(sa->sin_addr));
					printf("IP4: %s\n",data[posInData].ip4);
				}
				if(family== AF_INET6){
					//data[posInData].ip6=sIfAddr->ifa_addr->sa_data;

					struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *) sIfAddr->ifa_addr;
					inet_ntop(family, &sa6->sin6_addr, data[posInData].ip6,INET6_ADDRSTRLEN);
					//strcpy(&(data[posInData].ip6),inet_ntoa(sa6->sin6_addr));
					printf("IP6: %s\n",data[posInData].ip6);
				}
			}
			sIfAddr=sIfAddr->ifa_next;
		}
		*ifNum=counter;
		*ifData = data;
	}
	return result;
}


int parseRequest(char *request, char *property, char ***ifList, int *ifReq){
	printf("parseRequest\n");
	int result = 1;
	memset(property,0,9);
	if(*request == NULL){

		result = 0;
	}
	if(strstr(request,"<list>")!=NULL){
		strncpy(property, "list", 4);
		result = 1;
	}
	if(strstr(request,"<status>")!=NULL){
		int len =strlen(request)-17;
		char req[len];
		memset(req,0,len);
		strncpy(req,request+8,strlen(request)-18);

		*ifReq = findIfInReq(req,ifList);
		strncpy(property, "status", 6);
		result = 2;
	}
	if(strstr(request,"<addr_ip4>")!=NULL){
		strncpy(property, "addr_ip4", 8);
		result = 3;
	}
	if(strstr(request,"<addr_ip6>")!=NULL){
		strncpy(property, "addr_ip6", 8);
		result = 4;
	}
	if(strstr(request,"<addr_hw>")!=NULL){
		strncpy(property, "addr_hw", 7);
		result = 5;
	}
	if(strstr(request,"<exit>")!=NULL){
		strncpy(property, "exit", 4);
		result = 6;
	}
	return result;
}
int findIfInReq(char *request, char ***ifList){
	printf("findIfInReq\n");
	int ifCount=0;
	//char txt[] ="<if>enp0s25</if><if>lo</if>";
	 const char *delim ="</if>";
	 char **iflistLocal;
		char *token = strtok(request,delim);


		while(token){
			if(ifCount==0)
				iflistLocal = (char**) malloc((ifCount+1)*sizeof(char *));
			else
				iflistLocal= (char**) realloc(iflistLocal,(ifCount+1)*sizeof(char *));
			printf("%s\n", token);
			int sss=strlen(token);
			char *wsk = (char *) malloc((strlen(token)+1)*sizeof(char));
			iflistLocal[ifCount]=wsk;
			 memset(iflistLocal[ifCount],0,(strlen(token)+1));

			strcpy(iflistLocal[ifCount], token);
			token=strtok(NULL,delim);
			ifCount++;
		}
		printf("ile tokenów : %d",ifCount);
		*ifList=iflistLocal;
	return ifCount;
}

int createConncection(int *gSocketfd){
	struct sockaddr_in saddr = {AF_INET, 8001, INADDR_ANY};
	int socketfd;
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd == -1){
		perror("socket failed");
		return(-1);
	}

	if(bind(socketfd, (struct sockaddr *) &saddr, sizeof(struct sockaddr_in))== -1){
		perror("bind failed");
		return(-1);
	}

	if( listen(socketfd, 5)==-1){
		perror("listen failed");
		return(-1);
	}

	fflush(stdout);
	printf("Serwer starts...\n");
	*gSocketfd = socketfd;

	return(1);
}

int readRequest(int fd, char **request, int *n){
	printf("readRequest\n");
	int i =1;
	if(fd>0){
		*n=i*BUFF_SIZE;
		char *req = (char*)malloc((*n)*sizeof(char));
		memset(req,0,*n);
		for(int i=2;i<10;i++){

			int readBytes = read(fd, req, *n);
			if(readBytes==-1){
				perror("readRequest Failed\n");
				return(-1);
			}
			if(readBytes>0){

				*request=req;
				return(1);
			}
			else{
				*n=i*BUFF_SIZE;
				*req = realloc(req,(*n) * sizeof(char));
			}
			if(i==10){
				perror("Bląd przy odczytywaniu żądania klienta");
				return(-1);
			}
		}
	}
	else{
		perror("Błędnie przekazany parametr");
		return(-1);
	}
};

int preapareResponse(char *response,char *property,char ***ifList, int ifListCounter, struct myIfData *ifData, int ifDataCounter){
	printf("prepareResponse\n");
	int result = 0;
//	if(*property== NULL){
//		result = -1;
//	}
	memset(response,0,BUFF_SIZE);
	if(strcmp(property,"list")==0){
		for(int i=0; i<ifDataCounter; i++){
			if(i>0){
				strcat(response,";");
			}
			strcat(response,ifData[i].name);
		}
		result=1;
	}
	if(strcmp(property,"status")==0){
		for(int i=0; i<ifDataCounter; i++){
			if(ifListCounter==0){
				// wczytaj wszysteki e interfejsy
				if(i>0){
					strcat(response,";");
				}
				strcat(response,ifData[i].status);
			}
			else{
				//podaj tylko te co w zadaniu
				for(int j=0; j<ifListCounter;j++){
					if(strcmp(ifData[i].name, ifList[j])==0){
						if(j>0){
							strcat(response,";");
						}
						strcat(response,ifData[i].status);
					}
				}
			}
		}
		result=1;
	}
	if(strcmp(property,"addr_ip4")==0){
		for(int i=0; i<ifDataCounter; i++){
			if(ifListCounter==0){
				// wczytaj wszysteki e interfejsy
				if(i>0){
					strcat(response,";");
				}
				strcat(response,ifData[i].ip4);
			}
			else{
				//podaj tylko te co w zadaniu
				for(int j=0; j<ifListCounter;j++){
					if(strcmp(ifData[i].ip4, ifList[j])==0){
						if(j>0){
							strcat(response,";");
						}
						strcat(response,ifData[i].ip4);
					}
				}
			}
		}
		result=1;
	}
	if(strcmp(property,"addr_ip6")==0){
		for(int i=0; i<ifDataCounter; i++){
			if(ifListCounter==0){
				// wczytaj wszysteki e interfejsy
				if(i>0){
					strcat(response,";");
				}
				strcat(response,ifData[i].ip6);
			}
			else{
				//podaj tylko te co w zadaniu
				for(int j=0; j<ifListCounter;j++){
					if(strcmp(ifData[i].ip6, ifList[j])==0){
						if(j>0){
							strcat(response,";");
						}
						strcat(response,ifData[i].ip6);
					}
				}
			}
		}
		result=1;
	}
	if(strcmp(property,"addr_hw")==NULL){
		for(int i=0; i<ifDataCounter; i++){
			if(ifListCounter==0){
				// wczytaj wszysteki e interfejsy
				if(i>0){
					strcat(response,";");
				}
				strcat(response,ifData[i].mac);
			}
			else{
				//podaj tylko te co w zadaniu
				for(int j=0; j<ifListCounter;j++){
					if(strcmp(ifData[i].mac, ifList[j])==0){
						if(j>0){
							strcat(response,";");
						}
						strcat(response,ifData[i].mac);
					}
				}
			}
		}
		result=1;
	}

	if(result==0){
	//prepare response with warning
		strcat(response,"błędna komenda!\0");
		result=1;
	}
	return(result);
}
int sendResponse(int fd, char *response){
	printf("sendResponse\n");
	if(fd >-1){
			printf("%d . wysyłam do klienta:\n %s\n",fd, response);
			write(fd,response,strlen(response));
	}
	else{
		perror("sendResponse failed: błędny dekskryptor pliku\n");
		return(0);
	}

 return 1;
 }


int main(void) {


	int socketfd, newsocketfd;
	int conneNumberAllowed=5;
	fd_set readset,activeset;

	struct myIfData *myData = NULL;
	char props[9], **ifList, *request, response[BUFF_SIZE];
	int ifDataCounter=0, requestSize=0, ifListCounter=0;

	int result, counter=0;

	if(createConncection(&socketfd)<0){
		exit(-1);
	}
		FD_ZERO(&readset);
		FD_ZERO(&activeset);
		FD_SET(socketfd,&activeset);

		while(1){
			printf("czekamy na klienta \n");
			readset=activeset;
			if(select(10,&readset,NULL,NULL,NULL)<0){
				perror("select() error");
				exit(-1);
			}
			printf("ksksksks\n");


			for(int i =3;i<10;++i){
				printf("Nasłuchujemys %d \n",i);
				 if (FD_ISSET (i, &readset)){
					 if (i==socketfd){
						 printf("nowy klient chce się podłączyć \n");
						 if((newsocketfd = accept(socketfd, NULL, NULL))==-1){
							perror("accept failed");
							//exit(0);
						}
						FD_SET(newsocketfd,&activeset);
						printf("nowy klient podłączony %d\n",newsocketfd);
						newsocketfd =NULL;
					 }
					 else{
						 //czytanie requesta i reszta kodu
						 if((readRequest(i, &request, &requestSize))<0){
							 perror("readRequest failed!\n");
						 }
						 else{
							 if((parseRequest(request, &props, &ifList, &ifListCounter))<1){
								 perror("parseREquest failed!\n");
							 }else{
								 if(strcmp(props, "exit")!=0){
									 getIfData(&myData, &ifDataCounter);
									 memset(request,0,BUFF_SIZE);
									 preapareResponse(&response, &props, ifList, ifListCounter, myData, ifDataCounter);
									 printf("response: %s\n",response);
									 sendResponse(i, &response);
								 }
								 else{
									 printf("klient %d sie rozłączył\n",i);
									 FD_CLR(i,&activeset);

								 }
							 }
						 }
					 }
				 }
			}
		}


	for(int i=0; i < ifListCounter;i++)
		free(ifList[i]);
	free(ifList);
	free(myData);
	free(request);
	return EXIT_SUCCESS;
}
