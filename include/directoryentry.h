#ifndef _DIRECTORYENTRY_H_
#define _DIRECTORYENTRY_H_

#include <iostream>

#define MAX_NAME_SIZE 14

class DirectoryEntry{
    public:
        DirectoryEntry();
        int getInodeAddress();
        char* getDirectoryName();
        void setInodeAddress(int n);
        void setDirectoryName(const char* directoryName);
    private:
        uint16_t inodeAddress;
        char directoryName[MAX_NAME_SIZE];
};

#endif