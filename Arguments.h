#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <string>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/mman.h>

using namespace std;

class Arguments {
    private:
        string image;
        string fileName;
        bool testMMap = false;
        bool testBootSector = false;
        bool testDirectoryEntry = false;
        bool testFileClusters = false;
        bool testFileName = false;
        bool testFileContents = false;
        bool testNumEntries = false;
        bool testSpaceUsage = false;
        bool testLargestFile = false;
        bool testCookie = false;
        bool testNumDirLevels = false;
        bool testOldestFile = false;
        bool outputFsData = false;
        bool writeFsData = false;
        bool verbose = false;
        bool debug = false;
        bool valid = true;

        unsigned long directoryEntryIndex = 0;
        unsigned long clusterEntryIndex = 0;
        string prepend = "";

    public:
        Arguments(int, char**);

        string getImage();
        string getFileName();
        bool isTestMMap();
        bool isTestBootSector();
        bool isTestDirectoryEntry();
        bool isTestFileClusters();
        bool isTestFileName();
        bool isTestFileContents();
        bool isTestNumEntries();
        bool isTestSpaceUsage();
        bool isTestLargestFile();
        bool isTestCookie();
        bool isTestNumDirLevels();
        bool isTestOldestFile();
        bool isOutputFsData();
        bool isWriteFsData();
        unsigned long getDirectoryEntryIndex();
        unsigned long getClusterEntryIndex();
        bool getValid();
        bool isVerbose();
        bool isDebug();
};

#endif