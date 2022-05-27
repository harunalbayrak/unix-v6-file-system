#ifndef _FILE_SYSTEM_H
#define _FILE_SYSTEM_H

#include <string>
#include <vector>
#include "superblock.h"
#include "inode.h"
#include "datablock.h"

#define NUMBEROF_INODE 200
#define NUMBEROF_BLOCK 200

#define MODE_DIR 01000
#define MODE_FILE 00000
#define OWNER_R	4<<6
#define OWNER_W	2<<6
#define OWNER_X	1<<6
#define GROUP_R	4<<3
#define GROUP_W	2<<3
#define GROUP_X	1<<3
#define OTHERS_R 4
#define OTHERS_W 2
#define OTHERS_X 1
#define FILE_DEF_PERMISSION 0664
#define DIR_DEF_PERMISSION 0755

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
        int deneme();
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

        int inodeSize = 128;
        int blockSize = 0;

        string filename;
        SID sid;
};

#endif