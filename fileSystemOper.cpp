#include "include/superblock.h"
#include "include/directoryentry.h"
#include "include/inode.h"
#include "include/filesystem.h"
#include <iostream>
#include <cstring>

using namespace std;

string getFileSystemFilename(int argc, char **argv);
string getFileOperation(int argc, char **argv);
string getFilePath(int argc, char **argv);
string getFileName(int argc, char **argv);

int main(int argc, char **argv){
    if(argc < 3 || argc > 5){
        printf("Error: argument number must be between 3 and 5.\n");
        return -1;
    }

    string fileSystemFilename;
    string fileOperation;
    string filePath;
    string fileName;

    if(argc >= 2){
        fileSystemFilename = getFileSystemFilename(argc, argv);
    }
    if(argc >= 3){
        fileOperation = getFileOperation(argc, argv);
    }
    if(argc >= 4){
        filePath = getFilePath(argc, argv);
    }
    if(argc >= 5){
        fileName = getFileName(argc, argv);
    }
    
    FileSystem fileSystem = FileSystem(fileSystemFilename);

    if(fileOperation.compare("dir") == 0){
        fileSystem.sendCommand(0,filePath);
    } else if(fileOperation.compare("mkdir") == 0){
        fileSystem.sendCommand(1,filePath);
    } else if(fileOperation.compare("rmdir") == 0){
        fileSystem.sendCommand(2,filePath);
    } else if(fileOperation.compare("dumpe2fs") == 0){
        fileSystem.sendCommand(3,filePath);
    } else if(fileOperation.compare("write") == 0){
        fileSystem.sendCommand(4,filePath,fileName);
    } else if(fileOperation.compare("read") == 0){
        fileSystem.sendCommand(5,filePath,fileName);
    } else if(fileOperation.compare("del") == 0){
        fileSystem.sendCommand(6,filePath);
    } else{
        printf("Error: Invalid Usage\n");
        return -1;
    }

    return 0;
}

string getFileSystemFilename(int argc, char **argv){
    try{
        return string(argv[1]);
    } catch(const std::exception& e){
        printf("Error: %s\n", e.what());
        return NULL;
    }
}

string getFileOperation(int argc, char **argv){
    try{
        return string(argv[2]);
    } catch(const std::exception& e){
        // printf("Error: %s\n", e.what());
        return string("");
    }
}

string getFilePath(int argc, char **argv){
    try{
        return string(argv[3]);
    } catch(const std::exception& e){
        // printf("Error: %s\n", e.what());
        return string("");
    }
}

string getFileName(int argc, char **argv){
    try{
        return string(argv[4]);
    } catch(const std::exception& e){
        // printf("Error: %s\n", e.what());
        return string("");
    }
}