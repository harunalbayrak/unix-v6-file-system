#ifndef _SUPERBLOCK_H_
#define _SUPERBLOCK_H_

#define GROUP_BLOCK 64

typedef struct {
    unsigned short inodes;
    unsigned int dataBlocks;
    unsigned short freeInodes;
    unsigned int freeDataBlocks;
} S_NumberOf;

typedef struct {
    int superBlockStartAddress;
    int inodesStartAddress;
    int dataBlocksStartAddress;
} S_StartAddresses;

typedef struct {
    unsigned short superBlockSize;
    unsigned short inodeSize;
    unsigned short dataBlockSize;
    // unsigned short groupBlock;
} S_GeneralInfo;

typedef struct {
    S_NumberOf numberOf;
    S_StartAddresses startAddresses;
    S_GeneralInfo generalInfo;
} SuperBlock;

#endif