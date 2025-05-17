//
// Created by exozg on 24/01/2025.
//

#include "FNV.h"

namespace msml::hash::fnv {
    constexpr uint32_t FNV_PRIME_32 = 0x01000193;
    constexpr uint64_t FNV_PRIME_64 = 0x00000100000001B3;

    constexpr uint32_t FNV_OFFSET_32 = 0x811C9DC5;
    constexpr uint64_t FNV_OFFSET_64 = 0xCBF29CE484222325;

    // https://github.com/emd4600/Spore-ModAPI/blob/7890ffd4386af50703e528ee9d21bc07efda1636/Spore%20ModAPI/Spore/Hash.h#L24
    constexpr int const_tolower(const int c) {
        return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
    }

    uint64_t FromString64(const char *str) {
        uint64_t hash = FNV_OFFSET_64;

        for (int i = 0; str[i] != '\0'; i++) {
            hash *= FNV_PRIME_64;
            hash ^= const_tolower(str[i]);
        }
        return hash;
    }

    uint32_t FromString32(const char *str) {
        uint32_t hash = FNV_OFFSET_32;

        for (int i = 0; str[i] != '\0'; i++) {
            hash *= FNV_PRIME_32;
            hash ^= const_tolower(str[i]);
        }

        return hash;
    }
}
