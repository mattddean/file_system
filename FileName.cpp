#include "FileName.h"

string trim(string str) {
    int pos = str.find(' ');
    if (pos == -1) {
        return str;
    }
    return str.substr(0,pos);
}

bool operator==(const FileName &f1, const FileName &f2) {
    string f1NameTrimmed = trim(f1.name);
    string f2NameTrimmed = trim(f2.name);
    string f1ExtensionTrimmed = trim(f1.extension);
    string f2ExtensionTrimmed = trim(f2.extension);

    return f1NameTrimmed == f2NameTrimmed && f1ExtensionTrimmed == f2ExtensionTrimmed;
}

string FileName::toString() {
    stringstream stream;
    if (this->status == EMPTY) {
        stream << "Empty entry" << endl;
    }
    else if (this->status == PREVIOUSLY_ERASED) {
        stream << "Previously erased entry" << endl;
    }
    else if (this->status == GOOD) {
        stream << this->name + "." << this->extension << endl;
    }
    return stream.str();
}

string FileName::toTrimmedString() {
    stringstream stream;
    if (this->status == EMPTY) {
        stream << "Empty entry" << endl;
    }
    else if (this->status == PREVIOUSLY_ERASED) {
        stream << "Previously erased entry" << endl;
    }
    else if (this->status == GOOD) {
        stream << trim(this->name);
        if (trim(this->extension) != "") { // extension was just whitespace, so there wasn't an extension
            stream << "." << trim(this->extension);
        }
    }
    return stream.str();
}