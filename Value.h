#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <getopt.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <string.h>
#include <sstream>

using namespace std;

enum ValueType {
    CHAR,
    UNSIGNED_BYTE,
    UNSIGNED_BYTE_DECIMAL,
    TWO_BYTE,
    UNSIGNED_TWO_BYTE,
    FOUR_BYTE,
    UNSIGNED_FOUR_BYTE,
    STRING,
    DYNAMIC_LENGTH
};

enum PrintType {
    NORMAL,
    HEXADECIMAL,
    DECIMAL
};

class Value {
    private:
        void *file;
        ValueType type;
        PrintType printType;
        int size;
        void setTypeAndSize(char);
        void *retrieveMemoryFromFile(int);
        void determineType(char, int);
        void *rawMemory;
        string prepend;
    public:
        Value();
        Value(string, void*, string, int l=0);
        Value(char, unsigned long, void*, string, int l=0);
        void *getRawMemory();
        ValueType getType();
        void *getFile();
        friend ostream& operator<<(std::ostream&, const Value&);
        string toString();
        int toInt();
};

#endif