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
        void setNumberOfInodes(int n);
        void setNumberOfBlocks(int n);
        void setNumberOfFreeInodes(int n);
        void setNumberOfFreeBlocks(int n);
        void setSuperBlockSize(int n);
        void setInodeSize(int n);
        void setBlockSize(int n);
    private:
        unsigned int inodes;
        unsigned int blocks;
        unsigned int freeInodes;
        unsigned int freeBlocks;
        unsigned int superBlockSize;
        unsigned int inodeSize;
        unsigned int blockSize;
};

#endif