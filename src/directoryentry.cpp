#include <iostream>
#include <cstdlib>
#include <cstring>
#include "../include/directoryentry.h"

DirectoryEntry::DirectoryEntry(){
    inodeAddress = -1;
}

DirectoryEntry::DirectoryEntry(DirectoryEntry* dir){
    inodeAddress = dir->inodeAddress;
    strcpy(fileName, dir->fileName);
}

int DirectoryEntry::getInodeAddress(){
    return inodeAddress;
}

char* DirectoryEntry::getFileName(){
    return fileName;
}

void DirectoryEntry::setInodeAddress(int n){
    inodeAddress = n;
}

void DirectoryEntry::setFileName(const char* fileName){
    strcpy(this->fileName, fileName);
}

int DirectoryEntry::isEmpty(){
    return inodeAddress == -1;
}