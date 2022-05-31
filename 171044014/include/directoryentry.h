#ifndef _DIRECTORYENTRY_H_
#define _DIRECTORYENTRY_H_

#include <iostream>

#define MAX_NAME_SIZE 14

class DirectoryEntry{
    public:
        DirectoryEntry();
        DirectoryEntry(DirectoryEntry* dir);
        int getInodeAddress();
        char* getFileName();
        void setInodeAddress(int n);
        void setFileName(const char* fileName);
        int isEmpty();
    private:
        uint16_t inodeAddress;
        char fileName[MAX_NAME_SIZE];
};

#endif