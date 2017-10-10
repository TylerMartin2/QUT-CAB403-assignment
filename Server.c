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


	#define ARRAY_SIZE 30  /* Size of array to receive */
	#define BACKLOG 10     /* how many pending connections queue will hold */
	#define RETURNED_ERROR -1
	#define PORT_NO 12345
	#define MAX_USERS 10

	
//Define Functions
void debug_printuserlist(); // give &userlist as arguement

typedef struct {
   char *username;
   char *password;
   int  games_played;
   int   games_won;
} User; 

//Begin Program
int main(int argc, char *argv[]){
	
	/* Definitions */
	int sockfd, new_fd;  /* listen on sock_fd, new connection on new_fd */
	struct sockaddr_in my_addr;    /* my address information */
	struct sockaddr_in their_addr; /* connector's address information */
	socklen_t sin_size;
	
	char buffer[1024] = "0";
	User userlist[MAX_USERS];
	
	FILE *file;
	const char* authFilename = "Authentication.txt";
	const char* wordsFilename = "hangman_text.txt";
//-------------------------------------------------------------------	
// ***Import hangman word array


	
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
		currentUser++;
	}
    fclose(file);
	
	//debug
	//debug_printuserlist(&userlist);
	
//------------------------------------------------------------------
// generate socket & listen

	/* generate the socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* generate the end point */
	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(PORT_NO);     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
		/* bzero(&(my_addr.sin_zero), 8);   ZJL*/     /* zero the rest of the struct */

	/* bind the socket to the end point */
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) \
	== -1) {
		perror("bind");
		exit(1);
	}

	/* start listnening */
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}
	printf("server starts listnening ...\n");
	
//-----------------------------------------------------------------------------
// toplevel client progress loop

	while(1) {  
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, \
		&sin_size)) == -1) {
			perror("accept");
			continue;
		}
		printf("server: got connection from %s\n", \
			inet_ntoa(their_addr.sin_addr));
		
		if (!fork()) { //*** thread this currently forked from copy paste
		
		
			//Client Connected & Accepted
		
			if (send(new_fd, "Enter Username: ", 16, 0) == -1)
				perror("username send failure");
			
			if (recv(new_fd, buffer, sizeof(buffer),0) == -1) {
                        perror("recv");
                        exit(1);
                }
			printf("Server:Msg Received %s\n", buffer);
			
			//check if user registered
			
			
			if (send(new_fd, "Enter Password: ", 16, 0) == -1)
				perror("Password send failure");
			
			if (recv(new_fd, buffer, sizeof(buffer),0) == -1) {
                        perror("recv");
                        exit(1);
                }
			printf("Server:Msg Received %s\n", buffer);
		
		//***login accepted
		
		//***login failed
		
		//***hangman title
		
		
		//***leaderboard
		
		//***hangman game loop
		
		
			
			close(new_fd); // connection close
			exit(0);
		}

		while(waitpid(-1,NULL,WNOHANG) > 0); // clean up threading here
	}
}

void debug_printuserlist(User *userlist){
	int currentUser = 0;
	while(currentUser < MAX_USERS){
		printf("[%s]  [%s]	[%d][%d] \n", userlist[currentUser].username, userlist[currentUser].password,
			userlist[currentUser].games_played, userlist[currentUser].games_won);
		currentUser++;
	}
}