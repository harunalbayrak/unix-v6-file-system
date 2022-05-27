#include "superblock.h"

unsigned int SuperBlock::getNumberOfInodes(){
    return inodes;
}

unsigned int SuperBlock::getNumberOfBlocks(){
    return blocks;
}

unsigned int SuperBlock::getNumberOfFreeInodes(){
    return freeInodes;
}

unsigned int SuperBlock::getNumberOfFreeBlocks(){
    return freeBlocks;
}

unsigned int SuperBlock::getSuperBlockSize(){
    return superBlockSize;
}

unsigned int SuperBlock::getInodeSize(){
    return inodeSize;
}

unsigned int SuperBlock::getBlockSize(){
    return blockSize;
}

int SuperBlock::setNumberOfInodes(int n){
    inodes = n;
}

int SuperBlock::setNumberOfBlocks(int n){
    blocks = n;
}

int SuperBlock::setNumberOfFreeInodes(int n){
    freeInodes = n;
}

int SuperBlock::setNumberOfFreeBlocks(int n){
    freeBlocks = n;
}

int SuperBlock::setSuperBlockSize(int n){
    superBlockSize = n;
}

int SuperBlock::setInodeSize(int n){
    inodeSize = n;
}

int SuperBlock::setBlockSize(int n){
    blockSize = n;
}