#include "include/superblock.h"
#include "include/directoryentry.h"
#include "include/inode.h"
#include "include/filesystem.h"

using namespace std;

int getBlockSize(int argc, char **argv);
string getFileSystemFilename(int argc, char **argv);

int main(int argc, char **argv){
    int blockSize = getBlockSize(argc, argv);
    if(blockSize <= 0 || blockSize >= 9){
        printf("Error: Bad Block Size\n");
        return -1;
    }

    string fileSystemFilename = getFileSystemFilename(argc, argv);

    FileSystem fileSystem(blockSize, fileSystemFilename);

    fileSystem.makeFileSystem();

    return 0;
}

int getBlockSize(int argc, char **argv){
    if(argc != 3){
        return -1;
    }

    int blockSize = 0;

    try{
        blockSize = atoi(argv[1]); 
    } catch(const std::exception& e){
        printf("Error: %s\n", e.what());
    }

    return blockSize;
}

string getFileSystemFilename(int argc, char **argv){
    try{
        return string(argv[2]);
    } catch(const std::exception& e){
        printf("Error: %s\n", e.what());
        return NULL;
    }
}