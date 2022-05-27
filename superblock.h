#ifndef _SUPERBLOCK_H_
#define _SUPERBLOCK_H_

class SuperBlock{
    public:
        unsigned int getNumberOfInodes();
        unsigned int getNumberOfBlocks();
        unsigned int getNumberOfFreeInodes();
        unsigned int getNumberOfFreeBlocks();
        unsigned int getSuperBlockSize();
        unsigned int getInodeSize();
        unsigned int getBlockSize();
        int setNumberOfInodes(int n);
        int setNumberOfBlocks(int n);
        int setNumberOfFreeInodes(int n);
        int setNumberOfFreeBlocks(int n);
        int setSuperBlockSize(int n);
        int setInodeSize(int n);
        int setBlockSize(int n);
    private:
        unsigned int inodes;
        unsigned int blocks;
        unsigned int freeInodes;
        unsigned int freeBlocks;
        unsigned int superBlockSize;
        unsigned int inodeSize;
        unsigned int blockSize;
};

// typedef struct {
//     unsigned short inodes;
//     unsigned int dataBlocks;
//     unsigned short freeInodes;
//     unsigned int freeDataBlocks;
// } S_NumberOf;

// typedef struct {
//     unsigned short superBlockSize;
//     unsigned short inodeSize;
//     unsigned short dataBlockSize;
//     // unsigned short groupBlock;
// } S_GeneralInfo;

// typedef struct {
//     S_NumberOf numberOf;
//     S_GeneralInfo generalInfo;
// } SuperBlock;

#endif