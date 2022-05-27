#include "../include/superblock.h"

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

void SuperBlock::setNumberOfInodes(int n){
    inodes = n;
}

void SuperBlock::setNumberOfBlocks(int n){
    blocks = n;
}

void SuperBlock::setNumberOfFreeInodes(int n){
    freeInodes = n;
}

void SuperBlock::setNumberOfFreeBlocks(int n){
    freeBlocks = n;
}

void SuperBlock::setSuperBlockSize(int n){
    superBlockSize = n;
}

void SuperBlock::setInodeSize(int n){
    inodeSize = n;
}

void SuperBlock::setBlockSize(int n){
    blockSize = n;
}