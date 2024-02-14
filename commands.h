#ifndef commands_h;

#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>            
#include <stdlib.h>   
#include <string.h>
// you can use additional headers as needed

typedef struct node {
	char  name[64];       // node's name string
	char  type;
	struct node *child, *Ysibling, *Osibling, *parent;
} NODE;


NODE *root; 
NODE *cwd;
char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm",
"reload", "save", "quit", 0};  // fill with list of commands
// other global variables

int mkdir(char* pathway);
NODE* search(NODE* mRoot, char* target);

#endif;