#include "Arguments.h"

Arguments::Arguments(int argc, char **argv) {
    while (1) {
        int option_index = 0;

        static struct option long_options[] = {
            {"image",                required_argument, 0, 'i'},
            {"test-mmap",            no_argument,       0, 'm'},
            {"test-boot-sector",     no_argument,       0, 'b'},
            {"test-directory-entry", required_argument, 0, 'e'},
            {"test-file-clusters",   required_argument, 0, 'c'},
            {"test-file-name",       required_argument, 0, 'n'},
            {"test-file-contents",   required_argument, 0, 'f'},
            {"invalid-image",        no_argument,       0, 'z'},
            {"test-num-entries",     no_argument,       0, 't'},
            {"test-space-usage",     no_argument,       0, 's'},
            {"test-largest-file",    no_argument,       0, 'l'},
            {"test-cookie",          no_argument,       0, 'o'},
            {"test-num-dir-levels",  no_argument,       0, 'r'},
            {"test-oldest-file",     no_argument,       0, 'y'},
            {"output-fs-data",       no_argument,       0, 'a'},
            {"write-fs-data",        no_argument,       0, 'w'},
            {0,                      0,                 0,  0 }
        };

        int c = getopt_long(argc, argv, "i:mbvd",
                long_options, &option_index);
        
        if (c == -1) {
            break;
        }

        switch (c) {
        case 0:
        case 1:
            break;
                
        case 'i':
            image = optarg;
            break;

        case 'm':
            testMMap = true;
            break;
                
        case 'b':
            testBootSector = true;
            break;
        
        case 'v':
            verbose = true;
            break;

        case 'd':
            debug = true;
            break;

        case 'e':
            testDirectoryEntry = true;
            directoryEntryIndex = (unsigned long)atoi(optarg);
            break;

        case 'c':
            testFileClusters = true;
            clusterEntryIndex = (unsigned long)atoi(optarg);
            break;

        case 'n':
            testFileName = true;
            fileName = string{optarg};
            break;

        case 'f':
            testFileContents = true;
            fileName = string{optarg};
            break;
        
        case 't':
            testNumEntries = true;
            break;

        case 's':
            testSpaceUsage = true;
            break;

        case 'l':
            testLargestFile = true;
            break;

        case 'o':
            testCookie = true;
            break;

        case 'r':
            testNumDirLevels = true;
            break;

        case 'y':
            testOldestFile = true;
            break;

        case 'a':
            outputFsData = true;
            break;

        case 'z':
            valid = false;
            break;

        case 'w':
            writeFsData = true;
            break;

        default:
            break;
        }
    }
}

string Arguments::getImage() {
    return image;
}

string Arguments::getFileName() {
    return fileName;
}

bool Arguments::isTestMMap() {
    return testMMap;
}

bool Arguments::isTestBootSector() {
    return testBootSector;
}

bool Arguments::isTestDirectoryEntry() {
    return testDirectoryEntry;
}

bool Arguments::isTestFileClusters() {
    return testFileClusters;
}

bool Arguments::isTestFileName() {
    return testFileName;
}

bool Arguments::isTestFileContents() {
    return testFileContents;
}

bool Arguments::isTestNumEntries() {
    return testNumEntries;
}

bool Arguments::isTestSpaceUsage() {
    return testSpaceUsage;
}

bool Arguments::isTestLargestFile() {
    return testLargestFile;
}

bool Arguments::isTestCookie() {
    return testCookie;
}

bool Arguments::isTestNumDirLevels() {
    return testNumDirLevels;
}

bool Arguments::isTestOldestFile() {
    return testOldestFile;
}

bool Arguments::isOutputFsData() {
    return outputFsData;
}

bool Arguments::isWriteFsData() {
    return writeFsData;
}

unsigned long Arguments::getDirectoryEntryIndex() {
    return directoryEntryIndex;
}

unsigned long Arguments::getClusterEntryIndex() {
    return clusterEntryIndex;
}

bool Arguments::isVerbose() {
    return verbose;
}

bool Arguments::isDebug() {
    return debug;
}

bool Arguments::getValid() {
    return valid;
}