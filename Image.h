#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <vector>
#include <algorithm>

#include "DirectoryEntry.h"
#include "Value.h"

#define ROOT_DIRECTORY_SIZE 32

using namespace std;

class Image {
    private:
        void *file;

        unsigned long directoryEntriesOffset;
        unsigned long fatsOffset;
        unsigned long dataRegionOffset;

        Value oem;
        Value numBytesPerSector;
        Value numSectorsPerCluster;
        Value numReservedSectors;
        Value numFats;
        Value maxNumDirectoryEntries;
        unsigned long maxNumDirectoryEntriesInSubdirectory;
        Value numTotalSectors;
        Value mediaDescriptor;
        Value numSectorsPerFat;
        Value numLogicalSectors;
        int numBytesPerCluster;
        int largestFileSize = 0;
        string largestFileName;

        unsigned long numDirectories = 0;
        unsigned long numFiles = 0;
        unsigned long numFilesInRootDirectory = 0;

        string info;

        void calculateOem();
        void calculateNumBytesPerSector();
        void calculateNumSectorsPerCluster();
        void calculateNumReservedSectors();
        void calculateNumFats();
        void calculateMaxNumDirectoryEntries();
        void calculateMaxNumDirectoryEntriesInSubdirectory();
        void calculateNumTotalSectors();
        void calculateMediaDescriptor();
        void calculateNumSectorsPerFat();
        void calculateNumLogicalSectors();
        void calculateNumBytesPerCluster();
        void calculateInfo();

        string numEntries;
        string largestFile;
        string additionalInfo;

        void traverseImage();
        void processDirectoryEntry(DirectoryEntry, bool);
        void enumerateSubdirectoryEntries(DirectoryEntry);

        void calculateNumEntries();
        void calculateLargestFile();
        void calculateAdditionalInfo();

    public:
        Image(void* file, bool valid);

        void* getFile();

        unsigned long getFatsOffset();
        unsigned long getDirectoryEntriesOffset();
        unsigned long getDataRegionOffset();
        
        Value getOem();
        Value getNumBytesPerSector();
        Value getNumSectorsPerCluster();
        Value getNumReservedSectors();
        Value getNumFats();
        Value getMaxNumDirectoryEntries();
        int getMaxNumDirectoryEntriesInSubdirectory();
        Value getNumTotalSectors();
        Value getMediaDescriptor();
        Value getNumSectorsPerFat();
        Value getNumLogicalSectors();

        unsigned long getNumBytesPerCluster();

        vector<string> splitFilePath(string);
        FileName parseFileString(string);
        void getNextStartCluster(DirectoryEntry, vector<string>, unsigned long&);
        bool checkDirectoryEntry(DirectoryEntry, vector<string>);
        bool evaluateDirectoryEntry(DirectoryEntry, FileName, bool, unsigned long&);

        DirectoryEntry getDirectoryEntry(unsigned long index, bool root);
        DirectoryEntry getDirectoryEntry(unsigned long directoryIndex, unsigned long clusterIndex);
        DirectoryEntry getDirectoryEntry(string fileName);
        Value getNextClusterEntryIndex(unsigned long clusterEntryIndex);

        void* getCluster(unsigned long clusterEntryIndex);
        unsigned long getClusterOffset(unsigned long clusterEntryIndex);

        string getInfo();

        string getNumEntries();
        string getLargestFile();

        string getAdditionalInfo();
};

#endif