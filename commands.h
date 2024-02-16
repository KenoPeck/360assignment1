#ifndef commands_h

#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>            
#include <stdlib.h>   
#include <string.h>
#include <stdbool.h>
// you can use additional headers as needed

typedef struct node {
	char  name[64];       // node's name string
	char  path[128];       // node's path string
	char  type;
	struct node *child, *Ysibling, *Osibling, *parent;
} NODE;

void initialize(void);
int find_command(char *user_command);
int mkdir(char* pathname);
NODE* search(NODE* mRoot, char* target);
int rmdir(char* pathname);
int creat(char* pathname);
int rm(char* pathname);
int cd(char* pathname);
int pwd();
int ls(char* pathname);
int save(char* filename);
void saveHelper(FILE* fp, char* filename, NODE* currentDir);
int reload(char* filename);
void clearFiles(NODE* mroot);


#endif