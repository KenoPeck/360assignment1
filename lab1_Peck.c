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
		switch(commandIndex){
			case 0:
			 	int debug = mkdir(arg);
				printf("Debug: %d\n",(debug));
				break;
			case 9:
				printf("Exiting File System...\n");
				running = 0;
				break;
		}
	}
	return 0;
}

