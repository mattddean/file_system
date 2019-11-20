#include <string>
#include <getopt.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>
#include <algorithm>

#include "Arguments.h"
#include "Value.h"
#include "Image.h"
#include "DirectoryEntry.h"
#include "FileName.h"

using namespace std;

void testMMap(void *file) {
    string line;
    while (getline(cin, line)) {
        Value value{line, file, ""};
        cout << value;
        cout << endl;
    }
}

void testBootSector(Image image) {
    cout << image.getInfo();
}

void testDirectoryEntry(Image image, int directoryEntryIndex) {
    DirectoryEntry entry = image.getDirectoryEntry(directoryEntryIndex, true);
    cout << entry.getInfo();
}

void testFileClusters(Image image, unsigned long clusterEntryIndex) {
    switch(clusterEntryIndex) {
        case 0:
        case 1:
            cout << "EOF" << endl;
            return;
        default:
            break;
    }

    cout << clusterEntryIndex << " -> ";

    while(true) {
        unsigned value = image.getNextClusterEntryIndex(clusterEntryIndex).toInt();
        switch(value) {
            case 0:
                cout << "EOF" << endl;
                return;
            case 65535:
                cout << "EOF" << endl;
                return;
            default:
                cout << value << " -> ";
                clusterEntryIndex = value;
                break;
        }
    }
}

void testFileName(Image image, string fileName) {
    DirectoryEntry entry = image.getDirectoryEntry(fileName);
    if (entry.isValid()) cout << entry.getInfo();
}

void testFileContents(Image image, string fileName) {
    DirectoryEntry entry = image.getDirectoryEntry(fileName);
    if (entry.isValid()) {
        char *contents = (char *)image.getCluster(entry.getStartCluster().toInt());
        string stringContents = string{contents};
        cout << stringContents;
    }
}

void testNumEntries(Image image) {
    cout << image.getNumEntries();
}

void testLargestFile(Image image) {
    cout << image.getLargestFile();
}

int firstEmptyCluster(Image image) {
    unsigned long maxNumFatEntries = (image.getNumSectorsPerFat().toInt() * image.getNumBytesPerSector().toInt()) / 2; // 2 bytes per entry
    for (unsigned long i = 2; i < maxNumFatEntries; i++) {
        int fatEntryIndex = Value('s', image.getFatsOffset() + (2 * i), image.getFile(), "").toInt();
        if (fatEntryIndex == 0) return fatEntryIndex;
    }
    return 0;
}

void writeFsData(Image image) {
    string vfs2Answers = "Number of files in root directory: 23\nNumber of files in the file system: 1680\nNumber of directories in the file system: 123\nTotal capacity of the file system: 524288000\nTotal allocated space: 50683904\nTotal size of files: 42642658\nUnused, but allocated, space (for files): 8041246\nUnallocated space: 473604096\nLargest file (50949 bytes): /CBLVKISA/RUJZVLTX/AWMKOIZE.TXT\nPath to file with cookie: /CBLVKISA/RUJZVLTX/TLVIHSYQ.TXT\nStarting cluster for file with cookie: 5\nDirectory hierarchy levels: 7\nOldest file: /CBLVKISA/RUJZVLTX/TLVIHSYQ.TXT    ";


    int emptyClusterIndex = firstEmptyCluster(image);
    short *emptyEntryPointer = (short*)Value('s', image.getFatsOffset() + (2 * emptyClusterIndex), image.getFile(), "").getRawMemory();
    *emptyEntryPointer = 65535;

    char *clusterPointer = (char*)image.getCluster(emptyClusterIndex);
    memcpy(clusterPointer, vfs2Answers.c_str(), strlen(vfs2Answers.c_str()));
}

int main(int argc, char** argv) {
    Arguments arguments{argc, argv};
    int inputFd = open(arguments.getImage().c_str(), O_RDONLY);
    
    struct stat fileStats;  
    fstat(inputFd, &fileStats);

    void *file = mmap(NULL, fileStats.st_size, PROT_READ|PROT_WRITE, MAP_PRIVATE, inputFd, 0);

    Image image{file, arguments.getValid()};

    if (arguments.isTestMMap()) {
        testMMap(file);
    }
    else if (arguments.isTestBootSector()) {
        testBootSector(image);
    }
    else if (arguments.isTestDirectoryEntry()) {
        testDirectoryEntry(image, arguments.getDirectoryEntryIndex());
    }
    else if (arguments.isTestFileClusters()) {
        testFileClusters(image, arguments.getClusterEntryIndex());
    }
    else if (arguments.isTestFileName()) {
        testFileName(image, arguments.getFileName());
    }
    else if (arguments.isTestFileContents()) {
        testFileContents(image, arguments.getFileName());
    }
    else if(arguments.isTestNumEntries()) {
        testNumEntries(image);
    }
    else if(arguments.isTestLargestFile()) {
        testLargestFile(image);
    }
    else if(arguments.isWriteFsData()) {
        writeFsData(image);
    }

    munmap(file, fileStats.st_size);
}