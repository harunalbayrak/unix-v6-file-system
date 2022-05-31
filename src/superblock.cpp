#include <fstream>
#include "../include/superblock.h"

using namespace std;

uint16_t SuperBlock::getNumberOfInodes(){
    return inodes;
}

uint16_t SuperBlock::getNumberOfBlocks(){
    return blocks;
}

uint16_t SuperBlock::getNumberOfFreeInodes(){
    return freeInodes;
}

uint16_t SuperBlock::getNumberOfFreeBlocks(){
    return freeBlocks;
}

uint16_t SuperBlock::getSuperBlockSize(){
    return superBlockSize;
}

uint16_t SuperBlock::getBlockSize(){
    return blockSize;
}

void SuperBlock::setNumberOfInodes(uint16_t n){
    inodes = n;
}

void SuperBlock::setNumberOfBlocks(uint16_t n){
    blocks = n;
}

void SuperBlock::setNumberOfFreeInodes(uint16_t n){
    freeInodes = n;
}

void SuperBlock::setNumberOfFreeBlocks(uint16_t n){
    freeBlocks = n;
}

void SuperBlock::setSuperBlockSize(uint16_t n){
    superBlockSize = n;
}

void SuperBlock::setBlockSize(uint16_t n){
    blockSize = n;
}