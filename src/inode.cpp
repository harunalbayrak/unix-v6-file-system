#include <iostream>
#include <cstdlib>
#include "../include/inode.h"

Inode::Inode(){
    init();
}

Inode::Inode(int inodeNumber){
    init();
    this->inodeNumber = inodeNumber;
}

Inode::Inode(int inodeNumber, int addressOfDiskBlock){
    init();
    this->inodeNumber = inodeNumber;
    addressOfDiskBlocks[0] = addressOfDiskBlock;
    isEmpty = 0;
}

Inode::Inode(int inodeNumber, int addressOfDiskBlock, I_Type type){
    init();
    this->inodeNumber = inodeNumber;
    addressOfDiskBlocks[0] = addressOfDiskBlock;
    isEmpty = 0;
    this->type = type;
}

Inode::Inode(int inodeNumber, int* addressesOfDiskBlock, int addressesOfDiskBlocksSize, I_Type type){
    init();
    this->inodeNumber = inodeNumber;
    isEmpty = 0;
    this->type = type;
    for(int i=0;i<addressesOfDiskBlocksSize;++i){
        this->addressOfDiskBlocks[i] = addressesOfDiskBlock[i];
    }
}

int Inode::setEmpty(){
    changedTime = time(NULL);
    modifiedTime = time(NULL);
    accessTime = time(NULL);
    for(int i=0;i<10;++i){
        addressOfDiskBlocks[i] = -1;
    }
    isEmpty = 1;

    return 0;
}

int Inode::setAddresses(int addresses[], int addressesSize){
    for(int i=0;i<addressesSize;++i){
        addressOfDiskBlocks[i] = addresses[i];
    }

    return 0;
}

void Inode::init(){
    inodeNumber = 0;
    size = 0;
    permission = 0;
    changedTime = time(NULL);
    modifiedTime = time(NULL);
    accessTime = time(NULL);
    indirectBlockType = SINGLE;
    for(int i=0;i<10;++i){
        addressOfDiskBlocks[i] = -1;
    }
    isEmpty = 1;
    type = DIRECTORY;
}