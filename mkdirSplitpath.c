#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){
    char baseName[64] = {0};
    char dirName[256] = {0};

    if (pathName == NULL || strlen(pathName) == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    struct NODE* parent = splitPath(pathName, baseName, dirName);
    if (parent == NULL) {
        return;
    }

    if (strlen(baseName) == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    struct NODE* current = parent->childPtr;
    while (current != NULL) {
        if (strcmp(current->name, baseName) == 0 && current->fileType == 'D') {
            printf("MKDIR ERROR: directory %s already exists\n", baseName);
            return;
        }
        current = current->siblingPtr;
    }

    struct NODE* newNode = (struct NODE*)malloc(sizeof(struct NODE));
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
        current = parent->childPtr;
        while (current->siblingPtr != NULL) {
            current = current->siblingPtr;
        }
        current->siblingPtr = newNode;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", baseName);
    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){
char temp[256];
    char* lastSlash;
    char* token;
    struct NODE* current;
    struct NODE* child;

    if (pathName == NULL || strlen(pathName) == 0) {
        return NULL;
    }

    strcpy(temp, pathName);

    lastSlash = strrchr(temp, '/');

    if (lastSlash == NULL) {
        strcpy(baseName, temp);
        strcpy(dirName, "");
        return cwd;
    }

    if (lastSlash == temp) {
        strcpy(baseName, lastSlash + 1);
        strcpy(dirName, "/");
        return root;
    }

    strcpy(baseName, lastSlash + 1);
    *lastSlash = '\0';
    strcpy(dirName, temp);

    if (dirName[0] == '/') {
        current = root;
    } else {
        current = cwd;
    }

    token = strtok(dirName, "/");
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
