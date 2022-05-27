#ifndef _INODE_H_
#define _INODE_H_

#include <iostream>

enum I_IndirectBlockType { SINGLE, DOUBLE, TRIPLE };
enum I_Type { ROOT_DIRECTORY, DIRECTORY, REGULAR_FILE };

class Inode {
    public:
        Inode();
        Inode(int inodeNumber);
        Inode(int inodeNumber, int addressOfDiskBlock);
        Inode(int inodeNumber, int addressOfDiskBlock, I_Type type);
        Inode(int inodeNumber, int* addressesOfDiskBlock, int addressesOfDiskBlocksSize, I_Type type);
        
        int setEmpty();
        int setAddresses(int addresses[], int addressesSize);

        unsigned short inodeNumber;
        unsigned int size;
        unsigned short permission;
        time_t  changedTime;
	    time_t  modifiedTime;
	    time_t  accessTime;
    	I_IndirectBlockType indirectBlockType;
    	I_Type type;
        int addressOfDiskBlocks[10];
        int isEmpty;

    private:
        void init();
};

#endif