#include "commands.h"         

int main(void) {
	initialize();
	// other initialization as needed
	char userInput[71];
	char* command, *arg;
	int commandIndex;
	bool running = 1;
	int debug, debugOn = 0;
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
		debug = 0;
		switch(commandIndex){
			case 0:
			 	debug = mkdir(arg);
				if (debugOn){
					printf("Debug: %d\n",(debug));
				}
				break;
			case 1:
				debug = rmdir(arg);
				if (debugOn){
					printf("Debug: %d\n",(debug));
				}
				break;
			case 2:
				debug = ls(arg);
				if (debugOn){
					printf("Debug: %d\n",(debug));
				}
				break;
			case 3:
				debug = cd(arg);
				if (debugOn){
					printf("Debug: %d\n",(debug));
				}
				break;
			case 4:
				debug = pwd();
				if (debugOn){
					printf("Debug: %d\n",(debug));
				}
				break;
			case 5:
				debug = creat(arg);
				if (debugOn){
					printf("Debug: %d\n",(debug));
				}
				break;
			case 6:
				debug = rm(arg);
				if (debugOn){
					printf("Debug: %d\n",(debug));
				}
				break;
			case 7:
				debug = reload(arg);
				if (debugOn){
					printf("Debug: %d\n",(debug));
				}
				break;
			case 8:
				debug = save(arg);
				if (debugOn){
					printf("Debug: %d\n",(debug));
				}
				break;
			case 9:
				printf("Exiting File System...\n");
				save("fssim_peck.txt");
				running = 0;
				break;
			case 10:
				if (debugOn){
					printf("Debug mode is now OFF\n");
					debugOn = 0;
				}
				else{
					printf("Debug mode is now ON\n");
					debugOn = 1;
				}
		}
		arg = NULL;
	}
	return 0;
}

