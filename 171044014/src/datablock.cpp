#include <iostream>
#include <fstream>
#include <cstdlib>
#include "../include/datablock.h"

using namespace std;

DataBlock::DataBlock(){
    init();
}

DataBlock::DataBlock(int blockSize){
    init();
}

DataBlock::DataBlock(int blockSize, I_Type type){
    init();
}

int DataBlock::size(){
    if(data.compare("") == 0){
        return sizeof(DirectoryEntry) * directoryEntriesSize;
        // return 0;
    }

    return data.size();
}

int DataBlock::isEmpty(){
    return directoryEntriesSize == 0 && dataSize == 0 && addressesSize == 0;
    
    // return 0;
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
            for(int i=0;i<directoryEntriesSize;++i){
                old[i] = directoryEntries[i];
            }

            directoryEntriesSize++;
            directoryEntries = (DirectoryEntry*) realloc(directoryEntries, directoryEntriesSize*sizeof(DirectoryEntry));
    
            for(int i=0;i<directoryEntriesSize-1;++i){
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

int DataBlock::addAddress(uint16_t address){
    try{
        if (addresses == NULL){
            addresses = (uint16_t*) malloc(sizeof(uint16_t));
            addresses[addressesSize++] = address;
        } else{
            uint16_t* old = (uint16_t*) malloc(addressesSize*sizeof(uint16_t));
            for(int i=0;i<addressesSize;++i){
                old[i] = addresses[i];
            }

            addressesSize++;
            addresses = (uint16_t*) realloc(addresses, addressesSize*sizeof(uint16_t));
    
            for(int i=0;i<addressesSize-1;++i){
                addresses[i] = old[i];
            }

            addresses[addressesSize-1] = address;
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
            for(int i=0,j=0;i<directoryEntriesSize;++i,++j){
                if(directoryEntries[j].getInodeAddress() != inodeAddress){
                    old[i] = directoryEntries[j];
                } else{
                    i = i-1;
                    continue;
                }
            }

            directoryEntriesSize--;
            directoryEntries = (DirectoryEntry*) realloc(directoryEntries, directoryEntriesSize*sizeof(DirectoryEntry));
    
            for(int i=0;i<directoryEntriesSize;++i){
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
    init();

    return 0;
}

int DataBlock::setDirectoryEntriesSize(int size){
    directoryEntriesSize = size;

    return 0;
}

int DataBlock::setData(string data){
    this->data = data;
    dataSize = data.length();

    return 0;
}

DirectoryEntry* DataBlock::getDirectoryEntries(){
    return directoryEntries;
}

int DataBlock::getDirectoryEntriesSize(){
    return directoryEntriesSize;
}

uint16_t* DataBlock::getAddresses(){
    return addresses;
}

int DataBlock::getAddressesSize(){
    return addressesSize;
}

string DataBlock::getData(){
    return data;
}

int DataBlock::getDataSize(){
    return dataSize;
}

int DataBlock::serialize(FILE *fptr, bool bWrite){
    addressesSize = getAddressesSize();
    directoryEntriesSize = getDirectoryEntriesSize();

    try{
        if (bWrite) {
            fwrite(&addressesSize, sizeof(addressesSize), 1, fptr);
            fwrite(&directoryEntriesSize, sizeof(directoryEntriesSize), 1, fptr);
            fwrite(&dataSize, sizeof(dataSize), 1, fptr);

            for(int i=0;i<addressesSize;i++){
                fwrite(&addresses[i], sizeof(addresses[i]), 1, fptr);
            }
            for(int i=0;i<directoryEntriesSize;i++){
                fwrite(&directoryEntries[i], sizeof(directoryEntries[i]), 1, fptr);
            }
            if(dataSize > 0){
                // printf("datasizew: %d\n",dataSize);
                size_t size=data.size();

                fwrite(&size, sizeof(size), 1, fptr);
                fwrite(&data[0], size, 1, fptr);

                // printf("%s\n", str);
            }
        }
        else {
            fread(&addressesSize, sizeof(addressesSize), 1, fptr);
            fread(&directoryEntriesSize, sizeof(directoryEntriesSize), 1, fptr);
            fread(&dataSize, sizeof(dataSize), 1, fptr);

            addresses = (uint16_t*) malloc(addressesSize*sizeof(uint16_t));
            directoryEntries = (DirectoryEntry*) malloc(directoryEntriesSize*sizeof(DirectoryEntry));  

            for(int i=0;i<addressesSize;i++){
                fread(&addresses[i], sizeof(addresses[i]), 1, fptr);
            }
            for(int i=0;i<directoryEntriesSize;i++){
                fread(&directoryEntries[i], sizeof(directoryEntries[i]), 1, fptr);
            }
            if(dataSize > 0){
                size_t size;

                fread(&size, sizeof(size), 1, fptr);
                data.resize(size);
                fread(&data[0], size, 1, fptr);

                dataSize = size;

                // printf("_data: %s\n",data.c_str());
            }
        }
    } catch(const std::exception& e){
        printf("Error: %s\n",e.what());
        return -1;
    }

    return 0;
}

void DataBlock::init(){
    directoryEntries = NULL;
    directoryEntriesSize = 0;
    addresses = NULL;
    addressesSize = 0;
    dataSize = 0;
}