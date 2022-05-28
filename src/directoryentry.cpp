#include <iostream>
#include <cstdlib>
#include <cstring>
#include "../include/directoryentry.h"

DirectoryEntry::DirectoryEntry(){
    inodeAddress = 1;
}

int DirectoryEntry::getInodeAddress(){
    return inodeAddress;
}

char* DirectoryEntry::getDirectoryName(){
    return directoryName;
}

void DirectoryEntry::setInodeAddress(int n){
    inodeAddress = n;
}

void DirectoryEntry::setDirectoryName(const char* directoryName){
    strcpy(this->directoryName, directoryName);
}