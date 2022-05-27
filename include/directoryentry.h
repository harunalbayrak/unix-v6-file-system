#ifndef _DIRECTORYENTRY_H_
#define _DIRECTORYENTRY_H_

#define MAX_NAME_SIZE 30

typedef struct {
    int inodeAddress = -1;
    char directoryName[MAX_NAME_SIZE];
} DirectoryEntry;

#endif