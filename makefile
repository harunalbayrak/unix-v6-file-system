CC = g++
CFLAGS = -Wall -g
MAIN = main

$(MAIN): obj/$(MAIN).o obj/filesystem.o obj/superblock.o obj/inode.o obj/datablock.o obj/directoryentry.o
	$(CC) $(CFLAGS) -o $(MAIN) obj/$(MAIN).o obj/filesystem.o obj/superblock.o obj/inode.o obj/datablock.o obj/directoryentry.o

obj/$(MAIN).o: $(MAIN).cpp include/filesystem.h include/inode.h include/superblock.h include/directoryentry.h include/datablock.h
	$(CC) $(CFLAGS) -c $(MAIN).cpp -o obj/main.o

obj/filesystem.o: include/filesystem.h
	$(CC) $(CFLAGS) -c src/filesystem.cpp -o obj/filesystem.o

obj/superblock.o: include/superblock.h
	$(CC) $(CFLAGS) -c src/superblock.cpp -o obj/superblock.o

obj/inode.o: include/inode.h
	$(CC) $(CFLAGS) -c src/inode.cpp -o obj/inode.o

obj/datablock.o: include/datablock.h
	$(CC) $(CFLAGS) -c src/datablock.cpp -o obj/datablock.o

obj/directoryentry.o: include/directoryentry.h
	$(CC) $(CFLAGS) -c src/directoryentry.cpp -o obj/directoryentry.o

clean: 
	rm $(MAIN) obj/*.o