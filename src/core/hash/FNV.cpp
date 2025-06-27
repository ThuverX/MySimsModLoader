//
// Created by exozg on 24/01/2025.
//

#include "FNV.h"

namespace Msml::Hash::FNV {
    constexpr uint32_t kFnvPrime32 = 0x01000193;
    constexpr uint64_t kFnvPrime64 = 0x00000100000001B3;

    constexpr uint32_t kFnvOffset32 = 0x811C9DC5;
    constexpr uint64_t kFnvOffset64 = 0xCBF29CE484222325;


    uint64_t FromString64(const std::string_view& kStr) {
        uint64_t hash = kFnvOffset64;
        for (char c : kStr) {
            hash *= kFnvPrime64;
            hash ^= static_cast<uint8_t>(tolower(c));
        }
        return hash;
    }

    uint32_t FromString32(const std::string_view &kStr) {
        uint32_t hash = kFnvOffset32;
        for (char c : kStr) {
            hash *= kFnvPrime32;
            hash ^= static_cast<uint8_t>(tolower(c));
        }
        return hash;
    }
}
