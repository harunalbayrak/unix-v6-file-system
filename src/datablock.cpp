#include <iostream>
#include <cstdlib>
#include "../include/datablock.h"

DataBlock::DataBlock(){
    data.resize(0);
    init();
}

DataBlock::DataBlock(int blockSize){
    data.resize(blockSize);
    init();
}

DataBlock::DataBlock(int blockSize, I_Type type){
    data.resize(blockSize);
    init();
}

int DataBlock::size(){
    if(data.compare("") == 0){
        return sizeof(DirectoryEntry) * directoryEntriesSize;
    }

    return data.size();
}

int DataBlock::isEmpty(){
    return directoryEntriesSize == 0;
}

int DataBlock::isDirectory(){
    return data.size() == 0;
}

int DataBlock::addDirectoryEntry(DirectoryEntry directoryEntry){
    try{
        if (directoryEntries == NULL){
            directoryEntries = (DirectoryEntry*) malloc(sizeof(DirectoryEntry));
            directoryEntries[directoryEntriesSize++] = directoryEntry;
        } else{
            DirectoryEntry* old = (DirectoryEntry*) malloc(directoryEntriesSize*sizeof(DirectoryEntry));
            for(size_t i=0;i<directoryEntriesSize;++i){
                old[i] = directoryEntries[i];
            }

            directoryEntriesSize++;
            directoryEntries = (DirectoryEntry*) realloc(directoryEntries, directoryEntriesSize*sizeof(DirectoryEntry));
    
            for(size_t i=0;i<directoryEntriesSize-1;++i){
                directoryEntries[i] = old[i];
            }

            directoryEntries[directoryEntriesSize-1] = directoryEntry;
        }
    } catch(const std::exception& e){
        printf("Error: %s\n", e.what());
        return -1;
    }

    return 0;
}

int DataBlock::removeDirectoryEntry(int inodeAddress){
    try{
        if (directoryEntries == NULL){
            return -1;
        } else{
            DirectoryEntry* old = (DirectoryEntry*) malloc(directoryEntriesSize*sizeof(DirectoryEntry));
            for(size_t i=0,j=0;i<directoryEntriesSize;++i,++j){
                if(directoryEntries[j].getInodeAddress() != inodeAddress){
                    old[i] = directoryEntries[j];
                } else{
                    i = i-1;
                    continue;
                }
            }

            directoryEntriesSize--;
            directoryEntries = (DirectoryEntry*) realloc(directoryEntries, directoryEntriesSize*sizeof(DirectoryEntry));
    
            for(size_t i=0;i<directoryEntriesSize;++i){
                directoryEntries[i] = old[i];
            }
        }
    } catch(const std::exception& e){
        printf("Error: %s\n", e.what());
        return -1;
    }

    return 0;
}

int DataBlock::setEmptyDataBlock(int blockSize){
    data.clear();
    data.resize(blockSize);

    return 0;
}

int DataBlock::setDirectoryEntriesSize(int size){
    directoryEntriesSize = size;

    return 0;
}

int DataBlock::setData(string data){
    this->data = data;

    return 0;
}

DirectoryEntry* DataBlock::getDirectoryEntries(){
    return directoryEntries;
}

size_t DataBlock::getDirectoryEntriesSize(){
    return directoryEntriesSize;
}

string DataBlock::getData(){
    return data;
}

void DataBlock::init(){
    directoryEntries = NULL;
    directoryEntriesSize = 0;
}