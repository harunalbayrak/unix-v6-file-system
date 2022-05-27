#ifndef _DIRECTORYENTRY_H_
#define _DIRECTORYENTRY_H_

#define MAX_NAME_SIZE 30
class DirectoryEntry{
    public:
        DirectoryEntry();
        int getInodeAddress();
        char* getDirectoryName();
        void setInodeAddress(int n);
        void setDirectoryName(const char* directoryName);
    private:
        int inodeAddress;
        char directoryName[MAX_NAME_SIZE];
};

#endif