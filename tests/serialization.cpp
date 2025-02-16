#include <iostream>
#include <fstream>

#include "formats/CharacterDef.h"

int main() {
    // Just testing for now...
    // TODO: Start using Gtest for unit tests like this

    std::ifstream file("C:\\Users\\User\\Downloads\\CharacterDef.xml");
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }

    void *buffer = nullptr;
    size_t size = 0;

    file.seekg(0, std::ios::end);
    size = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer = new char[size];
    file.read(static_cast<char *>(buffer), size);

    // Read the data into a CharacterDef instance
    CharacterDef characterDef;
    CharacterDef::Read(characterDef, buffer, size);

    return 0;
}
