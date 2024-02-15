#include "commands.h"

NODE *root; 
NODE *cwd;
char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm",
"reload", "save", "quit", 0};  // fill with list of commands
// other global variables

void initialize() {
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

int find_command(char *user_command) {
	int i = 0;
	while(cmd[i]){
		if (strcmp(user_command, cmd[i])==0)
		return i;
		i++;
	}
	return -1;
}

/*
Function name: mkdir
Description: create new directory
Return type: int (1 = success, 0 = failure)
Input Parameters: char*target (new directory pathname)
*/
int mkdir(char* pathname){
    if (pathname == NULL){
        printf("ERROR - mkdir requires a pathname argument to work\n");
        return 0;
    }
    char baseName[64];
    char dirName[64];
    int pathLen = strlen(pathname) - 1; // setting pathLen to length of target pathname - 1
    for (int i = pathLen; i >= 0; i--){ // iterating through pathname to find '/'
        if (pathname[i] == '/'){ // if '/' found
            strcpy(baseName, &pathname[i+1]); // set baseName to characters after '/'
            strncpy(dirName, pathname, (i+1)); // set dirName to characters before '/'
            if (i != 0){
                dirName[i] = '\0'; // change '/' character to '\0' to separate the two
            }
            break;
        }
        else if (i == 0){ // if pathname is relative with no parent directories
            strcpy(dirName, "\0");
            strcpy(baseName, pathname);
            break;
        }
    }
    if (dirName[0] != '\0'){ // if path contains more than one DIR
        NODE* currentDir;
        if (dirName[0] == '/'){ // if pathname is absolute
            currentDir = search(root, dirName);
        }
        else{ // if pathname is relative
            char dirNameWithSlash[64] = "/";
            strcat(dirNameWithSlash, dirName); // adding '/' to beginning of pathname's parent directory
            currentDir = search(cwd, dirNameWithSlash);
        }
        if(currentDir && currentDir->type == 'D'){ // if dirName exists and is a directory
            char temp[64];
            if (dirName[0] != '/'){ // if pathname relative
                strcpy(temp, currentDir->name);
                if (strcmp(currentDir->name, "/") != 0){ // making sure current directory is not root
                    strcat(temp, "/");
                }
                strcat(temp, baseName); // combining pathname with parent directory name
                if (search(currentDir, temp)){
                    printf("Error - DIR %s already exists!\n", (pathname)); // print error message if directory already exists
                    return 0;
                }
            }
            else{
                if (search(currentDir, pathname)){
                    printf("Error - DIR %s already exists!\n", (pathname)); // print error message if directory already exists
                    return 0;
                }
            }

            NODE* newDir = (NODE*)malloc(sizeof(NODE)); // allocate new directory node
            if (newDir == NULL){ // if memory allocation fails
                printf("Malloc Error\n"); // print Malloc error to console
                return 0;
            }
            else {
                if (dirName[0] == '/'){
                    strcpy(newDir->name, pathname);
                }
                else{
                    strcpy(newDir->name, temp); // adding cwd's path to the beginning of new directory's path
                }
                newDir->type = 'D';
                newDir->parent = currentDir;
                newDir->child = NULL;
                newDir->Ysibling = NULL;
                if(currentDir->child == NULL){ // if currentDir is empty
                    currentDir->child = newDir;
                    newDir->Osibling = NULL;
                }
                else{ // if currentDir has at least 1 child
                    currentDir = currentDir->child;
                    while (currentDir->Ysibling){
                        currentDir = currentDir->Ysibling;
                    }
                    currentDir->Ysibling = newDir;
                    newDir->Osibling = currentDir;
                }
                return 1;
            }
        }
        else if (currentDir == NULL){
            printf("Error - DIR %s does not exist!\n", (dirName));
            return 0;
        }
        else{
            printf("Error - %s is a FILE not a DIRECTORY!\n",(dirName));
            return 0;
        }
    }
    else{ // if pathname contains no parent directories
        NODE* currentDir;
        currentDir = cwd;
        char temp[64];
        strcpy(temp, currentDir->name);
        if (strcmp(currentDir->name, "/") != 0){
            strcat(temp, "/");
        }
        strcat(temp, pathname);
        if (search(currentDir, temp)){
            printf("Error - DIR %s already exists!\n", (pathname)); // print error message if directory already exists
            return 0;
        }
        NODE* newDir = (NODE*)malloc(sizeof(NODE)); // allocate new directory node
        if (newDir == NULL){ // if memory allocation fails
            printf("Malloc Error\n"); // print Malloc error to console
            return 0;
        }
        else {
            strcpy(newDir->name, temp); // adding cwd's path to the beginning of new directory's path
            newDir->type = 'D';
            newDir->parent = currentDir;
            newDir->child = NULL;
            newDir->Ysibling = NULL;
            if(currentDir->child == NULL){
                currentDir->child = newDir;
                newDir->Osibling = NULL;
            }
            else{
                currentDir = currentDir->child;
                while (currentDir->Ysibling){
                    currentDir = currentDir->Ysibling;
                }
                currentDir->Ysibling = newDir;
                newDir->Osibling = currentDir;
            }
            return 1;
        }
    }

}

/*
Function name: Search
Description: Breadth-First tree search
Return type: NODE*
Input Parameters: NODE* mRoot (root node of search algorithm), char*target (target pathname)
Returns NODE* with target pathname if it exists in the tree, otherwise returns NULL
*/
NODE* search(NODE* mRoot, char* target){
    if (mRoot){
        if(strcmp(mRoot->name, target) == 0){ // checking if mRoot has target pathname
            return mRoot;
        }
        else{
            NODE* currentDir = mRoot;
            while(currentDir->Ysibling){ // checking that currentDir has a younger sibling
                currentDir = currentDir->Ysibling;
                if(strcmp(currentDir->name, target) == 0){ // checking if new currentDir has target pathname
                    return currentDir;
                }
            }
            currentDir = mRoot;
            NODE* child = search(currentDir->child, target); // recursively calling self on mRoot's child
            if (child){
                return child; // returning Node containing target pathname
            }
            while(currentDir->Ysibling){
                currentDir = currentDir->Ysibling;
                if(currentDir->type == 'D'){
                    child = search(currentDir->child, target); // recursively calling self on younger siblings
                    if (child){
                        return child;
                    }
                }
            }
            return NULL; // returning NULL if no siblings or children of siblings contain target pathname
        }
    }
    else{
        return NULL;
    }
}

int rmdir(char* pathname){
    if (pathname == NULL){
        printf("ERROR - rmdir requires a pathname argument to work\n");
        return 0;
    }
    NODE* currentDir;
    if(pathname[0] == '/'){
        currentDir = search(root, pathname);
        if (currentDir == NULL){
            printf("ERROR - %s does not exist",(pathname));
            return 0;
        }
    }
    else{
        char pathnameWithSlash[64] = "/";
        strcat(pathnameWithSlash, pathname);
        currentDir = search(cwd, pathnameWithSlash);
        if (currentDir == NULL){
            printf("ERROR - %s does not exist",(pathname));
            return 0;
        }
    }
    if (currentDir->type == 'F'){
        printf("ERROR - %s is not a directory!",(pathname));
        return 0;
    }
    else if (currentDir->child){
        printf("ERROR - %s is not empty!",(pathname));
        return 0;
    }
    else{
        NODE* parentDir = currentDir->parent;
        NODE* oSibling = currentDir->Osibling;
        NODE* ySibling = currentDir->Ysibling;
        if(parentDir->child == currentDir){
            parentDir->child = ySibling;
        }
        if(oSibling){
            oSibling->Ysibling = ySibling;
            ySibling->Osibling = oSibling;
        }
        free(currentDir);
        return 1;
    }
    
}