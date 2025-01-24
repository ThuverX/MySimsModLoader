//
// Created by exozg on 24/01/2025.
//

#include "File.h"

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>


std::vector<uint8_t> File::ReadAsBytes(const char *filePath)  {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error(std::string("Failed to open file: ") + filePath);
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(fileSize);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        throw std::runtime_error(std::string("Failed to read file: ") + filePath);
    }

    return buffer;
}

const char * File::ReadAsString(const char *filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error(std::string("Failed to open file: ") + filePath);
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    char* content = new char[fileSize + 1];
    if (!file.read(content, fileSize)) {
        delete[] content;
        throw std::runtime_error(std::string("Failed to read file: ") + filePath);
    }

    content[fileSize] = '\0';

    return content;
}
