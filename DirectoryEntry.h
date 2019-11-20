#ifndef DIRECTORY_ENTRY_H
#define DIRECTORY_ENTRY_H

#include <string>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <vector>
#include <algorithm>

#include "Value.h"
#include "FileName.h"

#define ROOT_DIRECTORY_SIZE 32
#define ATR_SIZE 7

using namespace std;

class DirectoryEntry {
    private:
        bool valid;

        void *file;

        unsigned long enclosingOffset; // the offset of the subdirectory that this directory entry lies within
        unsigned long maxNumDirectoryEntriesInSubdirectory;
        unsigned long rootDirectoryOffset;

        FileName name;
        vector<string> attributes;
        string createTime;
        string accessDate;
        Value extendedAttributes;
        string modifyTime;
        Value startCluster;
        Value size;
        string info;
        bool isDotEntry = false;
        bool isCountableSubdirectory = false;
        bool isCountableFile = false;

        string fullPath;

        vector<DirectoryEntry> subdirectories;
        
        string getDate(int);
        string getTime(int thisTime, int milliseconds=0);

        void calculateAttributes();
        void calculateCreateTime();
        void calculateAccessDate();
        void calculateExtendedAttributes();
        void calculateModifyTime();
        void calculateStartCluster();
        void calculateSize();
        void calculateName();

        void calculateInfo();

        void calculateIsDotEntry();
        void calculateIsCountableSubdirectory();
        void calculateIsCountableFile();

        // void calculateFullPath();

    public:
        bool isValid();

        unsigned long offset;
        DirectoryEntry();
        DirectoryEntry(unsigned long, void*, unsigned long, unsigned long, unsigned long);

        FileName getName();

        vector<string> getAttributes();
        string getCreateTime();
        string getAccessDate();
        Value getExtendedAttributes();
        string getModifyTime();
        Value getStartCluster();
        Value getSize();

        bool getIsDotEntry();
        bool getIsCountableSubdirectory();
        bool getIsCountableFile();
        
        string getInfo();

        string getFullPath();
};

#endif