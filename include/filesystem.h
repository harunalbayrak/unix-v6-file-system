#ifndef _FILE_SYSTEM_H
#define _FILE_SYSTEM_H

#include <string>
#include <vector>
#include "superblock.h"
#include "inode.h"
#include "datablock.h"

#define NUMBEROF_INODE 200
#define NUMBEROF_BLOCK 200

using namespace std;

typedef struct{
    SuperBlock superBlock;
    Inode inodes[NUMBEROF_INODE];
    DataBlock dataBlocks[NUMBEROF_BLOCK];
} SID;

class FileSystem {
    public:
        FileSystem();
        FileSystem(int blockSize);
        FileSystem(int blockSize, string filename);
        int makeFileSystem();
        int createFile();
        int createReader();
        int sendCommand(int command, string path);
        int sendCommand(int command, string path, string filename);
        int dir(string path);
        int mkdir(string path);
        int rmdir(string path);
        int dumpe2fs();
        int write(string path, string filename);
        int read(string path, string filename);
        int del(string path);
    private:
        int getBlockSize();
        int createInode(Inode* inode ,int i);
        int getDataBlock2(const char* db);
        int splitPath(string* path);
        int calculateInodeSize(int inodeNum);
        int writeSIDToFS();
        int readSIDFromFS();
        int incrementI_B();
        int printCommand(int command, string path, string filename);
        int printSuperBlock(const SuperBlock& superBlock);
        int printInode(const Inode& inode);
        int printDataBlockNonEmpty();
        int printDataBlock(DataBlock* dataBlock);

        uint16_t inodeSize = 128;
        uint16_t blockSize = 0;

        string filename;
        SID sid;
};

#endif