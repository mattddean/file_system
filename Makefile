# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -std=c++17

# for cleaning
RM = rm

# the build target executable:
TARGET = fs

all: $(TARGET)

Arguments.o: Arguments.cpp
	$(CC) $(CFLAGS) -c Arguments.cpp

Value.o: Value.cpp
	$(CC) $(CFLAGS) -c Value.cpp
	
Image.o: Image.cpp
	$(CC) $(CFLAGS) -c Image.cpp

DirectoryEntry.o: DirectoryEntry.cpp
	$(CC) $(CFLAGS) -c DirectoryEntry.cpp

FileName.o: FileName.cpp
	$(CC) $(CFLAGS) -c FileName.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

$(TARGET): main.o Arguments.o Value.o Image.o DirectoryEntry.o FileName.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o Arguments.o Value.o Image.o DirectoryEntry.o FileName.o

clean:
	$(RM) ./*.o
	$(RM) $(TARGET)
