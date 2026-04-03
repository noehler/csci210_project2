#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){
    char baseName[64];
    char dirName[256];
    struct NODE *parent, *temp, *newNode;

    baseName[0] = '\0';
    dirName[0] = '\0';

    if (pathName == NULL || strcmp(pathName, "/") == 0 || strlen(pathName) == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    parent = splitPath(pathName, baseName, dirName);
    if (parent == NULL) {
        return;
    }

    temp = parent->childPtr;
    while (temp != NULL) {
        if (strcmp(temp->name, baseName) == 0 && temp->fileType == 'D') {
            printf("MKDIR ERROR: directory %s already exists\n", pathName);
            return;
        }
        temp = temp->siblingPtr;
    }

    newNode = (struct NODE*)malloc(sizeof(struct NODE));
    if (newNode == NULL) {
        return;
    }

    strcpy(newNode->name, baseName);
    newNode->fileType = 'D';
    newNode->childPtr = NULL;
    newNode->siblingPtr = NULL;
    newNode->parentPtr = parent;

    if (parent->childPtr == NULL) {
        parent->childPtr = newNode;
    } else {
        temp = parent->childPtr;
        while (temp->siblingPtr != NULL) {
            temp = temp->siblingPtr;
        }
        temp->siblingPtr = newNode;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){
    char tempDir[256];
    char tempPath[256];
    char *lastSlash;
    char *token;
    struct NODE *current, *child;

    if (pathName == NULL) {
        return NULL;
    }

    strcpy(tempPath, pathName);

    if (strcmp(tempPath, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    lastSlash = strrchr(tempPath, '/');

    if (lastSlash == NULL) {
        strcpy(dirName, "");
        strcpy(baseName, tempPath);
        return cwd;
    }

    if (lastSlash == tempPath) {
        strcpy(dirName, "/");
        strcpy(baseName, lastSlash + 1);
        return root;
    }

    *lastSlash = '\0';
    strcpy(dirName, tempPath);
    strcpy(baseName, lastSlash + 1);

    if (dirName[0] == '/') {
        current = root;
    } else {
        current = cwd;
    }

    strcpy(tempDir, dirName);
    token = strtok(tempDir, "/");

    while (token != NULL) {
        child = current->childPtr;

        while (child != NULL) {
            if (strcmp(child->name, token) == 0 && child->fileType == 'D') {
                break;
            }
            child = child->siblingPtr;
        }

        if (child == NULL) {
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }

        current = child;
        token = strtok(NULL, "/");
    }

    return current;
}