//
// Created by exozg on 12/03/2025.
//

#ifndef IDRESOLVER_H
#define IDRESOLVER_H

#include <string>
#include <unordered_map>

#include "../../EA/ResourceMan/ResourceKey.h"

namespace msml::core::resource {
    class IdResolver {
    public:
        static IdResolver &GetInstance();

        std::string GetHash64(uint64_t hash);

        std::string GetHash32(uint32_t hash);

        void Load(const std::string &path);

        void Add(const std::string &str);

        static std::string ToHumanReadable(const EA::ResourceMan::Key &key);

        static std::string ToFilename(const EA::ResourceMan::Key &key);

    private:
        std::unordered_map<uint32_t, std::string> hashes32;
        std::unordered_map<uint64_t, std::string> hashes64;
    };
}


#endif //IDRESOLVER_H
