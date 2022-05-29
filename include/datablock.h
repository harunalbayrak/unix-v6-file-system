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
        int addAddress(uint16_t address);
        int removeDirectoryEntry(int inodeAddress);
        int setEmptyDataBlock(int blockSize);
        int setDirectoryEntriesSize(int size);
        int setData(string data);
        DirectoryEntry* getDirectoryEntries();
        size_t getDirectoryEntriesSize();
        uint16_t* getAddresses();
        size_t getAddressesSize();
        string getData();
    private:
        void init();
        // If the datablock consist of directory entries
        size_t directoryEntriesSize;
        DirectoryEntry* directoryEntries;
        
        // If the datablock consist of addresses of other data blocks
        size_t addressesSize;
        uint16_t* addresses;
        string data;
};

#endif