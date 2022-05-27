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
        ~DataBlock();
        int addDirectoryEntry(DirectoryEntry directoryEntry);
        int removeDirectoryEntry(int inodeAddress);
        DirectoryEntry* getDirectoryEntries();
        int getDirectoryEntriesSize();
        int setEmptyDataBlock(int blockSize);
        int setDirectoryEntriesSize(int size);
        int size();
        int isEmpty();
        int isDirectory();
        DirectoryEntry getDirectoryEntry();
        int setData(string data);
        string getData();
    private:
        void init();
     
        int directoryEntriesSize;
        DirectoryEntry* directoryEntries;
        string data;
};

#endif