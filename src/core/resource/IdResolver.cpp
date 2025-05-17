//
// Created by exozg on 12/03/2025.
//

#include "./IdResolver.h"

#include <fstream>

#include "../assets/Asset.h"
#include "../hash/FNV.h"
#include "../system/Logger.h"

namespace msml::core::resource {
    IdResolver &IdResolver::GetInstance() {
        static IdResolver instance;
        return instance;
    }

    std::string IdResolver::GetHash64(const uint64_t hash) {
        if (hashes64.contains(hash)) {
            return hashes64.at(hash);
        }

        return std::format("{:016x}", hash);
    }

    std::string IdResolver::GetHash32(const uint32_t hash) {
        if (hashes32.contains(hash)) {
            return hashes32.at(hash);
        }

        return std::format("{:08x}", hash);
    }

    // TODO: i hate this, we should have a class for this. OH WAIT WE DO
    // replace
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

    void IdResolver::Load(const std::string &path) {
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
            char *text;
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

    void IdResolver::Add(const std::string &str) {
        uint64_t fnv64 = hash::fnv::FromString64(str.c_str());
        uint32_t fnv32 = hash::fnv::FromString32(str.c_str());

        hashes32.insert(std::pair(fnv32, str));
        hashes64.insert(std::pair(fnv64, str));
    }

    std::string IdResolver::ToHumanReadable(const EA::ResourceMan::Key &key) {
        const std::string instance = GetInstance().GetHash64(key.instance);
        const std::string type = assets::Asset::GetTypeName(key.type);
        const std::string group = GetInstance().GetHash32(key.group);

        if (key.group == 0) return instance + "." + type;

        return group + "!" + instance + "." + type;
    }

    std::string IdResolver::ToFilename(const EA::ResourceMan::Key &key) {
        const std::string instance = std::format("0x{:016x}", key.instance);
        const std::string type = assets::Asset::GetTypeName(key.type);
        const std::string group = std::format("0x{:08x}", key.group);

        return group + "!" + instance + "." + type;
    }
}
