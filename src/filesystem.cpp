#include <cstring>
#include <iostream>
#include <fstream>
#include <cmath>
#include "../include/filesystem.h"
#include "../include/directoryentry.h"

FileSystem::FileSystem(){
    this->blockSize = 4096;
    this->filename = "myFileSystem.dat";
}

FileSystem::FileSystem(int blockSize){
    this->blockSize = blockSize;
    this->filename = "myFileSystem.dat";
}

FileSystem::FileSystem(int blockSize, string filename){
    this->blockSize = blockSize;
    this->filename = filename;

    printf("Block Size: %d\nFilename: %s\n", blockSize, filename.c_str());
}

int FileSystem::getBlockSize(){
    return this->blockSize;
}

int FileSystem::makeFileSystem(){
    sid.superBlock.setSuperBlockSize(sizeof(SuperBlock));
    sid.superBlock.setInodeSize(inodeSize);
    sid.superBlock.setBlockSize(blockSize * 1024);
    sid.superBlock.setNumberOfInodes(0);
    sid.superBlock.setNumberOfBlocks(0);
    sid.superBlock.setNumberOfFreeInodes(NUMBEROF_INODE);
    sid.superBlock.setNumberOfFreeBlocks(NUMBEROF_BLOCK);

    for(int i=0;i<NUMBEROF_INODE;++i){
        // createInode(&sid.inodes[i],i+1);
        Inode _inode = Inode(i+1);
        sid.inodes[i] = _inode;
    }

    for(int i=0;i<NUMBEROF_BLOCK;++i){
        DataBlock dataBlock = DataBlock(sid.superBlock.getBlockSize());

        sid.dataBlocks[i] = dataBlock;
    }

    sid.superBlock.setNumberOfInodes(sid.superBlock.getNumberOfInodes() + 1);
    sid.superBlock.setNumberOfBlocks(sid.superBlock.getNumberOfBlocks() + 1);
    sid.superBlock.setNumberOfFreeInodes(sid.superBlock.getNumberOfFreeInodes() - 1);
    sid.superBlock.setNumberOfFreeBlocks(sid.superBlock.getNumberOfFreeBlocks() - 1);

    sid.inodes[0].isEmpty = 0;
    sid.inodes[0].type = ROOT_DIRECTORY;
    sid.inodes[0].addressOfDiskBlocks[0] = 0;

    writeSIDToFS();

    return 0;
}

int FileSystem::dir(string path){
    readSIDFromFS();

    size_t pos = 0;
    int count = 0, _block = 0;
    string token2, delimiter = "/";
    while ((pos = path.find(delimiter)) != string::npos) {
        count++;
        token2 = path.substr(0, pos);
        path.erase(0, pos + delimiter.length());

        if(strcmp(token2.c_str(), "") == 0){
            continue;
        }
        _block = getDataBlock2(path.c_str());
        if(_block == -1){
            printf("Error 2\n");
            return -1;
        } 
    }

    _block = getDataBlock2(path.c_str());
    if(_block == -1){
        _block = 0;
    }

    DataBlock _dataBlock = sid.dataBlocks[_block];
    for(size_t i=0;i<_dataBlock.getDirectoryEntriesSize();++i){
        DirectoryEntry _directoryEntry = _dataBlock.getDirectoryEntries()[i];

        printf("-> %s\n",_directoryEntry.getDirectoryName());
    }

    return 0;
}

int FileSystem::mkdir(string path){
    readSIDFromFS();

    if(sid.superBlock.getNumberOfFreeInodes() == 0 || sid.superBlock.getNumberOfFreeBlocks() == 0){
        printf("Error 4");
        return -1;
    }

    int currentInodeNum = 0;
    int currentBlockNum = 0;

    for(int i=0;i<NUMBEROF_INODE;++i){
        if(sid.inodes[i].isEmpty == 1){
            currentInodeNum = i;
            break;
        }
    }

    for(int i=0;i<NUMBEROF_BLOCK;++i){
        if(sid.dataBlocks[i].isEmpty() == 1){
            currentBlockNum = i;
            break;
        }
    }

    incrementI_B();

    Inode _inode = Inode(currentInodeNum,currentBlockNum,DIRECTORY);
    sid.inodes[currentInodeNum] = _inode;

    // printf("path: %s \n",path.c_str());

    int _block = splitPath(&path);

    DirectoryEntry _directoryEntry;
    _directoryEntry.setInodeAddress(currentInodeNum);
    _directoryEntry.setDirectoryName(path.c_str());

    sid.dataBlocks[_block].addDirectoryEntry(_directoryEntry); 

    writeSIDToFS();
    
    return 0;
}

int FileSystem::rmdir(string path){
    readSIDFromFS();

    // printDataBlockNonEmpty();

    if(strcmp(path.c_str(),"/")==0){
        printf("Root path silinemez");
        return -1;
    }

    int flag = 0, _block = splitPath(&path);

    DataBlock _dataBlock = sid.dataBlocks[_block];
    for(size_t i=0;i<_dataBlock.getDirectoryEntriesSize();++i){
        DirectoryEntry _directoryEntry = _dataBlock.getDirectoryEntries()[i];

        if(strcmp(_directoryEntry.getDirectoryName(),path.c_str()) == 0){
            if(sid.inodes[_directoryEntry.getInodeAddress()].type == DIRECTORY){
                sid.dataBlocks[_block].removeDirectoryEntry(_directoryEntry.getInodeAddress());
                flag = 1;
            }
        }
    }
    
    // printDataBlockNonEmpty();

    writeSIDToFS();

    if(flag == 0){
        return -1;
    }

    return 0;
}

int FileSystem::dumpe2fs(){

    return 0;
}

int FileSystem::write(string path, string filename){
    readSIDFromFS();

    if(sid.superBlock.getNumberOfFreeInodes() == 0 || sid.superBlock.getNumberOfFreeBlocks() == 0){
        printf("Error 4");
        return -1;
    }

    int currentInodeNum = 0;
    int currentBlockNum = 0;
    int maxBlockSize = sid.superBlock.getBlockSize()/2;
    uint16_t* currentBlocks = (uint16_t*) malloc(maxBlockSize*sizeof(uint16_t));
    int currentBlocksSize = 0;

    for(int i=0;i<NUMBEROF_INODE;++i){
        if(sid.inodes[i].isEmpty == 1){
            currentInodeNum = i;
            break;
        }
    }

    for(int i=0;i<NUMBEROF_BLOCK;++i){
        if(sid.dataBlocks[i].isEmpty() == 1){
            currentBlockNum = i;
            break;
        }
    }

    incrementI_B();

    Inode _inode = Inode(currentInodeNum,currentBlockNum,REGULAR_FILE);
    
    int _block = splitPath(&path);

    fstream newfile;
    string fileContent;
    newfile.open(filename,ios::in);
    if (newfile.is_open()){
        string tp;
        while(getline(newfile, tp)){
            fileContent.append(tp);
            fileContent.append("\n");
        }
        newfile.close();
    }

    // FIXME: Fix last character problem
    // fileContent.pop_back();

    double d_howManyBlock = fileContent.size()/(double)sid.superBlock.getBlockSize();
    int i_howManyBlock = ceil(d_howManyBlock);

    for(int i=0;i<NUMBEROF_BLOCK;++i){
        if(sid.dataBlocks[i].isEmpty() == 1 && i != currentBlockNum){
            currentBlocks[currentBlocksSize++] = i;
            if(currentBlocksSize == i_howManyBlock){
                break;
            }
        }
    }

    // printf("%d %d\n",maxBlockSize,i_howManyBlock);

    d_howManyBlock = currentBlocksSize/(double)maxBlockSize;
    i_howManyBlock = ceil(d_howManyBlock);

    // printf("%d\n",i_howManyBlock);

    int k=0;
    for(int i=0;i<8 && i<i_howManyBlock;++i){
        int addressSingleDB = -1;

        // printf("hey\n");

        if(i == 0){
            addressSingleDB = _inode.addressOfDiskBlocks[i];
        } else{
            for(int j=0;j<NUMBEROF_BLOCK;++j){
                if(sid.dataBlocks[j].isEmpty() == 1){
                    currentBlockNum = j;
                    break;
                }
            }
            _inode.addressOfDiskBlocks[i] = currentBlockNum;
            addressSingleDB = _inode.addressOfDiskBlocks[i];
        }

        for(;k<currentBlocksSize;++k){
            // printf("cc: %d %d\n",addressSingleDB,currentBlocks[k]);

            sid.dataBlocks[addressSingleDB].addAddress(currentBlocks[k]);
        }
    }

    // _inode.setAddresses(currentBlocks,currentBlocksSize);
    sid.inodes[currentInodeNum] = _inode;

    int i=0;
    for (unsigned int j=0; j<fileContent.size(); j+=sid.superBlock.getBlockSize()) {
        sid.dataBlocks[currentBlocks[i]] = DataBlock(sid.superBlock.getBlockSize(),REGULAR_FILE);
        sid.dataBlocks[currentBlocks[i]].setData(fileContent.substr(j,sid.superBlock.getBlockSize()));
        // printf("s---> %d\n",currentBlocks[i]);
        // printf("size %d : %d\n",i,sid.dataBlocks[currentBlocks[i]].size());
        // printf("size %d : %d : %ld\n",j,j+sid.superBlock.getBlockSize(), fileContent.substr(j,sid.superBlock.getBlockSize()).size());
        i++;
    }

    DirectoryEntry _directoryEntry;
    _directoryEntry.setInodeAddress(currentInodeNum);
    _directoryEntry.setDirectoryName(path.c_str());

    sid.dataBlocks[_block].addDirectoryEntry(_directoryEntry); 

    writeSIDToFS();

    return 0;
}

int FileSystem::read(string path, string filename){
    readSIDFromFS();

    size_t pos = 0;
    int count = 0, _block = 0;
    string token2, delimiter = "/";
    while ((pos = path.find(delimiter)) != string::npos) {
        count++;
        token2 = path.substr(0, pos);
        path.erase(0, pos + delimiter.length());

        if(strcmp(token2.c_str(), "") == 0){
            continue;
        }
        _block = getDataBlock2(path.c_str());
        if(_block == -1){
            printf("Error 2\n");
            return -1;
        } 
    }

    _block = getDataBlock2(path.c_str());
    if(_block == -1){
        _block = 0;
    }

    int addresses[8], addressesSize=0;

    for(int i=0;i<8;++i){
        if(sid.inodes[_block].addressOfDiskBlocks[i] == -1){
            break;
        }

        // printf("adress: %d\n",sid.inodes[_block].addressOfDiskBlocks[i]);

        addresses[i] = sid.inodes[_block].addressOfDiskBlocks[i];
        addressesSize++;
    }

    string str = "";

    for(int i=0;i<addressesSize;++i){
        DataBlock _db = sid.dataBlocks[addresses[i]];
        uint16_t* _addresses = _db.getAddresses();
        int _addressesSize = _db.getAddressesSize();

        for(int j=0;j<_addressesSize;++j){
            // printf("adress2->: %d\n",_addresses[j]);

            string str2 = sid.dataBlocks[_addresses[j]].getData();
            str.append(str2);
        }
    }

    // printf("%s\n",str.c_str());

    fstream newfile;
    newfile.open(filename,ios::out);
    if(newfile.is_open()){
        newfile << str;
        newfile.close();
    }

    return 0;
}

int FileSystem::del(string path){
    readSIDFromFS();

    // printDataBlockNonEmpty();

    if(strcmp(path.c_str(),"/")==0){
        printf("Root path silinemez");
        return -1;
    }

    int flag = 0, _block = splitPath(&path);

    DataBlock _dataBlock = sid.dataBlocks[_block];
    for(size_t i=0;i<_dataBlock.getDirectoryEntriesSize();++i){
        DirectoryEntry _directoryEntry = _dataBlock.getDirectoryEntries()[i];

        if(strcmp(_directoryEntry.getDirectoryName(),path.c_str()) == 0){
            if(sid.inodes[_directoryEntry.getInodeAddress()].type == REGULAR_FILE){
                sid.dataBlocks[_block].removeDirectoryEntry(_directoryEntry.getInodeAddress());
                flag = 1;
            }
        }
    }
    
    // printDataBlockNonEmpty();

    writeSIDToFS();

    if(flag == 0) {
        return -1;
    }

    return 0;
}

int FileSystem::getDataBlock2(const char* db){
    readSIDFromFS();

    for(int i=0;i<NUMBEROF_INODE;++i){
        if(sid.inodes[i].isEmpty == 1){
            continue;
        }
        int address = sid.inodes[i].addressOfDiskBlocks[0];
        DataBlock _dblock = sid.dataBlocks[address];
        for(size_t i=0;i<_dblock.getDirectoryEntriesSize();++i){
            DirectoryEntry _directoryEntry = _dblock.getDirectoryEntries()[i];

            // printf("%d %s %s\n",_directoryEntry.inodeAddress,_directoryEntry.directoryName,db);

            int cmp = strcmp(_directoryEntry.getDirectoryName(),db);
            if(cmp == 0){
                return _directoryEntry.getInodeAddress();
            }
        }
    }

    return -1;
}

int FileSystem::splitPath(string* path){
    size_t pos = 0;
    int count = 0, _block = 0;
    string token2, delimiter = "/";
    while ((pos = path->find(delimiter)) != string::npos) {
        count++;
        token2 = path->substr(0, pos);
        // printf("1- %s\n",token2.c_str());
        path->erase(0, pos + delimiter.length());

        if(strcmp(token2.c_str(), "") == 0){
            continue;
        }
        _block = getDataBlock2(token2.c_str());
        if(_block == -1){
            printf("Error 2\n");
            return -1;
        }
    }

    return _block;
}

int FileSystem::calculateInodeSize(int inodeNum){
    readSIDFromFS();

    if(inodeNum > NUMBEROF_INODE || sid.inodes[inodeNum].isEmpty == 1){
        return -1;
    }

    int _size = 0;
    for(int i=0;i<8;++i){
        if(sid.inodes[inodeNum].addressOfDiskBlocks[i] == -1){
            break;
        }

        int address = sid.inodes[inodeNum].addressOfDiskBlocks[i];
        _size += sid.dataBlocks[address].size();
    }

    return _size;
}

int FileSystem::writeSIDToFS(){
    ofstream wf(filename.c_str(), ios::out | ios::binary);
    if(!wf) {
        cout << "Cannot open file!" << endl;
        return -1;
    }

    wf.write((char *) &sid.superBlock, sizeof(SuperBlock));
    for(int i=0;i<NUMBEROF_INODE;i++)
        wf.write((char *) &sid.inodes[i], sizeof(Inode));
    for(int i=0;i<NUMBEROF_BLOCK;i++)
        wf.write((char *) &sid.dataBlocks[i], sizeof(DataBlock));

    wf.close();

    return 0;
}

int FileSystem::readSIDFromFS(){
    SID* _sid = new SID();

    ifstream rf(filename.c_str(), ios::out | ios::binary);
    if(!rf) {
        cout << "Cannot open file!" << endl;
        return -1;
    }

    rf.read((char *) &sid.superBlock, sizeof(SuperBlock));
    for(int i= 0;i<NUMBEROF_INODE;i++)
        rf.read((char *) &_sid->inodes[i], sizeof(Inode));
    for(int i= 0;i<NUMBEROF_BLOCK;i++)
        rf.read((char *) &_sid->dataBlocks[i], sizeof(DataBlock));

    return 0;
}

int FileSystem::incrementI_B(){
    readSIDFromFS();

    sid.superBlock.setNumberOfInodes(sid.superBlock.getNumberOfInodes() + 1);
    sid.superBlock.setNumberOfBlocks(sid.superBlock.getNumberOfBlocks() + 1);
    sid.superBlock.setNumberOfFreeInodes(sid.superBlock.getNumberOfFreeInodes() - 1);
    sid.superBlock.setNumberOfFreeBlocks(sid.superBlock.getNumberOfFreeBlocks() - 1);

    writeSIDToFS();

    return 0;
}

int FileSystem::sendCommand(int command, string path){
    int result = 0;

    printf("\n-------------------\n");
    printCommand(command, path, "");

    switch(command){
        case 0:
            result = dir(path);
            break;
        case 1:
            result = mkdir(path);
            break;
        case 2:
            result = rmdir(path);
            break;
        case 3:
            result = dumpe2fs();
            break;
        case 6:
            result = del(path);
            break;
    }

    if(result == 0){
        printf("STATUS: SUCCESSFUL!\n");
    } else{
        printf("STATUS: FAIL!\n");
    }

    printf("-------------------\n");

    return 0;
}

int FileSystem::sendCommand(int command, string path, string filename){
    int result = 0;

    printf("\n-------------------\n");
    printCommand(command, path, filename);

    switch(command){
        case 4:
            result = write(path,filename);
            break;
        case 5:
            result = read(path,filename);
            break;
    }

    if(result == 0){
        printf("STATUS: SUCCESSFUL!\n");
    } else{
        printf("STATUS: FAIL!\n");
    }

    printf("-------------------\n");

    return 0;
}

int FileSystem::printCommand(int command, string path, string filename){
    switch(command){
        case 0:
            printf("dir %s\n", path.c_str());
            break;
        case 1:
            printf("mkdir %s\n", path.c_str());
            break;
        case 2:
            printf("rmdir %s\n", path.c_str());
            break;
        case 3:
            printf("dumpe2fs\n");
            break;
        case 4:
            printf("write %s %s\n",path.c_str(), filename.c_str());
            break;
        case 5:
            printf("read %s %s\n",path.c_str(), filename.c_str());
            break;
        case 6:
            printf("del %s\n",path.c_str());
            break;
    }

    return 0;
}

int FileSystem::printSuperBlock(const SuperBlock& superblock){
    printf("---------- SUPERBLOCK ----------\n");
    printf("--------- General Info ---------\n");
    printf("Block Size: %d\n",sid.superBlock.getBlockSize());
    printf("Inode Size: %d\n",sid.superBlock.getInodeSize());
    printf("SuperBlock Size: %d\n",sid.superBlock.getSuperBlockSize());
    // printf("Block per group: %d\n",superBlock.generalInfo.groupBlock);
    printf("---------- Number Of ----------\n");
    printf("Number of Free Data Blocks: %d\n",sid.superBlock.getNumberOfFreeBlocks());
    printf("Number of Data Blocks: %d\n",sid.superBlock.getNumberOfBlocks());
    printf("Number of Free Inodes: %d\n",sid.superBlock.getNumberOfFreeInodes());
    printf("Number of Inodes: %d\n",sid.superBlock.getNumberOfInodes());
    printf("--------------------------------\n");

    return 0;
}

int FileSystem::printInode(const Inode& inode){

    return 0;
}

// int FileSystem::printDataBlockNonEmpty(){
//     for(int i=0;i<NUMBEROF_BLOCK;++i){
//         if(sid.dataBlocks[i].isEmpty() == 0){
//             printDataBlock(&sid.dataBlocks[i]);
//         }
//     }

//     return 0;   
// }

// int FileSystem::printDataBlock(DataBlock* dataBlock){
//     printf("--------------------------------\n");
//     // printf("------ Block Number: %d\n", 0);
//     printf("------ Inode : %d\n", dataBlock->getDirectoryEntry().inodeAddress);
//     printf("------ Directory Name : %s\n", dataBlock->getDirectoryEntry().directoryName);
//     printf("------ ADbs: ");
//     for(int i=0;i<dataBlock->getADbsSize();++i){
//         printf("%d,",dataBlock->getADbs()[i]);
//     }
//     printf("\n--------------------------------\n");

//     return 0;
// }