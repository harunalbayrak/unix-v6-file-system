#ifndef _DATABLOCK_H_
#define _DATABLOCK_H_

#include <fstream>
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
        int addAddress(uint16_t address);
        int removeDirectoryEntry(int inodeAddress);
        int setEmptyDataBlock(int blockSize);
        int setDirectoryEntriesSize(int size);
        int setData(string data);
        DirectoryEntry* getDirectoryEntries();
        int getDirectoryEntriesSize();
        uint16_t* getAddresses();
        int getAddressesSize();
        string getData();
        int getDataSize();
        int serialize(FILE *fptr, bool bWrite);
        // int serialize(fstream &fs, bool bWrite);
        
    private:
        void init();
        int addressesSize;
        uint16_t* addresses;
        int directoryEntriesSize;
        DirectoryEntry* directoryEntries;
        int dataSize;
        string data;
};

#endif