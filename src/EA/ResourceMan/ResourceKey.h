#ifndef RESOURCEKEY_H
#define RESOURCEKEY_H

#include <EASTL/functional.h>

#include "../IO/FileStream.h"

namespace EA::ResourceMan {
    struct Key {
        uint64_t mInstance;
        uint32_t mType;
        uint32_t mGroup;

        bool operator==(const Key &other) const {
            return mInstance == other.mInstance &&
                   mType == other.mType &&
                   mGroup == other.mGroup;
        }

        static void Read(Key &instance, IO::IStream *stream);

        void Write(IO::IStream *stream) const;
    };
}

template<>
struct eastl::hash<EA::ResourceMan::Key> {
    uint64_t operator()(const EA::ResourceMan::Key &val) const { return val.mInstance ^ val.mGroup; }
};

template<>
struct std::hash<EA::ResourceMan::Key> {
    std::size_t operator()(const EA::ResourceMan::Key &kOther) const noexcept {
        const std::size_t kH1 = std::hash<uint64_t>{}(kOther.mInstance);
        const std::size_t kH2 = std::hash<uint32_t>{}(kOther.mType);
        const std::size_t kH3 = std::hash<uint32_t>{}(kOther.mGroup);

        return kH1 ^ (kH2 << 1) ^ (kH3 << 2);
    }
};

#endif // RESOURCEKEY_H
