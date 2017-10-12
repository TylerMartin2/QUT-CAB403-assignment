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

void getMessage(int sock_fd, char buffer[]);
void sendMessage(int sock_fd, char * message);
int userMenu();


int main(int argc, char *argv[]) {
	int sock_fd, numbytes, readline;  
	int input;
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

	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(atoi(argv[2]));    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */
	
	if (connect(sock_fd, (struct sockaddr *)&their_addr, \
	sizeof(struct sockaddr)) == -1) {
		perror("Failed to Connect");
		exit(1);
	}
	//----------------------------------------------------------------------------------
	// Authentication
	
	printf("Enter Username: ");
	fgets(sendbuffer,1024-1,stdin);
	sendMessage(sock_fd, sendbuffer);
	
		
	printf("Enter Password: ");
	fgets(sendbuffer,1024-1,stdin);
	sendMessage(sock_fd, sendbuffer);
	
	//result
	getMessage(sock_fd, buf);
	
	if (strcmp(buf,"authPass")!=0){ // not equal
		printf("Authentication Failed");
		close(sock_fd);
		exit(0);
	} else {
		printf("Authentication Accepted\n");
	}
	
	//----------------------------------------------------------------------------------
	// Main Connection Loop
	while (1){		
		//***hangman title
		do
		{
			printf("----- Enter your choice: -----\n\n");
			printf("1. Play Hangman\n");
			printf("2. Show Leaderboard\n");
			printf("3. Exit\n\n");
			printf("Enter your choice [0-2]:");
	
			fgets(buf, 1024-1, stdin);
			sendMessage(sock_fd, buf);
			input = atoi(buf);
		
		} while ((input > 3) || (input < 1));

		
		switch(input) 
		{
			case 1:
				printf("1");
			case 2:
				printf("2");
				
			case 3:
				printf("3");
				exit(0);
				
		}
		//char * message = malloc(20);
		/* if (userMenu() == 1) {
			strcpy(sendbuffer, "game");
			sendMessage(sock_fd, sendbuffer);
		} else if (userMenu() == 2) {
			strcpy(sendbuffer, "leaderboard");
			sendMessage(sock_fd, sendbuffer);
		
		} else {
			strcpy(sendbuffer, "User has exit");
			sendMessage(sock_fd, sendbuffer);
			break;
		} */
		
		//***leaderboard
		
		//***hangman game loop
		
		
		//getMessage(sock_fd,buf);
		//printf("%s",buf);
	}
	
	close(sock_fd);
	exit(0);
	
	return 0;
}

void getMessage(int sock_fd, char buffer[]){
	int numbytes;
	if ((numbytes=recv(sock_fd, buffer, MAXDATASIZE, 0)) == -1) {
		perror("recv");
		close(sock_fd);
		exit(1);
	} 
	buffer[numbytes] = '\0';
}


void sendMessage(int sock_fd, char * message){
	if (send(sock_fd,message, strlen(message),0)== -1) {
		fprintf(stderr, "Failure Sending Message\n");
		close(sock_fd);
		exit(1);
	}
}

int userMenu() {
	char buffer[1024];
	int input;
	
		do {
			printf("----- Enter your choice: -----\n\n");
			printf("1. Play Hangman\n");
			printf("2. Show Leaderboard\n");
			printf("3. Quit\n\n");
			printf("Selection option 1-3: ");
	
			fgets(buffer, sizeof(buffer), stdin);
			input = atoi(buffer);
		
		} while ((input > 3) || (input < 1));
		
		return input;
}