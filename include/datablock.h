#ifndef _DATABLOCK_H_
#define _DATABLOCK_H_

#include "directoryentry.h"
#include "inode.h"

using namespace std;

class DataBlock {
    public:
        DataBlock();
        DataBlock(int blockSize);
        DataBlock(int blockSize, I_Type type);
        int size();
        int isEmpty();
        int isDirectory();
        int addDirectoryEntry(DirectoryEntry directoryEntry);
        int removeDirectoryEntry(int inodeAddress);
        int setEmptyDataBlock(int blockSize);
        int setDirectoryEntriesSize(int size);
        int setData(string data);
        DirectoryEntry* getDirectoryEntries();
        int getDirectoryEntriesSize();
        string getData();
    private:
        void init();
        int directoryEntriesSize;
        DirectoryEntry* directoryEntries;
        string data;
};

#endif