#ifndef RESOURCEKEY_H
#define RESOURCEKEY_H

#include <EASTL/functional.h>

#include "../IO/FileStream.h"

namespace EA::ResourceMan {
    struct Key {
        uint64_t instance;
        uint32_t type;
        uint32_t group;

        bool operator==(const Key& other) const {
            return instance == other.instance &&
                   type == other.type &&
                   group == other.group;
        }

        static void Read(Key &instance, IO::IStream* stream);
        void Write(IO::IStream* stream) const;
    };
}

template <> struct eastl::hash<EA::ResourceMan::Key>
{
    uint64_t operator()(const EA::ResourceMan::Key& val) const { return val.instance ^ val.group; }
};

template <>
struct std::hash<EA::ResourceMan::Key> {
    std::size_t operator()(const EA::ResourceMan::Key& k) const noexcept {
        const std::size_t h1 = std::hash<uint64_t>{}(k.instance);
        const std::size_t h2 = std::hash<uint32_t>{}(k.type);
        const std::size_t h3 = std::hash<uint32_t>{}(k.group);

        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

#endif // RESOURCEKEY_H