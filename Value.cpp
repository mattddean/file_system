#include "Value.h"

Value::Value() {}

Value::Value(char sizeChar, unsigned long location, void *file, string prepend, int length) {
    this->file = file;
    this->prepend = prepend;

    determineType(sizeChar, length);
    
    rawMemory = retrieveMemoryFromFile(location);
}

Value::Value(string instruction, void *file, string prepend, int length) {
    this->file = file;
    this->prepend = prepend;

    string delimeter = " ";
    int delimeterIndex = instruction.find(delimeter);
    int location = stoi(instruction.substr(delimeterIndex + 1));

    char sizeChar = instruction.substr(0, delimeterIndex)[0];

    determineType(sizeChar, length);

    rawMemory = retrieveMemoryFromFile(location);
}

void Value::determineType(char sizeChar, int length) {
    switch(sizeChar) {
        case 'c':
            type = CHAR;
            printType = NORMAL;
            size = 1;
            break;
        case 'b':
            type = UNSIGNED_BYTE;
            printType = HEXADECIMAL;
            size = 1;
            break;
        case 'a':
            type = UNSIGNED_BYTE_DECIMAL;
            printType = DECIMAL;
            size = 1;
            break;
        case 's':
            type = TWO_BYTE;
            printType = DECIMAL;
            size = 2;
            break;
        case 'w':
            type = UNSIGNED_TWO_BYTE;
            printType = HEXADECIMAL;
            size = 2;
            break;
        case 'i':
            type = FOUR_BYTE;
            printType = DECIMAL;
            size = 4;
            break;
        case 'u':
            type = UNSIGNED_FOUR_BYTE;
            printType = HEXADECIMAL;
            size = 4;
            break;
        case 'l':
            type = DYNAMIC_LENGTH;
            printType = NORMAL;
            size = length;
            break;
        case 'z':
        default:
            type = STRING;
            printType = NORMAL;
            size = 8;
            break;
    }
}

std::ostream& operator<<(std::ostream& os, const Value& v) {
    char *temp = (char*)malloc(v.size + 1);
    temp[v.size] = '\0';
    os << v.prepend;
    switch (v.type) {
        case CHAR:
            os << dec << *((char *)v.rawMemory);
            break;
        case UNSIGNED_BYTE:
            os << hex << (int)*((unsigned char *)v.rawMemory);
            break;
        case UNSIGNED_BYTE_DECIMAL:
            os << dec << (int)*((unsigned char *)v.rawMemory);
            break;
        case TWO_BYTE:
            os << dec << *((unsigned short *)v.rawMemory);
            break;
        case UNSIGNED_TWO_BYTE:
            os << hex << *((unsigned short *)v.rawMemory);
            break;
        case FOUR_BYTE:
            os << dec << *((int *)v.rawMemory);
            break;
        case UNSIGNED_FOUR_BYTE:
            os << hex << *((unsigned int *)v.rawMemory);
            break;
        case DYNAMIC_LENGTH:
            memcpy(temp, v.rawMemory, v.size);
            os << dec << temp;
            break;
        case STRING:
        default:
            memcpy(temp, v.rawMemory, 8);
            os << dec << temp;
            break;
    }
    free(temp);
    return os;
}

string Value::toString() {
    ostringstream stream;
    stream << *this;
    string outString = stream.str();
    return outString;
}

int Value::toInt() {
    int intNum;
    // cout << "this:" << this->toString() << endl;
    
    try {
        intNum = stoi(this->toString());
    }
    catch(std::invalid_argument& e){
        cout << e.what() << endl;
        return 0;
    }
    catch(std::out_of_range& e){
        cout << e.what() << endl;
        return 0;
    }
    catch(...) {
        cout << "stoi error" << endl;
        return 0;
    }
    return intNum;
}

void *Value::getRawMemory() {
    return rawMemory;
}

void *Value::retrieveMemoryFromFile(int offset) {
    return (void *)((char *)file + offset);
}

void *Value::getFile() {
    return file;
}

ValueType Value::getType() {
    return type;
}

