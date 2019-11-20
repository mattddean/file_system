#include "Image.h"
#include "Value.h"
#include "DirectoryEntry.h"

Image::Image(void* file, bool valid) {
    this->file = file;
    calculateOem();
    calculateNumBytesPerSector();
    calculateNumSectorsPerCluster();
    calculateNumReservedSectors();
    calculateNumFats();
    calculateMaxNumDirectoryEntries();
    calculateNumTotalSectors();
    calculateMediaDescriptor();
    calculateNumSectorsPerFat();
    calculateNumLogicalSectors();
    calculateNumBytesPerCluster();

    calculateInfo();

    if (valid) {
        calculateMaxNumDirectoryEntriesInSubdirectory();

        this->fatsOffset = this->numReservedSectors.toInt() * this->numBytesPerSector.toInt();

        this->directoryEntriesOffset = ((this->numReservedSectors.toInt() + (this->numFats.toInt() * this->numSectorsPerFat.toInt())) * this->numBytesPerSector.toInt());

        this->dataRegionOffset = this->directoryEntriesOffset + (this->maxNumDirectoryEntries.toInt() * ROOT_DIRECTORY_SIZE);

        traverseImage();
        calculateNumEntries();
        calculateLargestFile();
        calculateAdditionalInfo();
    }
}

void Image::calculateOem() {
    this->oem = Value("z 3", file, "");
}

void Image::calculateNumBytesPerSector() {
    this->numBytesPerSector = Value("s 11", file, "");
}

void Image::calculateNumSectorsPerCluster() {
    this->numSectorsPerCluster = Value("a 13", file, "");
}

void Image::calculateNumReservedSectors() {
    this->numReservedSectors = Value("s 14", file, "");
}

void Image::calculateNumFats() {
    this->numFats = Value("a 16", file, "");
}

void Image::calculateMaxNumDirectoryEntries() {
    this->maxNumDirectoryEntries = Value("s 17", file, "");
}

void Image::calculateMaxNumDirectoryEntriesInSubdirectory() {
    this->maxNumDirectoryEntriesInSubdirectory = getNumBytesPerCluster() / ROOT_DIRECTORY_SIZE;
}

void Image::calculateNumTotalSectors() {
    this->numTotalSectors = Value("i 32", file, "");
}

void Image::calculateMediaDescriptor() {
    this->mediaDescriptor = Value("b 21", file, "");
}

void Image::calculateNumSectorsPerFat() {
    this->numSectorsPerFat = Value("s 22", file, "");
}

void Image::calculateNumLogicalSectors() {
    Value value{"s 19", file, ""};
    if (value.toInt() == 0) {
        this->numLogicalSectors = Value("i 32", file, "");
    }
    else {
        this->numLogicalSectors = value;
    }
}

void Image::calculateNumBytesPerCluster() {
    this->numBytesPerCluster = this->numBytesPerSector.toInt() * this->numSectorsPerCluster.toInt();
}

void Image::calculateInfo() {
    stringstream stream;

    stream << "OEM: " << this->oem << endl;
    stream << "Bytes per sector: " << this->numBytesPerSector << endl;
    stream << "Sectors per cluster: " << this->numSectorsPerCluster << endl;
    stream << "Reserved sectors: " << this->numReservedSectors << endl;
    stream << "Num FATs: " << this->numFats << endl;
    stream << "Max root directory entries: " << this->maxNumDirectoryEntries << endl;
    stream << "Num logical sectors: " << this->numLogicalSectors << endl;
    stream << "Media Descriptor: " << this->mediaDescriptor << endl;
    stream << "Sectors per FAT: " << this->numSectorsPerFat << endl;

    this->info = stream.str();
}

void* Image::getFile() {
    return this->file;
}

unsigned long Image::getFatsOffset() {
    return this->fatsOffset;
}

unsigned long Image::getDirectoryEntriesOffset() {
    return this->directoryEntriesOffset;
}

unsigned long Image::getDataRegionOffset() {
    return this->dataRegionOffset;
}

Value Image::getOem() {
    return this->oem;
}

Value Image::getNumBytesPerSector() {
    return this->numBytesPerSector;
}

Value Image::getNumSectorsPerCluster() {
    return this->numSectorsPerCluster;
}

Value Image::getNumReservedSectors() {
    return this->numReservedSectors;
}

Value Image::getNumFats() {
    return this->numFats;
}

Value Image::getMaxNumDirectoryEntries() {
    return this->maxNumDirectoryEntries;
}

int Image::getMaxNumDirectoryEntriesInSubdirectory() {
    return this->maxNumDirectoryEntriesInSubdirectory;
}

Value Image::getNumTotalSectors() {
    return this->numTotalSectors;
}

Value Image::getMediaDescriptor() {
    return this->mediaDescriptor;
}

Value Image::getNumSectorsPerFat() {
    return this->numSectorsPerFat;
}

Value Image::getNumLogicalSectors() {
    return this->numLogicalSectors;
}

unsigned long Image::getNumBytesPerCluster() {
    return this->numBytesPerCluster;
}

vector<string> Image::splitFilePath(string str) {
    vector<string> tokens;
    stringstream stream{str};
    string temp;
    int index = 0;
    while (getline(stream, temp, '/')) {
        if (index > 0) {
            tokens.push_back(temp);
        }
        index++;
    }
    return tokens;
}

FileName Image::parseFileString(string str) {
    FileName name;
    int dotPos = str.find('.');
    if (dotPos == -1) {
        name.name = string(str);
        name.extension = string("");
        return name;
    }
    name.name = str.substr(0, dotPos);
    name.extension = str.substr(dotPos + 1);
    name.status = GOOD;
    return name;
}

void Image::getNextStartCluster(DirectoryEntry entry, vector<string> atrs, unsigned long& startCluster) {
    if (find(atrs.begin(), atrs.end(), "subdir") != atrs.end()) { // subdir is in atrs
        startCluster = (unsigned long)entry.getStartCluster().toInt();
    }
}

bool Image::checkDirectoryEntry(DirectoryEntry entry, vector<string> atrs) {
    if (find(atrs.begin(), atrs.end(), "subdir") == atrs.end()) { // subdir is not in atrs
        return true;
    }
    return false;
}

// returns true if it found the file
bool Image::evaluateDirectoryEntry(DirectoryEntry entry, FileName nameToFind, bool atLastSegmentOfPath, unsigned long& startCluster) {
    FileName currentName = entry.getName();
    vector<string> atrs = entry.getAttributes();
    if (nameToFind == currentName) {
        if (atLastSegmentOfPath) { // we're at the last piece of the path so we're looking for the file
            return checkDirectoryEntry(entry, atrs);
        } 
        else { // we're not at the last piece of the path so we're looking for a subdirectory
            getNextStartCluster(entry, atrs, startCluster);
            return false;
        }
    }
    return false;
}

DirectoryEntry Image::getDirectoryEntry(unsigned long index, bool root) { // a root directory entry
    return DirectoryEntry(index, file, directoryEntriesOffset, this->maxNumDirectoryEntriesInSubdirectory, this->directoryEntriesOffset);
}

DirectoryEntry Image::getDirectoryEntry(unsigned long directoryIndex, unsigned long clusterIndex) { // not a root directory entry
    return DirectoryEntry(directoryIndex, file, this->getClusterOffset(clusterIndex), this->maxNumDirectoryEntriesInSubdirectory, this->directoryEntriesOffset);
}

DirectoryEntry Image::getDirectoryEntry(string fileName) {
    DirectoryEntry entry{};
    vector<string> tokens = splitFilePath(fileName);
    unsigned long startCluster = 0;
    for (unsigned i = 0; i < tokens.size(); i++) {
        FileName nameToFind = parseFileString(tokens[i]);
        bool atLastSegmentOfPath = i == tokens.size() - 1;
        if (i == 0) { // at root
            for (unsigned long j = 0; j < (unsigned long)this->maxNumDirectoryEntries.toInt(); j++) {
                entry = this->getDirectoryEntry(j, true);
                if (evaluateDirectoryEntry(entry, nameToFind, atLastSegmentOfPath, startCluster)) {
                    return entry;
                }
            }
        }
        else { // in subdirectory
            for (unsigned long j = 0; j < (unsigned long)this->maxNumDirectoryEntriesInSubdirectory; j++) {
                entry = this->getDirectoryEntry(j, startCluster);
                if (evaluateDirectoryEntry(entry, nameToFind, atLastSegmentOfPath, startCluster)) {
                    return entry;
                }
            }
        }
    }
    return entry;
}

void Image::processDirectoryEntry(DirectoryEntry entry, bool isRootDirectoryEntry) {
    if (entry.getIsCountableSubdirectory()) {
        // num entries
        this->numDirectories++;
    }
    else if (entry.getIsCountableFile()) {
        // num entries
        if (isRootDirectoryEntry) {
            this->numFilesInRootDirectory++;
        }
        this->numFiles++;

        // largest file
        int fileSize = entry.getSize().toInt();
        if (fileSize > this->largestFileSize) {
            this->largestFileSize = fileSize;
            this->largestFileName = entry.getFullPath() + '\n';
        }
    }    
}

void Image::enumerateSubdirectoryEntries(DirectoryEntry entry) {
    if (entry.getIsCountableSubdirectory()) {
        unsigned long startCluster = (unsigned long)entry.getStartCluster().toInt();
        for (unsigned long j = 0; j < this->maxNumDirectoryEntriesInSubdirectory; j++) {
            DirectoryEntry subdirectoryEntry = this->getDirectoryEntry(j, startCluster);
            this->processDirectoryEntry(subdirectoryEntry, false);
            this->enumerateSubdirectoryEntries(subdirectoryEntry);
        }
    }
}

void Image::traverseImage() {
    for (int i = 0; i < this->getMaxNumDirectoryEntries().toInt(); i++) {
        DirectoryEntry entry = this->getDirectoryEntry(i, true);
        this->processDirectoryEntry(entry, true);
        this->enumerateSubdirectoryEntries(entry);
    }
}

// traverseDirectory(){
//     if (i == 0) { // at root
//         for (int j = 0; j < this->getMaxNumDirectoryEntries().toInt(); j++) {
//             entry = this->getDirectoryEntry(j, true);
//             if (evaluateDirectoryEntry(entry, nameToFind, atLastSegmentOfPath, startCluster)) {
//                 return entry;
//             }
//         }
//     }
//     else { // in subdirectory
//         unsigned long maxNumDirectoryEntriesInSubdirectory = this->getNumBytesPerCluster() / ROOT_DIRECTORY_SIZE;
//         for (unsigned long j = 0; j < maxNumDirectoryEntriesInSubdirectory; j++) {
//             entry = this->getDirectoryEntry(j, startCluster);
//             if (evaluateDirectoryEntry(entry, nameToFind, atLastSegmentOfPath, startCluster)) {
//                 return entry;
//             }
//         }
//     }
// }
 
// void DirectoryEntry::traverseDirectory(DirectoryEntry entry) {
//     unsigned long entriesToTraverse = 0;
//     if (atRoot) {
//         entriesToTraverse = this->getMaxNumDirectoryEntries().toInt();
//         entries = 
//     }
//     else {
//         entriesToTraverse = this->getNumBytesPerCluster() / ROOT_DIRECTORY_SIZE;
//     }
//     for (int i = 0; i < entriesToTraverse; i++) {
//         entry = this->getDirectoryEntry(i, true);
//         vector<DirectoryEntry> entries = entry.getEntries();
//     }
// }

void Image::calculateNumEntries() {
    stringstream stream;

    stream << "Number of files in root directory: " << this->numFilesInRootDirectory  << endl;
    stream << "Number of files in the file system: " << this->numFiles << endl;
    stream << "Number of directories in the file system: " << this->numDirectories << endl;

    this->numEntries = stream.str();
}

void Image::calculateLargestFile() {
    stringstream stream;

    stream << "Largest file (" << this->largestFileSize << " bytes): " << this->largestFileName;

    this->largestFile = stream.str();
}

void Image::calculateAdditionalInfo() {
    stringstream stream;

    stream << this->numEntries;

    this->additionalInfo = stream.str();
}

Value Image::getNextClusterEntryIndex(unsigned long clusterEntryIndex) {
    return Value('s', fatsOffset + (2 * clusterEntryIndex), file, "");
}

string Image::getInfo() {
    return this->info;
}

void* Image::getCluster(unsigned long clusterEntryIndex) {
    return Value('b', getClusterOffset(clusterEntryIndex), this->file, "").getRawMemory();
}

unsigned long Image::getClusterOffset(unsigned long clusterEntryIndex) {
    return this->dataRegionOffset + ((clusterEntryIndex - 2) * this->numBytesPerCluster);
}

string Image::getNumEntries() {
    return this->numEntries;
}

string Image::getLargestFile() {
    return this->largestFile;
}

string Image::getAdditionalInfo() {
    return this->additionalInfo;
}