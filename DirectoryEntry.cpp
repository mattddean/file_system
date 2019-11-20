#include "DirectoryEntry.h"

DirectoryEntry::DirectoryEntry() {
    this->valid = false;
};

DirectoryEntry::DirectoryEntry(unsigned long index, void* file, unsigned long directoryEntriesOffset, unsigned long maxNumDirectoryEntriesInSubdirectory, unsigned long rootDirectoryOffset) {
    this->rootDirectoryOffset = rootDirectoryOffset;
    this->maxNumDirectoryEntriesInSubdirectory = maxNumDirectoryEntriesInSubdirectory;

    this->valid = true;
    this->offset = directoryEntriesOffset + (index * ROOT_DIRECTORY_SIZE);
    this->enclosingOffset = directoryEntriesOffset;
    this->file = file;

    calculateName();
    calculateAttributes();
    calculateCreateTime();
    calculateAccessDate();
    calculateExtendedAttributes();
    calculateModifyTime();
    calculateStartCluster();
    calculateSize();
    calculateInfo();
    calculateIsDotEntry();
    calculateIsCountableSubdirectory();
    calculateIsCountableFile();
    // calculateFullPath();
}

bool DirectoryEntry::isValid() {
    return valid;
}

void DirectoryEntry::calculateIsDotEntry() {
    this->isDotEntry = this->name.name == ".       " 
                    || this->name.name == "..      ";
}

void DirectoryEntry::calculateIsCountableSubdirectory() {
    vector<string> atrs = this->attributes;
    this->isCountableSubdirectory = find(atrs.begin(), atrs.end(), "subdir") != atrs.end() // "subdir" is in atrs
                                && !this->isDotEntry
                                && this->name.status == GOOD;
}

void DirectoryEntry::calculateIsCountableFile() {
    this->isCountableFile = !this->isCountableSubdirectory
                         && !this->isDotEntry
                         && this->name.status == GOOD;
}

void DirectoryEntry::calculateName() {
    Value extVal{'l', offset + 8, file, "", 3};
    Value nameVal{'l', offset + 0, file, "", 8};
    name.name = nameVal.toString();
    name.extension = extVal.toString();
    name.status = GOOD;
    if (strlen(name.name.c_str()) < 1) {
        name.status = EMPTY;
        return;
    }
    if (name.name.at(0) == 0) {
        name.status = EMPTY;
    }
    else if (name.name.at(0) == -27) {
        name.name.replace(0, 1, "?");
        name.status = PREVIOUSLY_ERASED;
    }
}

void DirectoryEntry::calculateAttributes() {
    Value value{'b', offset + 11, file, ""};
    int atrVal = (int)*((unsigned char *)value.getRawMemory());
    int masks[ATR_SIZE] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40};
    string atrs[ATR_SIZE] = {"RO", "Hidden", "Sys", "Vol. label", "subdir", "archive", "device"};
    for (int i = 0; i < ATR_SIZE; i++) {
        if ((atrVal & masks[i]) == masks[i]) {
            attributes.push_back(atrs[i]);
        }
    }
}

void DirectoryEntry::calculateCreateTime() {
    Value ctVal{'s', offset + 14, file, ""};
    int ct = ctVal.toInt();
    
    Value ctMilVal{'a', offset + 13, file, ""};
    int ctMil = ctMilVal.toInt();

    Value cdVal{'s', offset + 16, file, ""};
    int cd = cdVal.toInt();

    createTime = this->getDate(cd) + " " + this->getTime(ct, ctMil);
}

void DirectoryEntry::calculateAccessDate() {
    Value value{'s', offset + 18, file, ""};
    accessDate = this->getDate(value.toInt());
}

void DirectoryEntry::calculateExtendedAttributes() {
    extendedAttributes = Value('s', offset + 20, file, "");
}

void DirectoryEntry::calculateModifyTime() {
    Value modifyDateVal{'s', offset + 24, file, ""};
    int md = modifyDateVal.toInt();
    Value modifyTimeVal{'s', offset + 22, file, ""};
    int mt = modifyTimeVal.toInt();
    modifyTime = this->getDate(md) + " " + this->getTime(mt);
}

void DirectoryEntry::calculateStartCluster() {
    startCluster = Value('s', offset + 26, file, "");
}

void DirectoryEntry::calculateSize() {
    size = Value('i', offset + 28, file, "");
}

void DirectoryEntry::calculateInfo() {
    stringstream stream;

    FileName fileName = this->name;
    if (fileName.status == EMPTY) {
        stream << "Empty entry" << endl;
        this->info = stream.str();
        return;
    }
    else if (fileName.status == PREVIOUSLY_ERASED) {
        stream << "Previously erased entry" << endl;
    }
    else if (fileName.status == GOOD) {
        stream << "Name: " + fileName.name + "." << fileName.extension << endl;
    }

    stream << "File Attributes: ";
    vector<string> attributes = this->attributes;
    for(auto const& attribute: attributes) {
        stream << attribute << " ";
    }
    stream << endl;
    
    stream << "Create time: " << this->createTime << endl;
    stream << "Access date: " << this->accessDate << endl;
    stream << "Extended attributes: " << this->extendedAttributes << endl;
    stream << "Modify time: " << this->modifyTime << endl;
    stream << "Start cluster: " << this->startCluster << endl;
    stream << "Bytes: " << this->size << endl;

    this->info = stream.str();
}

// void DirectoryEntry::calculateFullPath() {
//     unsigned long clusterOffset = this->getStartCluster().toInt();
//     if (this->enclosingOffset != this->rootDirectoryOffset) {
//         for (unsigned long i = 0; i < this->maxNumDirectoryEntriesInSubdirectory; i++) {
//             DirectoryEntry entry = DirectoryEntry(i, this->file, clusterOffset, this->maxNumDirectoryEntriesInSubdirectory, this->rootDirectoryOffset, this->image);
//             if (entry.getName().name == "..      ") {
//                 this->fullPath = entry.getFullPath() + "/" + this->getName().toTrimmedString();
//                 return;
//                 clusterOffset = entry.getStartCluster().toInt();
//                 DirectoryEntry subEntry{i, this->file, clusterOffset, this->maxNumDirectoryEntriesInSubdirectory, this->rootDirectoryOffset, this->image};
                
//             }
//         }
//     }
//     else {
//         this->fullPath = "/" + this->getName().toTrimmedString();
//     }
// }

vector<string> DirectoryEntry::getAttributes() {
    return attributes;
}

string DirectoryEntry::getCreateTime() {
    return createTime;
}

string DirectoryEntry::getAccessDate() {
    return accessDate;
}

Value DirectoryEntry::getExtendedAttributes() {
    return extendedAttributes;
}

string DirectoryEntry::getModifyTime() {
    return modifyTime;
}

Value DirectoryEntry::getStartCluster() {
    return startCluster;
}

Value DirectoryEntry::getSize() {
    return size;
}

FileName DirectoryEntry::getName() {
    return name;
}

string DirectoryEntry::getTime(int thisTime, int milliseconds) {
    short secondsMask = ~(~0 << 5);
    short ctSecondsPart = (secondsMask & thisTime) * 2;
    short minutesMask = ~(~0 << 6);
    short ctMinutes = (minutesMask & (thisTime >> 5));
    short hoursMask = ~(~0 << 5);
    short ctHours = (hoursMask & (thisTime >> 11));
    // cout << ctHours << endl;
    // cout << ctMinutes << endl;
    // cout << ctSecondsPart << endl;
    
    int ctMil = milliseconds * 10;
    int ctSeconds = ctSecondsPart + ctMil;
    int ctMilRem = ctMil;
    if (ctMil >= 1000) {
        ctMilRem = ctMil % 1000;
        ctSeconds++;
    }

    string secString = "";
    if (ctSeconds < 10) {
        secString += "0";
    }
    secString += to_string(ctSeconds);

    string minString = "";
    if (ctMinutes < 10) {
        minString += "0";
    }
    minString += to_string(ctMinutes);

    string hourString = "";
    if (ctHours < 10) {
        hourString += "0";
    }
    hourString += to_string(ctHours);

    string milString = "";
    if (ctMilRem < 100) {
        milString += "00";
    }
    else if (ctMilRem < 10) {
        milString += "0";
    }
    milString += to_string(ctMilRem);

    ostringstream stream;
    stream << hourString << ":" << minString << ":" << secString << "." + milString;
    string retString = stream.str();
    return retString;
}

string DirectoryEntry::getDate(int date) {
    short dayMask = ~(~0 << 5);
    short cdDay = (dayMask & date);
    short monthMask = ~(~0 << 4);
    short cdMonth = (monthMask & (date >> 5));
    short yearMask = ~(~0 << 7);
    short cdYear = (yearMask & (date >> 9)) + 1980;
    string cdDayString = "";
    if (cdDay < 10 ) {
        cdDayString += "0";
    }
    cdDayString += to_string(cdDay);

    string cdMonthString = "";
    if (cdMonth < 10) {
        cdMonthString += "0";
    }
    cdMonthString += to_string(cdMonth);

    ostringstream stream;
    stream << cdYear << "/" << cdMonthString << "/" << cdDayString;
    string retString = stream.str();
    return retString;
}

string DirectoryEntry::getInfo() {
    return this->info;
}

bool DirectoryEntry::getIsCountableSubdirectory() {
    return isCountableSubdirectory;
}

bool DirectoryEntry::getIsCountableFile() {
    return isCountableFile;
}

string DirectoryEntry::getFullPath() {
    return fullPath;
}