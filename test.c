//Solution to Prac 7
//Maolin Tang
//10/09/2013
// Modified Anthony Gough - gets deprecated - fgets() implemented Sept 2015

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define m 3   // m is the number of resources
#define n 5  // n is the number of processes

void user_menu() 
{
	int counter = 0;
	char buffer[1024];
	int input;
	int guesses_left = 10;
	char guessed_letters[10] = "";
	char my_string[] = "elephant";
	int temp[10];
	int word_size;
	
	do
	{
		printf("----- Enter your choice: -----\n\n");
		printf("1. Play Hangman\n");
		printf("2. Show Leaderboard\n");
		printf("0. Exit\n\n");
		printf("Enter your choice [0-2]:");
	
		fgets(buffer, sizeof(buffer), stdin);
		input = atoi(buffer);
		
	} while ((input > 2) || (input < 0));

		
	switch(input) 
	{
		case 1:
			word_size = strlen(my_string);
			for (int i = 0; i < word_size; i++) {
				temp[i] = 0;
			}
			while (guesses_left > 0) {
				for (int i = 0; i<sizeof(guessed_letters)/sizeof(char); i++){
					printf("%c", guessed_letters[i]);
				}
				printf("\nNumber of guesses left: %d\n", guesses_left);
				for (int i = 0; i < word_size; i++){
					if (temp[i] == 1) {
						printf("%c", my_string[i]);
					} else {
						printf("_");
					}
				}
				/* for (int i = 0; i < sizeof(my_string)/sizeof(char); i++){
					if (*buffer == my_string[i]){
						//printf("%c", *buffer);
						temp[i]=1;
					}else {
						//printf("-");
						temp[i]="-";
					}
				} */
				/* for (int i = 0; i<sizeof(temp)/sizeof(char); i++){
					printf("%c", temp[i]);
				} */
				printf("\nEnter guess:");
				fgets(buffer, sizeof(buffer), stdin);
				guesses_left--;
				guessed_letters[counter]=*buffer;
				counter++;
				for (int i = 0; i < sizeof(my_string)/sizeof(char); i++){
					if (*buffer == my_string[i]){
						//printf("%c", *buffer);
						temp[i]=1;
					}
				}
				
			}
			break;
		case 2:
			printf("Selection 2");
			break;
		case 0:
			exit(0);
	
	}
}

int main(int argc, char **argv)
{
	printf("\n\nWelcome to Hangman:\n\n");
	
	while (1) 
	{
		user_menu();
	}
	return 0;
}