#ifndef commands_h;

#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>            
#include <stdlib.h>   
#include <string.h>
#include <stdbool.h>
// you can use additional headers as needed

typedef struct node {
	char  name[64];       // node's name string
	char  type;
	struct node *child, *Ysibling, *Osibling, *parent;
} NODE;

void initialize(void);
int find_command(char *user_command);
int mkdir(char* pathname);
NODE* search(NODE* mRoot, char* target);

#endif;