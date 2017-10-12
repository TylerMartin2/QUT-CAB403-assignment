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
		printf("Authentication Failed \n");
		close(sock_fd);
		exit(0);
	} else {
		printf("Authentication Accepted\n");
	}
	
	//----------------------------------------------------------------------------------
	// Main Connection Loop
	while (1){		
		//***hangman title
		// do
		// {
			// printf("----- Enter your choice: -----\n\n");
			// printf("1. Play Hangman\n");
			// printf("2. Show Leaderboard\n");
			// printf("3. Exit\n\n");
			// printf("Enter your choice [0-2]:");
	
			// fgets(buf,1024-1,stdin);
			// printf("\nbuf =%s. \n", buf);
			// buf[1] = '\0';
			
			// input = atoi(buf);
			// printf("input =%d. \n", input);
			
			// if ((input > 3) || (input < 1)){
				// printf("please enter a number within the range supplied \n");
			// }
		
		// } while ((input > 3) || (input < 1));

		int selection = userMenu(sock_fd);
		printf("\ninput =%d. \n", selection);
		if (selection == 1) {
			char guessed_letters[10] = "";
			int guesses_left = 10;
			int word_size;
			int counter = 0;
			//char my_string[] = "elephant";
			char temp[10] = "";
			printf("game\n");
			printf("%d", guesses_left);
			
				
			getMessage(sock_fd, buf);
			printf("No of letters %s\n", buf);
			//sscanf(buf, "%c", &word_size, temp);
			//printf("something %d\n", word_size);
			strcpy(temp, buf);
			printf("%c", temp[2]);
			word_size = strlen(temp);
			//getMessage(sock_fd, buf);
			//sscanf(buf, "%c", temp);
			/* for (int i = 0; i < word_size; i++) {
				temp[i] = *"_";
			} */
			while (guesses_left > 0) {
				for (int i = 0; i<sizeof(guessed_letters)/sizeof(char); i++){
					printf("%c", guessed_letters[i]);
				}
				printf("\nNumber of guesses left: %d\n", guesses_left);
				for (int i = 0; i < word_size; i++){
					printf("%c", temp[i]);
					
				}
				printf("\nEnter guess:");
				fgets(buf, sizeof(buf), stdin);
				sendMessage(sock_fd, buf);
				guesses_left--;
				guessed_letters[counter]=*buf;
				counter++;
				getMessage(sock_fd, buf);
				strcpy(temp, buf);
				/* for (int i = 0; i < sizeof(my_string)/sizeof(char); i++){
					if (*buf == my_string[i]){
						//printf("%c", *buffer);
						temp[i]= my_string[i];
					}
				} */
			}
				
			
			
		} else if (selection == 2) {
				printf("leaderboard\n");
				break;
				
		} else {
			
				printf("quit");
				exit(0);
				break;
		
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
	for (int i =0;i<strlen(buffer);i++){
		if(buffer[i] == '\n'){
			buffer[i] = '\0';
		}
	}	
}


void sendMessage(int sock_fd, char *message){
	if (send(sock_fd,message, strlen(message),0)== -1) {
		fprintf(stderr, "Failure Sending Message\n");
		close(sock_fd);
		exit(1);
	}
}

int userMenu(int sock_fd) {
	char buffer[1024];
	int input;
	
		do {
			printf("----- Enter your choice: -----\n\n");
			printf("1. Play Hangman\n");
			printf("2. Show Leaderboard\n");
			printf("3. Quit\n\n");
			printf("Selection option 1-3: ");
	
			fgets(buffer, sizeof(buffer), stdin);
			sendMessage(sock_fd, buffer);
			input = atoi(buffer);
			
			if ((input > 3) || (input < 1)){
				printf("please enter a number within the range supplied \n");
			}
		
		} while ((input > 3) || (input < 1));
		
		return input;
}