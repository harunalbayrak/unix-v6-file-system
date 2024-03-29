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
    this->blockSize = blockSize * 1024;
    this->filename = filename;
}

FileSystem::FileSystem(string filename){
    this->filename = filename;

    readSIDFromFS();
}

int FileSystem::getBlockSize(){
    return this->blockSize;
}

// Create a empty file system. Create a ROOT Directory.
int FileSystem::makeFileSystem(){
    sid.superBlock.setSuperBlockSize(sizeof(SuperBlock));
    sid.superBlock.setBlockSize(blockSize);
    sid.superBlock.setNumberOfInodes(0);
    sid.superBlock.setNumberOfBlocks(0);
    sid.superBlock.setNumberOfFreeInodes(NUMBEROF_INODE);
    sid.superBlock.setNumberOfFreeBlocks(NUMBEROF_BLOCK);

    for(int i=0;i<NUMBEROF_INODE;++i){
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

    int _block = -1;

    if(strcmp(path.c_str(),"\\") == 0){
        _block = 0;
    } else{
        _block = splitPath(&path);
    }

    if(_block == -1){
        return -1;
    }

    if(sid.inodes[_block].type == REGULAR_FILE){
        return -1;
    }

    sid.inodes[_block].changedTime = time(NULL);
    sid.inodes[_block].accessTime = time(NULL);
    sid.inodes[_block].modifiedTime = time(NULL);

    int inode = -1;
    DataBlock _dataBlock = sid.dataBlocks[_block];
    for(int i=0;i<_dataBlock.getDirectoryEntriesSize();++i){
        DirectoryEntry _directoryEntry = _dataBlock.getDirectoryEntries()[i];
        if(strcmp(_directoryEntry.getFileName(),path.c_str()) == 0){
            inode = _directoryEntry.getInodeAddress();
            break;
        }
    }
    
    if(strcmp(path.c_str(),"\\") == 0){
        _dataBlock = sid.dataBlocks[_block];
    } else{
        if(inode == -1){
            return -1;
        }

        _dataBlock = sid.dataBlocks[inode];
    }
    for(int i=0;i<_dataBlock.getDirectoryEntriesSize();++i){
        DirectoryEntry _directoryEntry = _dataBlock.getDirectoryEntries()[i];
        printf("-> %s\n",_directoryEntry.getFileName());
    }

    return 0;
}

int FileSystem::mkdir(string path){
    if(strcmp(path.c_str(),"\\")==0){
        printf("Error: Root path can't be created! It is automatically created when creating a file system.\n");
        return -1;
    }

    readSIDFromFS();

    if(sid.superBlock.getNumberOfFreeInodes() == 0 || sid.superBlock.getNumberOfFreeBlocks() == 0){
        printf("Error: There is no free inode or free block.\n");
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

    int _block = splitPath(&path);
    if(_block == -1){
        return -1;
    }

    if(sid.inodes[_block].type == REGULAR_FILE){
        return -1;
    }

    if(path.length() > 14){
        printf("Error: Maximum length of the file name must be 14. The length of your file name is: %ld\n", path.length());
        return -1;
    }

    DataBlock _dataBlock = sid.dataBlocks[sid.inodes[_block].addressOfDiskBlocks[0]];
    for(int i=0;i<_dataBlock.getDirectoryEntriesSize();++i){
        DirectoryEntry _directoryEntry = _dataBlock.getDirectoryEntries()[i];

        if(strcmp(_directoryEntry.getFileName(), path.c_str()) == 0){
            return -1;
        }
    }

    incrementI_B();

    Inode _inode = Inode(currentInodeNum,currentBlockNum,DIRECTORY);
    _inode.changedTime = time(NULL);
    _inode.accessTime = time(NULL);
    _inode.modifiedTime = time(NULL);
    sid.inodes[currentInodeNum] = _inode;

    DirectoryEntry _directoryEntry;
    _directoryEntry.setInodeAddress(currentInodeNum);

    _directoryEntry.setFileName(path.c_str());

    sid.dataBlocks[_block].addDirectoryEntry(_directoryEntry); 

    writeSIDToFS();
    
    return 0;
}

int FileSystem::rmdir(string path){
    if(strcmp(path.c_str(),"\\")==0){
        printf("Root Path can't be deleted!");
        return -1;
    }

    readSIDFromFS();

    int flag = 0, _block = splitPath(&path);
    if(_block == -1){
        return -1;
    }

    if(path.length() > 14){
        printf("Error: Maximum length of the file name must be 14. The length of your file name is: %ld\n", path.length());
        return -1;
    }

    DataBlock _dataBlock = sid.dataBlocks[_block];
    for(int i=0;i<_dataBlock.getDirectoryEntriesSize();++i){
        DirectoryEntry _directoryEntry = _dataBlock.getDirectoryEntries()[i];

        if(strcmp(_directoryEntry.getFileName(),path.c_str()) == 0){
            if(sid.inodes[_directoryEntry.getInodeAddress()].type == DIRECTORY){
                sid.dataBlocks[_block].removeDirectoryEntry(_directoryEntry.getInodeAddress());
                flag = 1;
            }
        }
    }

    writeSIDToFS();

    if(flag == 0){
        return -1;
    }

    return 0;
}

int FileSystem::dumpe2fs(){
    readSIDFromFS();

    printf("Filesystem volume name: %s\n",filename.c_str());
    printf("Block Size: %d\n",sid.superBlock.getSuperBlockSize());
    printf("Inode Count: %d\n",sid.superBlock.getNumberOfInodes());
    printf("Block Count: %d\n",sid.superBlock.getNumberOfBlocks());
    printf("Free Inode Count: %d\n",sid.superBlock.getNumberOfFreeInodes());
    printf("Free Block Count: %d\n",sid.superBlock.getNumberOfFreeBlocks());
    printInodes();

    return 0;
}

int FileSystem::write(string path, string filename){
    readSIDFromFS();

    if(sid.superBlock.getNumberOfFreeInodes() == 0 || sid.superBlock.getNumberOfFreeBlocks() == 0){
        printf("Error: There is no free inode or free block.\n");
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
    if(_block == -1){
        return -1;
    }

    if(path.length() > 14){
        printf("Error: Maximum length of the file name must be 14. The length of your file name is: %ld\n", path.length());
        return -1;
    }

    if(sid.inodes[_block].type == REGULAR_FILE){
        return -1;
    }

    DataBlock _dataBlock = sid.dataBlocks[sid.inodes[_block].addressOfDiskBlocks[0]];
    for(int i=0;i<_dataBlock.getDirectoryEntriesSize();++i){
        DirectoryEntry _directoryEntry = _dataBlock.getDirectoryEntries()[i];

        if(strcmp(_directoryEntry.getFileName(), path.c_str()) == 0){
            return -1;
        }
    }

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

    char c;
    newfile.open(filename,ios::in);
    if (newfile.is_open()){
        newfile.seekg(-1,newfile.end);
        newfile.read(&c,1);
        newfile.close();
    }

    if(c != '\n'){
        fileContent.pop_back();
    }

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

    d_howManyBlock = currentBlocksSize/(double)maxBlockSize;
    i_howManyBlock = ceil(d_howManyBlock);

    int k=0;
    for(int i=0;i<8 && i<i_howManyBlock;++i){
        int addressSingleDB = -1;

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
            sid.dataBlocks[addressSingleDB].addAddress(currentBlocks[k]);
        }
    }

    _inode.setAddresses(currentBlocks,currentBlocksSize);
    sid.inodes[currentInodeNum] = _inode;

    int i=0;
    for (unsigned int j=0; j<fileContent.size(); j+=sid.superBlock.getBlockSize()) {
        string data = fileContent.substr(j,sid.superBlock.getBlockSize());
        sid.dataBlocks[currentBlocks[i]] = DataBlock(sid.superBlock.getBlockSize(),REGULAR_FILE);
        sid.dataBlocks[currentBlocks[i]].setData(data);
        i++;
    }

    DirectoryEntry _directoryEntry;
    _directoryEntry.setInodeAddress(currentInodeNum);
    _directoryEntry.setFileName(path.c_str());

    sid.dataBlocks[_block].addDirectoryEntry(_directoryEntry); 

    sid.superBlock.setNumberOfBlocks(currentBlocksSize + sid.superBlock.getNumberOfBlocks());
    sid.superBlock.setNumberOfFreeBlocks(sid.superBlock.getNumberOfFreeBlocks() - currentBlocksSize);

    writeSIDToFS();

    return 0;
}

int FileSystem::read(string path, string filename){
    readSIDFromFS();

    int _block = splitPath(&path);
    if(_block == -1){
        return -1;
    }

    
    int inode = -1;
    DataBlock _dataBlock = sid.dataBlocks[_block];
    for(int i=0;i<_dataBlock.getDirectoryEntriesSize();++i){
        DirectoryEntry _directoryEntry = _dataBlock.getDirectoryEntries()[i];

        if(strcmp(_directoryEntry.getFileName(), path.c_str()) == 0){
            inode = _directoryEntry.getInodeAddress();
            break;
        }
    }
    if(inode == -1){
        return -1;
    }

    int addresses[8], addressesSize=0;

    for(int i=0;i<8;++i){
        if(sid.inodes[inode].addressOfDiskBlocks[i] == -1){
            break;
        }

        addresses[i] = sid.inodes[inode].addressOfDiskBlocks[i];
        addressesSize++;
    }

    string str = "";

    for(int i=0;i<addressesSize;++i){
        DataBlock _db = sid.dataBlocks[addresses[i]];
        str.append(_db.getData());
    }

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

    if(strcmp(path.c_str(),"\\")==0){
        printf("Error: Root path can't be deleted\n");
        return -1;
    }

    int flag = 0, _block = splitPath(&path);
    if(_block == -1){
        return -1;
    }

    DataBlock _dataBlock = sid.dataBlocks[_block];
    for(int i=0;i<_dataBlock.getDirectoryEntriesSize();++i){
        DirectoryEntry _directoryEntry = _dataBlock.getDirectoryEntries()[i];

        if(strcmp(_directoryEntry.getFileName(),path.c_str()) == 0){
            if(sid.inodes[_directoryEntry.getInodeAddress()].type == REGULAR_FILE){
                sid.dataBlocks[_block].removeDirectoryEntry(_directoryEntry.getInodeAddress());
                flag = 1;
            }
        }
    }
    
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
        for(int i=0;i<_dblock.getDirectoryEntriesSize();++i){
            DirectoryEntry _directoryEntry = _dblock.getDirectoryEntries()[i];

            int cmp = strcmp(_directoryEntry.getFileName(),db);
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
    string token2, delimiter = "\\";
    while ((pos = path->find(delimiter)) != string::npos) {
        count++;
        token2 = path->substr(0, pos);
        path->erase(0, pos + delimiter.length());

        if(strcmp(token2.c_str(), "") == 0){
            continue;
        }
        _block = getDataBlock2(token2.c_str());
        if(_block == -1){
            // printf("Error\n");
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
    FILE *fptr = fopen(filename.c_str(), "wb");
    if(fptr == NULL){
        printf("Error: %s could not be opened.\n",filename.c_str());
        return -1;
    }

    fwrite(&sid.superBlock, sizeof(SuperBlock), 1, fptr);
    try{
        for(int i=0;i<NUMBEROF_INODE;i++){
            fwrite(&sid.inodes[i], sizeof(Inode), 1, fptr);
        }
    } catch(const std::exception& e){
        printf("Error: %s\n",e.what());
        return -1;
    }

    try{
        for(int i=0;i<NUMBEROF_BLOCK;i++){
            if(sid.dataBlocks[i].serialize(fptr, true) == -1){
                printf("Error: Writing Blocks Error: Block Number: %d\n",i);
                return -1;
            }
        }
    } catch(const std::exception& e){
        printf("Error: %s\n",e.what());
        return -1;
    }

    fclose(fptr);

    return 0;
}

int FileSystem::readSIDFromFS(){
    FILE *fptr = fopen(filename.c_str(), "rb");
    if(fptr == NULL){
        printf("Error: %s could not be opened.\n",filename.c_str());
        return -1;
    }

    fread(&sid.superBlock, sizeof(SuperBlock), 1, fptr);
    try{
        for(int i=0;i<NUMBEROF_INODE;i++){
            fread(&sid.inodes[i], sizeof(Inode), 1, fptr);
        }
    } catch(const std::exception& e){
        printf("-> Error: %s\n",e.what());
        return -1;
    }

    try{
        for(int i=0;i<NUMBEROF_BLOCK;i++){
            if(sid.dataBlocks[i].serialize(fptr, false) == -1){
                printf("Error: Reading Blocks Error: Block Number: %d\n",i);
                return -1;
            }
        }
    } catch(const std::exception& e){
        printf("-> Error: %s\n",e.what());
        return -1;
    }

    fclose(fptr);    

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

    printf("-------------------\n");
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

    printf("-------------------\n\n");

    return 0;
}

int FileSystem::sendCommand(int command, string path, string filename){
    int result = 0;

    printf("-------------------\n");
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

    printf("-------------------\n\n");

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

int FileSystem::printInodes(){
    readSIDFromFS();

    struct tm *tmp;

    for(int i=0;i<NUMBEROF_INODE;++i){
        Inode _inode = sid.inodes[i];

        if(_inode.isEmpty == 1){
            continue;
        }

        printf("Inode Number: %d\n",i);

        tmp = localtime(&_inode.changedTime);
        printf("Changed Time: %s",asctime(tmp));

        tmp = localtime(&_inode.modifiedTime);
        printf("Modified Time: %s",asctime(tmp));

        tmp = localtime(&_inode.accessTime);
        printf("Access Time: %s",asctime(tmp));

        if(_inode.type == ROOT_DIRECTORY){
            printf("Type: Root Directory\n");
        } else if(_inode.type == DIRECTORY){
            printf("Type: Directory\n");
        } else if(_inode.type == REGULAR_FILE){
            printf("Type: Regular File\n");
        }
        printf("Blocks: ");
        for(int i=0;i<8;++i){
            if(_inode.addressOfDiskBlocks[i] != -1){
                if(i == 7)
                    printf("%d ",_inode.addressOfDiskBlocks[i]);
                else
                    printf("%d, ",_inode.addressOfDiskBlocks[i]);
            }
        }
        printf("\n---\n");
    }

    return 0;
}