//
// Created by exozg on 12/03/2025.
//

#ifndef HASHES_H
#define HASHES_H

#include <string>
#include <unordered_map>

#include "../signatures/sigdef.h"


class Hashes {
public:
    static Hashes& GetInstance();
    std::string GetHash64(uint64_t hash);
    std::string GetHash32(uint32_t hash);
    void Load(const std::string& path);
    static std::string ToHumanReadable(const EA::ResourceMan::Key &key);
private:
    std::unordered_map<uint32_t, std::string> hashes32;
    std::unordered_map<uint64_t, std::string> hashes64;
};



#endif //HASHES_H
