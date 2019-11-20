#ifndef FILE_NAME_H
#define FILE_NAME_H

#include <string>
#include <stdlib.h>
#include <iostream>
#include <getopt.h>
#include <sys/mman.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sstream>

using namespace std;

enum FileNameStatus {
    EMPTY,
    PREVIOUSLY_ERASED,
    GOOD
};

class FileName {    
    public:
    string name;
    string extension;
    FileNameStatus status;
    friend bool operator==(const FileName &f1, const FileName &f2);
    string toString();
    string toTrimmedString();
};

#endif