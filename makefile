CC = g++
CFLAGS = -Wall -g
MAIN = main

$(MAIN): $(MAIN).o filesystem.o superblock.o inode.o datablock.o
	$(CC) $(CFLAGS) -o $(MAIN) $(MAIN).o filesystem.o superblock.o inode.o datablock.o 

$(MAIN).o: $(MAIN).cpp filesystem.h inode.h superblock.h directoryentry.h datablock.h
	$(CC) $(CFLAGS) -c $(MAIN).cpp

filesystem.o: filesystem.h
	$(CC) $(CFLAGS) -c filesystem.cpp

superblock.o: superblock.h
	$(CC) $(CFLAGS) -c superblock.cpp

inode.o: inode.h
	$(CC) $(CFLAGS) -c inode.cpp

datablock.o: datablock.h
	$(CC) $(CFLAGS) -c datablock.cpp

clean: 
	rm $(MAIN) *.o