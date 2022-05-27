#include "include/superblock.h"
#include "include/directoryentry.h"
#include "include/inode.h"
#include "include/filesystem.h"

using namespace std;

int getBlockSize(int argc, char **argv);
string getFileSystemFilename(int argc, char **argv);

int main(int argc, char **argv){
    int blockSize = getBlockSize(argc, argv);
    if(blockSize <= 0){
        return -1;
    }

    string fileSystemFilename = getFileSystemFilename(argc, argv);

    FileSystem fileSystem(blockSize, fileSystemFilename);

    char str[] = "/usr";
    char str2[] = "/usr/harun";
    char str3[] = "/usr/harun2";
    char str4[] = "/usr/harun3";
    char str5[] = "/usr/harun/selam";
    char str6[] = "/usr/harun/selam2";
    char str7[] = "/usr/harun3/selam3";
 
    fileSystem.makeFileSystem();

    fileSystem.sendCommand(1,str);
    fileSystem.sendCommand(1,str2);
    fileSystem.sendCommand(1,str3);
    fileSystem.sendCommand(1,str4);
    fileSystem.sendCommand(1,str5);
    fileSystem.sendCommand(1,str6);
    fileSystem.sendCommand(1,str7);
    fileSystem.sendCommand(0,"/");
    fileSystem.sendCommand(0,"/usr");
    fileSystem.sendCommand(0,"/usr/harun");
    fileSystem.sendCommand(0,"/usr/harun3");
    fileSystem.sendCommand(0,"/usr/harun00");
    fileSystem.sendCommand(2,"/usr/harun");
    fileSystem.sendCommand(0,"/usr");
    fileSystem.sendCommand(4,"/usr/deneme.txt","linuxData.data");
    fileSystem.sendCommand(0,"/usr");
    fileSystem.sendCommand(5,"/usr/deneme.txt","ReadDeneme.txt");
    fileSystem.sendCommand(6,"/usr/deneme.tx2t");
    fileSystem.sendCommand(0,"/usr");

    // fileSystem.dir(dd);
    // fileSystem.mkdir(str);
    // fileSystem.mkdir(str2);
    // fileSystem.mkdir(str3);
    // fileSystem.mkdir(str4);
    // fileSystem.mkdir(str5);
    // fileSystem.mkdir(str6);
    // fileSystem.mkdir(str7);
    // fileSystem.dir("/");
    // printf("----\n");
    // fileSystem.dir("/usr");
    // printf("----\n");
    // fileSystem.dir("/usr/harun");
    // printf("----\n");
    // fileSystem.dir("/usr/harun3");
    // printf("----\n");
    // fileSystem.rmdir("/usr/harun");
    // fileSystem.dir("/usr");

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