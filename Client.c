#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h>

	#define MAXDATASIZE 100 /* max number of bytes we can get at once */
	#define ARRAY_SIZE 30  /* size of array to be sent */
	#define PORT_NO 12345 /* PORT Number */
	
	
static int getLine (int *buff, size_t sz);


int main(int argc, char *argv[]) {
	int sockfd, numbytes, readline;  
	char buf[MAXDATASIZE];
	char sendbuffer[ARRAY_SIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information */
	
	for (int i = 0; i < MAXDATASIZE; i++){
		buf[i] = 0;
	}

	if (argc != 3) {
		fprintf(stderr,"usage: client_hostname port \n");
		exit(1);
	}

	if ((he=gethostbyname(argv[1])) == NULL) {  /* get the host info */
		herror("gethostbyname");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}


	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(*argv[2]);    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */
	
	
	
	if (connect(sockfd, (struct sockaddr *)&their_addr, \
	sizeof(struct sockaddr)) == -1) {
		perror("Failed to Connect");
		exit(1);
	}
	while (1){
		/* Receive message from server */
	if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
		perror("recv");
		exit(1);
	} 
	buf[numbytes] = '\0';
	printf("Received: %s",buf);
	
	
	fgets(sendbuffer,1024-1,stdin);
        if ((send(sockfd,sendbuffer, strlen(sendbuffer),0))== -1) {
                fprintf(stderr, "Failure Sending Message\n");
                close(sockfd);
                exit(1);
        }
	}
	close(sockfd);
	
	return 0;
}