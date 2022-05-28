#ifndef _SUPERBLOCK_H_
#define _SUPERBLOCK_H_

#include <iostream>

class SuperBlock{
    public:
        uint16_t getNumberOfInodes();
        uint16_t getNumberOfBlocks();
        uint16_t getNumberOfFreeInodes();
        uint16_t getNumberOfFreeBlocks();
        uint16_t getSuperBlockSize();
        uint16_t getInodeSize();
        uint16_t getBlockSize();
        void setNumberOfInodes(uint16_t n);
        void setNumberOfBlocks(uint16_t n);
        void setNumberOfFreeInodes(uint16_t n);
        void setNumberOfFreeBlocks(uint16_t n);
        void setSuperBlockSize(uint16_t n);
        void setInodeSize(uint16_t n);
        void setBlockSize(uint16_t n);
    private:
        uint16_t inodes;
        uint16_t blocks;
        uint16_t freeInodes;
        uint16_t freeBlocks;
        uint16_t superBlockSize;
        uint16_t inodeSize;
        uint16_t blockSize;
};

#endif