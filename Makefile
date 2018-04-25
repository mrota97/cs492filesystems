CC = gcc
CFLAGS = -g -Wall

TARGET = FileSystem

default: filesystem

filesystem: main.o
	$(CC) $(CFLAGS) -o filesystem main.o

main.o: main.cpp filetree.h ldisk.h lfile.h
	$(CC) $(CFLAGS) -c main.cpp

clean:
	$(RM) filesystem *.o *~
