#include <arpa/inet.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <unistd.h>
#include <errno.h>

	#define MAXDATASIZE 100
	#define ARRAY_SIZE 30  /* Size of array to receive */
	#define BACKLOG 10     /* how many pending connections queue will hold */
	#define RETURNED_ERROR -1
	#define DEFAULT_PORT_NO 12345
	#define MAX_USERS 10

	
//Define Functions
void debug_printuserlist(); // e.g. debug_printuserlist(&userlist, userCount);
void getMessage(int sock_fd, char buffer[]); // e.g. (sock_fd, buffer);
void sendMessage(int sock_fd, char * message);

typedef struct {
   char *username;
   char *password;
   int  games_played;
   int   games_won;
} User;

typedef struct {
   char *type;
   char *object;
} Word_pair;

//Begin Program
int main(int argc, char *argv[]){
	
	/* Definitions */
	int sock_fd, new_fd;  /* listen on sock_fd, new connection on new_fd */
	struct sockaddr_in my_addr;    /* my address information */
	struct sockaddr_in their_addr; /* connector's address information */
	socklen_t sin_size;
	
	char buffer[1024] = "0";
	
	User userlist[MAX_USERS];
	int userCount = 0;
	
	Word_pair words[500];
	int numWords = 0;
	
	FILE *file;
	const char* authFilename = "Authentication.txt";
	const char* wordsFilename = "hangman_text.txt";
	
//-------------------------------------------------------------------	
// ***Import hangman word array

	file = fopen(wordsFilename, "r");
	if (file == NULL){
        printf("Could not open file %s",authFilename);
        return 1;
    }
	int wordPairCounter = 0;
	while (fgets(buffer, sizeof(buffer), file) != NULL){
		words[wordPairCounter].type = malloc(20*sizeof(char));
		words[wordPairCounter].object = malloc(20*sizeof(char));
		sscanf( buffer, "%[^,],%s ", words[wordPairCounter].object, words[wordPairCounter].type);
		//printf("[%s],[%s] \n", words[wordPairCounter].type, words[wordPairCounter].object);
		wordPairCounter++;
	}
	 fclose(file);
	 numWords = wordPairCounter;
	 memset(&buffer[0], 0, sizeof(buffer));
	
//-------------------------------------------------------------------
// import users
    file = fopen(authFilename, "r");
    if (file == NULL){
        printf("Could not open file %s",authFilename);
        return 1;
    }
	int currentUser = -1; //-1 to prevent header
	while (fgets(buffer, sizeof(buffer), file) != NULL){
		if (currentUser < 0){ //skip header line
			currentUser++;
			continue;
		}
		userlist[currentUser].username = malloc(20*sizeof(char));
		userlist[currentUser].password = malloc(20*sizeof(char));
		sscanf( buffer, "%s %s", userlist[currentUser].username,
			userlist[currentUser].password);
		userlist[currentUser].games_played = 0;
		userlist[currentUser].games_won = 0;
		currentUser++;
	}
	fclose(file);
	userCount = currentUser;
	memset(&buffer[0], 0, sizeof(buffer));
	
	debug_printuserlist(&userlist, userCount);
	
//------------------------------------------------------------------
// generate socket & listen

	/* generate the socket */
	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* generate the end point */
	my_addr.sin_family = AF_INET;         /* host byte order */
	if (argc < 2){
		my_addr.sin_port = htons(DEFAULT_PORT_NO); /* short, network byte order */
	} else{
		my_addr.sin_port = htons(atoi(argv[1]));
	}
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
		/* bzero(&(my_addr.sin_zero), 8);   ZJL*/     /* zero the rest of the struct */

	/* bind the socket to the end point */
	if (bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) \
	== -1) {
		perror("bind");
		exit(1);
	}

	/* start listnening */
	if (listen(sock_fd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}
	printf("server starts listnening ...\n");
	
//-----------------------------------------------------------------------------
// toplevel client progress loop

	while(1) {  
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sock_fd, (struct sockaddr *)&their_addr, \
		&sin_size)) == -1) {
			perror("accept");
			continue;
		}
		printf("server: got connection from %s\n", \
			inet_ntoa(their_addr.sin_addr));
		
		if (!fork()) { //*** thread this currently forked from copy paste
		
			
		
		
			//Client Connected & Accepted
			
			char *username;
			char *password;
			
			username = malloc(15 * sizeof(char));
			password = malloc(15 * sizeof(char));
			
			//recv username
			if (recv(new_fd, buffer, 10,0) == -1) {
				perror("recv");
				exit(1);
			}
			sscanf(buffer, "%s", username);
			
			//recv password
			if (recv(new_fd, buffer, 10,0) == -1) {
                perror("recv");
				exit(1);
            }
			sscanf(buffer, "%s", password);
			
			
			printf(",%s,%s, \n", username, password);
			printf(",%s,%s, \n", userlist[0].username, userlist[0].password);
			
			//check if user registered
			int authFailed = 1;
			for (int i = 0; i < userCount; i++ ){		
				if (strcmp(username, userlist[i].username) == 0 && strcmp(password, userlist[i].password) == 0){
					printf("user found & pass correct \n");
					authFailed = 0;
					break;
				}
			}
			
			//send response to client
			char * message = malloc(20);
			if (authFailed == 1){
				printf("failed auth, quitting \n");
				message = "authFail";
			} else {
				message = "authPass";
			}
			sendMessage(new_fd, message);
			
		
		//***hangman title
		
		
		//***leaderboard
		// compareto
			
			// while swapped
			// swapped = false
				// for user in userlist
				// compare with previous
					
		
		
		//***hangman game loop
		// {
		// l
		
		// elephant
		// _l___a___
		 // add l to guess list
		
		// for _
			// if no _
				// you won
				// wins + 1
				// break
			
		// send buf
		// +1 guesses
		// }
		// if guesses lef < 0
			// send you lost
			
			
			close(new_fd); // connection close
			exit(0);
		}

		while(waitpid(-1,NULL,WNOHANG) > 0); // clean up threading here
	}
}

void debug_printuserlist(User *userlist, int userCount){
	int currentUser = 0;
	while(currentUser < userCount){
		printf("[%s]  [%s]	[%d][%d] \n", userlist[currentUser].username, userlist[currentUser].password,
			userlist[currentUser].games_played, userlist[currentUser].games_won);
		currentUser++;
	}
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
