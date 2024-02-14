#include "commands.h"

int mkdir(char* pathname){
    char* baseName;
    char* dirName;
    int pathLen = strlen(pathname) - 1; // setting pathLen to length of target pathname - 1
    for (int i = pathLen; i >= 0; i--){ // iterating through pathname to find '/'
        if (pathname[i] == '/'){ // if '/' found
            strcpy(baseName, &pathname[i+1]); // set baseName to characters after '/'
            strncpy(dirName, pathname, i+1); // set dirName to characters before '/'
            dirName[i] = '\0'; // change '/' character to '\0' to separate the two
            break;
        }
        else if (i == 0){ // if pathname is relative with no parent directories
            strcpy(dirName, "\0");
            strcpy(baseName, pathname);
        }
    }
    if (dirName[0] != '\0'){ // if path contains more than one DIR
        NODE* currentDir;
        if (dirName[0] == '/'){ // if pathname is absolute
            currentDir = search(root, dirName);
        }
        else{ // if pathname is relative
            currentDir = search(cwd, dirName);
        }
        if(currentDir && currentDir->type == 'D'){ // if dirName exists and is a directory
            if (search(currentDir, pathname)){
                printf("Error - DIR %s already exists!", (pathname)); // print error message if directory already exists
                return 0;
            }
            NODE* newDir = (NODE*)malloc(sizeof(NODE)); // allocate new directory node
            if (newDir == NULL){ // if memory allocation fails
                printf("Malloc Error"); // print Malloc error to console
                return 0;
            }
            else {
                strcpy(newDir->name, pathname);
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
        else if (currentDir == NULL){
            printf("Error - DIR %s does not exist!", (dirName));
            return 0;
        }
        else{
            printf("Error - %s is a FILE not a DIRECTORY!",(dirName));
            return 0;
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