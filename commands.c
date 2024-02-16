#include "commands.h"

NODE *root; 
NODE *cwd;
char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm",
"reload", "save", "quit", "DEBUG", 0};  // fill with list of commands
// other global variables

void initialize() {
	root = (NODE *)malloc(sizeof(NODE));
	strcpy(root->path, "/");
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
                strcpy(temp, currentDir->path);
                if (strcmp(currentDir->path, "/") != 0){ // making sure current directory is not root
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
                    strcpy(newDir->path, pathname);
                }
                else{
                    strcpy(newDir->path, temp); // adding cwd's path to the beginning of new directory's path
                }
                strcpy(newDir->name, baseName);
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
        strcpy(temp, currentDir->path);
        if (strcmp(currentDir->path, "/") != 0){
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
            strcpy(newDir->path, temp); // adding cwd's path to the beginning of new directory's path
            strcpy(newDir->name, baseName);
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
        if(strcmp(mRoot->path, target) == 0){ // checking if mRoot has target pathname
            return mRoot;
        }
        else{
            NODE* currentDir = mRoot;
            while(currentDir->Ysibling){ // checking that currentDir has a younger sibling
                currentDir = currentDir->Ysibling;
                if(strcmp(currentDir->path, target) == 0){ // checking if new currentDir has target pathname
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

/*
Function name: rmdir
Description: remove a directory (either absolute or relative)
Return type: int (1 = success, 0 = failure)
Input Parameters: char*pathname (pathname to be removed)
*/
int rmdir(char* pathname){
    if (pathname == NULL){
        printf("ERROR - rmdir requires a pathname argument to work\n");
        return 0;
    }
    NODE* currentDir;
    if(pathname[0] == '/'){ // if pathname is absolute
        currentDir = search(root, pathname);
        if (currentDir == NULL){
            printf("ERROR - %s does not exist\n",(pathname));
            return 0;
        }
    }
    else{ // if pathname is relative
        char pathnameWithSlash[64];
        strcpy(pathnameWithSlash,cwd->path);
        if(cwd != root){
            strcat(pathnameWithSlash, "/"); // add '/'
        }
        strcat(pathnameWithSlash, pathname);
        currentDir = search(cwd, pathnameWithSlash);
        if (currentDir == NULL){
            printf("ERROR - %s does not exist\n",(pathname));
            return 0;
        }
    }
    if (currentDir->type == 'F'){
        printf("ERROR - %s is not a directory!\n",(pathname));
        return 0;
    }
    else if (currentDir->child){
        printf("ERROR - %s is not empty!\n",(pathname));
        return 0;
    }
    else{
        NODE* parentDir = currentDir->parent; // update other file statuses
        NODE* oSibling = currentDir->Osibling;
        NODE* ySibling = currentDir->Ysibling;
        if(parentDir->child == currentDir){
            parentDir->child = ySibling;
        }
        if(oSibling){
            oSibling->Ysibling = ySibling;
        }
        if(ySibling){
            ySibling->Osibling = oSibling;
        }
        free(currentDir);
        return 1;
    }
}

/*
Function name: creat
Description: create new file
Return type: int (1 = success, 0 = failure)
Input Parameters: char*target (new file pathname)
*/
int creat(char* pathname){
    if (pathname == NULL){
        printf("ERROR - creat requires a pathname argument to work\n");
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
                strcpy(temp, currentDir->path);
                if (strcmp(currentDir->path, "/") != 0){ // making sure current directory is not root
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

            NODE* newFile = (NODE*)malloc(sizeof(NODE)); // allocate new File node
            if (newFile == NULL){ // if memory allocation fails
                printf("Malloc Error\n"); // print Malloc error to console
                return 0;
            }
            else {
                if (dirName[0] == '/'){
                    strcpy(newFile->path, pathname);
                }
                else{
                    strcpy(newFile->path, temp); // adding cwd's path to the beginning of new directory's path
                }
                strcpy(newFile->name, baseName);
                newFile->type = 'F';
                newFile->parent = currentDir;
                newFile->child = NULL;
                newFile->Ysibling = NULL;
                if(currentDir->child == NULL){ // if currentDir is empty
                    currentDir->child = newFile;
                    newFile->Osibling = NULL;
                }
                else{ // if currentDir has at least 1 child
                    currentDir = currentDir->child;
                    while (currentDir->Ysibling){
                        currentDir = currentDir->Ysibling;
                    }
                    currentDir->Ysibling = newFile;
                    newFile->Osibling = currentDir;
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
        strcpy(temp, currentDir->path);
        if (strcmp(currentDir->path, "/") != 0){
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
            strcpy(newDir->path, temp); // adding cwd's path to the beginning of new directory's path
            strcpy(newDir->name, baseName);
            newDir->type = 'F';
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
Function name: rm
Description: remove a file (either absolute or relative)
Return type: int (1 = success, 0 = failure)
Input Parameters: char*pathname (pathname to be removed)
*/
int rm(char* pathname){
    if (pathname == NULL){
        printf("ERROR - rm requires a pathname argument to work\n");
        return 0;
    }
    NODE* currentDir;
    if(pathname[0] == '/'){ // absolute
        currentDir = search(root, pathname);
        if (currentDir == NULL){
            printf("ERROR - %s does not exist\n",(pathname));
            return 0;
        }
    }
    else{ // relative
        char pathnameWithSlash[64] = "/";
        strcpy(pathnameWithSlash,cwd->path);
        if(cwd != root){
            strcat(pathnameWithSlash, "/");
        }
        strcat(pathnameWithSlash, pathname);
        currentDir = search(cwd, pathnameWithSlash);
        if (currentDir == NULL){
            printf("ERROR - %s does not exist\n",(pathname));
            return 0;
        }
    }
    if (currentDir->type == 'D'){
        printf("ERROR - %s is not a FILE!\n",(pathname));
        return 0;
    }
    else{
        NODE* parentDir = currentDir->parent; // update other file's connections
        NODE* oSibling = currentDir->Osibling;
        NODE* ySibling = currentDir->Ysibling;
        if(parentDir->child == currentDir){
            parentDir->child = ySibling;
        }
        if(oSibling){
            oSibling->Ysibling = ySibling;
        }
        if(ySibling){
            ySibling->Osibling = oSibling;
        }
        free(currentDir);
        return 1;
    }
}

/*
Function name: cd
Description: change cwd to a different a directory (either absolute or relative)
Return type: int (1 = success, 0 = failure)
Input Parameters: char*pathname (new directory)
*/
int cd(char* pathname){
    if (pathname == NULL){
        cwd = root;
        return 1;
    }
    else if(strcmp(pathname, "..") == 0){ // going back 1 directory
        if(cwd != root){
            cwd = cwd->parent;
            return 1;
        }
        else{
            return 1;
        }
    }
    else{
        NODE* currentDir;
        if (pathname[0] == '/'){ // absolute
            currentDir = search(root, pathname);
        }
        else{ // relative
            char pathnameWithSlash[64] = "/";
            strcpy(pathnameWithSlash,cwd->path);
            if(cwd != root){
                strcat(pathnameWithSlash, "/");
            }
            strcat(pathnameWithSlash, pathname); // adding '/'
            currentDir = search(cwd, pathnameWithSlash);
        }
        if(currentDir){
            if(currentDir->type == 'D'){
                cwd = currentDir; // updating cwd
                return 1;
            }
            else{
                printf("ERROR - %s is not a DIRECTORY!\n",(pathname));
                return 0;
            }
        }
        else{
            printf("ERROR - %s does not exist!\n",(pathname));
            return 0;
        }
    }
}

/*
Function name: pwd
Description: prints current working directory
Return type: int (1 = success, 0 = failure)
Input Parameters: N/A
*/
int pwd(){
    printf("%s\n",(cwd->path));
    return 1;
}

/*
Function name: ls
Description: list all files/directories in a directory (either absolute or relative)
Return type: int (1 = success, 0 = failure)
Input Parameters: char*pathname (target directory)
*/
int ls(char* pathname){
    if (pathname == NULL){ // listing cwd contents
        NODE* currentDir = cwd->child;
        while(currentDir){
            if(currentDir->type == 'F'){
                printf("FILE    %s\n",(currentDir->name));
            }
            else{
                printf("DIRECTORY    %s\n",(currentDir->name));
            }
            currentDir = currentDir->Ysibling;
        }
        return 1;
    }
    else{ 
        NODE* currentDir;
        if (pathname[0] == '/'){ // absolute path contents
            currentDir = search(root, pathname);
        }
        else{ // relative
            char pathnameWithSlash[64] = "/";
            strcpy(pathnameWithSlash,cwd->path);
            if(cwd != root){
                strcat(pathnameWithSlash, "/");
            }
            strcat(pathnameWithSlash, pathname);
            currentDir = search(cwd, pathnameWithSlash); // adding '/'
        }
        if(currentDir){
            currentDir = currentDir->child;
            while(currentDir){ // printing all contents
                if(currentDir->type == 'F'){
                    printf("FILE    %s\n",(currentDir->name));
                }
                else{
                    printf("DIRECTORY    %s\n",(currentDir->name));
                }
                currentDir = currentDir->Ysibling;
            }
            return 1;
        }
        else{
            printf("ERROR - %s does not exist!\n",(pathname));
            return 0;
        }
    }
}

/*
Function name: save
Description: saves current filesystem to a file with name filename
Return type: int (1 = success, 0 = failure)
Input Parameters: char*filename (name of file to save to)
*/
int save(char* filename){
    if(filename == NULL){
        printf("ERROR - save requires a filename argument to work\n");
        return 0;
    }
    if(root->child){
        FILE *fp = fopen(filename, "w+"); // open a file stream        
        saveHelper(fp, filename, root->child); // helper function
        fclose(fp); // close file stream when done
    }
    return 1;
}

/*
Function name: savehelper
Description: recursive helper function for save() that saves current file and all children
Return type: N/A
Input Parameters: FILE* fp (file being worked on), char* filename (name of file), NODE* currentDir (current file node)
*/
void saveHelper(FILE* fp, char* filename, NODE* currentDir){
    if(currentDir == NULL){
        return;
    }
    fprintf(fp, "%c %s\n", currentDir->type, currentDir->path); // print a line to file
    saveHelper(fp, filename, currentDir->child); // save self
    NODE* tempDir = currentDir;
    while(tempDir->Ysibling){ // save children
        tempDir = tempDir->Ysibling;
        fprintf(fp, "%c %s\n", tempDir->type, tempDir->path); // print a line to file
        saveHelper(fp, filename, tempDir->child);
    }
}

/*
Function name: reload
Description: reinitializes file system with files saved in filename
Return type: int (1 = success, 0 = failure)
Input Parameters: char*filename (file with filesystem saved)
*/
int reload(char* filename){
    if(filename == NULL){
        printf("ERROR - load requires a filename argument to work\n");
        return 0;
    }
    clearFiles(root->child); // resetting tree to be refilled
    FILE *fp = fopen(filename, "r"); // open a file stream
    char savedType, savedPath[128];
    while(fscanf(fp, "%c %s", &savedType, savedPath) != EOF){
        if(savedType == 'D'){
            mkdir(savedPath); // adding directory
        }
        else{
            creat(savedPath); // adding file
        }
        fgetc(fp);
    }
    fclose(fp); // close file stream when done
    return 1;
}

/*
Function name: clearFiles
Description: recursive helper function for resetting filesystem in reload()
Return type: N/A
Input Parameters: NODE*mroot (current File to be removed)
*/
void clearFiles(NODE* mroot){ // depth-first deletion
    if(mroot == NULL){
        return;
    }
    if(mroot->child){
        clearFiles(mroot->child); // clear child
    }
    NODE* next2 = mroot->Ysibling, *next; // nodes for finding youngest child then deleting backwards from there
    if(next2){
        while(next2->Ysibling){
            next2 = next2->Ysibling;
        }
    }
    while(next2){
        next = next2->Osibling;
        clearFiles(next2);
        if(next != mroot){
            next2 = next;
        }
        else{
            next2 = NULL;
        }
    }
    if (mroot->type == 'D'){ // deleting directory
        rmdir(mroot->path);
    }
    else{
        rm(mroot->path); // deleting file
    }
}