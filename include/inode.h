#ifndef _INODE_H_
#define _INODE_H_

#include <iostream>

using namespace std;

enum I_IndirectBlockType { DIRECT, SINGLE, DOUBLE, TRIPLE };
enum I_Type { ROOT_DIRECTORY, DIRECTORY, REGULAR_FILE };

class Inode {
    public:
        Inode();
        Inode(int inodeNumber);
        Inode(int inodeNumber, int addressOfDiskBlock);
        Inode(int inodeNumber, int addressOfDiskBlock, I_Type type);
        Inode(int inodeNumber, int* addressesOfDiskBlock, int addressesOfDiskBlocksSize, I_Type type);
        int setEmpty();
        int setAddresses(uint16_t* addresses, int addressesSize);
        uint16_t inodeNumber;
        uint16_t permission;
        time_t  changedTime;
	    time_t  modifiedTime;
	    time_t  accessTime;
    	I_IndirectBlockType indirectBlockType;
    	I_Type type;
        int addressOfDiskBlocks[8];
        int isEmpty;

    private:
        void init();
};

#endif