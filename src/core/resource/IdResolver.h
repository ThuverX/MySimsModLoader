//
// Created by exozg on 12/03/2025.
//

#ifndef IDRESOLVER_H
#define IDRESOLVER_H

#include <string>
#include <unordered_map>

#include "../../EA/ResourceMan/ResourceKey.h"

namespace Msml::Core::Resource {
    class IdResolver {
        std::unordered_map<uint32_t, std::string> mHashes32;
        std::unordered_map<uint64_t, std::string> mHashes64;

    public:
        static IdResolver &GetInstance();

        std::string GetHash64(uint64_t kHash);

        std::string GetHash32(uint32_t kHash);

        void Load(const std::string &kPath);

        void Add(const std::string &kStr);

        static std::string ToHumanReadable(const EA::ResourceMan::Key &key);

        static std::string ToFilename(const EA::ResourceMan::Key &key);
    };
}


#endif //IDRESOLVER_H
