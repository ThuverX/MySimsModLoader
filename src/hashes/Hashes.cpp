//
// Created by exozg on 12/03/2025.
//

#include "Hashes.h"

#include <fstream>

#include "../assets/Asset.h"
#include "../util/Logger.h"

Hashes & Hashes::GetInstance() {
    static Hashes instance;
    return instance;
}

std::string Hashes::GetHash64(const uint64_t hash) {
    if (hashes64.contains(hash)) {
        return hashes64.at(hash);
    }

    return std::format("{:016x}", hash);
}

std::string Hashes::GetHash32(const uint32_t hash) {
    if (hashes32.contains(hash)) {
        return hashes32.at(hash);
    }

    return std::format("{:08x}", hash);
}

#define READ_BIN(var) infile.read((char*)&var, sizeof(var));
#define WRITE_BIN(var) outfile.write((char*)&var, sizeof(var))

#define READ_CSTRING(str) { \
    std::vector<char> buffer; \
    char ch; \
    while (infile.read(&ch, 1) && ch != '\0') { \
    buffer.push_back(ch); \
    } \
    buffer.push_back('\0'); /* Ensure null termination */ \
    str = new char[buffer.size()]; \
    memcpy(str, buffer.data(), buffer.size()); \
}

#define WRITE_CSTRING(str) { \
    outfile.write(str, strlen(str) + 1); /* Write including null terminator */ \
}

void Hashes::Load(const std::string& path) {
    std::ifstream infile(path, std::ios::in | std::ios::binary);
    if (!infile.good()) {
        MSML_LOG_ERROR("Failed to open %s", path.c_str());
        return;
    }
    hashes32.clear();
    hashes64.clear();
    uint64_t count;
    READ_BIN(count);

    for (int i = 0; i < count; i++) {
        char* text;
        READ_CSTRING(text);
        uint32_t fnv32;
        READ_BIN(fnv32);
        uint64_t fnv64;
        READ_BIN(fnv64);

        hashes32.insert(std::pair<uint32_t, std::string>(fnv32, text));
        hashes64.insert(std::pair<uint64_t, std::string>(fnv64, text));
    }

    MSML_LOG_INFO("Loaded %d Hashes from %s", hashes32.size(), path.c_str());
}

std::string Hashes::ToHumanReadable(const EA::ResourceMan::Key &key) {
    const std::string instance = GetInstance().GetHash64(key.instance);
    const std::string type = Asset::GetTypeName(key.type);
    const std::string group = GetInstance().GetHash32(key.group);

    if (key.group == 0) return instance + "." + type;

    return group + "/" + instance + "." + type;
}
