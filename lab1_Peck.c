#include "commands.h"         

int main(void) {
	initialize();
	// other initialization as needed
	char userInput[71];
	char* command, *arg;
	int commandIndex;
	bool running = 1;
	while(running) {
		printf("Enter command: ");
		// complete implementations
		fgets(userInput, sizeof(userInput), stdin);
		command = strtok(userInput, " \n");
		commandIndex = find_command(command);
		while (commandIndex == -1){
			printf("Command Not Found!\nEnter command: \n");
			fgets(userInput, sizeof(userInput), stdin);
			command = strtok(userInput, " \n");
			commandIndex = find_command(command);
		}
		arg = strtok(NULL, "\n");
		int debug = 0;
		switch(commandIndex){
			case 0:
			 	debug = mkdir(arg);
				printf("Debug: %d\n",(debug));
				break;
			case 1:
				debug = rmdir(arg);
				printf("Debug: %d\n",(debug));
				break;
			case 2:
				// debug = ls(arg);
				// printf("Debug: %d\n",(debug));
				break;
			case 3:
				// debug = cd(arg);
				// printf("Debug: %d\n",(debug));
				break;
			case 4:
				// debug = pwd(arg);
				// printf("Debug: %d\n",(debug));
				break;
			case 5:
				// debug = creat(arg);
				// printf("Debug: %d\n",(debug));
				break;
			case 6:
				// debug = rm(arg);
				// printf("Debug: %d\n",(debug));
				break;
			case 7:
				// debug = reload(arg);
				// printf("Debug: %d\n",(debug));
				break;
			case 8:
				// debug = save(arg);
				// printf("Debug: %d\n",(debug));
				break;
			case 9:
				printf("Exiting File System...\n");
				running = 0;
				break;
		}
		arg = NULL;
	}
	return 0;
}

