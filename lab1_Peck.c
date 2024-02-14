#include "commands.h"         

int find_command(char *user_command) {
	int i = 0;
	while(cmd[i]){
		if (strcmp(user_command, cmd[i])==0)
		return i;
		i++;
	}
	return -1;
}

int initialize(void) {
	root = (NODE *)malloc(sizeof(NODE));
	strcpy(root->name, "/");
	root->parent = root;
	root->Ysibling = 0;
	root->Osibling = 0;
	root->child = 0;
	root->type = 'D';
	cwd = root;
	printf("Filesystem initialized!\n");
}

int main(void) {
	initialize();
	// other initialization as needed

	while(1) {
		printf("Enter command: ");
		// complete implementations

		break;
	}
}

