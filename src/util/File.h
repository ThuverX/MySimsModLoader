//
// Created by exozg on 24/01/2025.
//

#ifndef FILE_H
#define FILE_H

#include <vector>

class File {
public:
    static std::vector<uint8_t> ReadAsBytes(const char* filePath);
    static const char* ReadAsString(const char* filePath);
};



#endif //FILE_H
