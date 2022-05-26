CC = g++
CFLAGS = -Wall -g
MAIN = main

$(MAIN): $(MAIN).o filesystem.o datablock.o inode.o
	$(CC) $(CFLAGS) -o $(MAIN) $(MAIN).o filesystem.o datablock.o inode.o

$(MAIN).o: $(MAIN).cpp filesystem.h inode.h superblock.h directoryentry.h datablock.h
	$(CC) $(CFLAGS) -c $(MAIN).cpp

filesystem.o: filesystem.h
	$(CC) $(CFLAGS) -c filesystem.cpp

datablock.o: datablock.h
	$(CC) $(CFLAGS) -c datablock.cpp

inode.o: inode.h
	$(CC) $(CFLAGS) -c inode.cpp

clean: 
	rm $(MAIN) *.o