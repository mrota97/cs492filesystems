CC = gcc
CFLAGS = -g -Wall -lstdc++

TARGET = FileSystem

default: filesystem

filesystem: filesystem.o
	$(CC) $(CFLAGS) -o filesystem filesystem.o

filesystem.o: filesystem.cpp filetree.h ldisk.h lfile.h
	$(CC) $(CFLAGS) -c filesystem.cpp

clean:
	$(RM) filesystem *.d *.o *~
