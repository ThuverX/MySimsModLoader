//
// Created by exozg on 24/01/2025.
//

#include "FNV.h"

constexpr uint32_t FNV_PRIME_32 = 0x01000193;
constexpr uint64_t FNV_PRIME_64 = 0x00000100000001B3;

constexpr uint32_t FNV_OFFSET_32 = 0x811C9DC5;
constexpr uint64_t FNV_OFFSET_64 = 0xCBF29CE484222325;

uint64_t FNV::FromString64(const char *str) {
    uint64_t hash = FNV_OFFSET_64;

    for (int i = 0; str[i] != '\0'; i++) {
        hash *= FNV_PRIME_64;
        hash ^= str[i];
    }
    return hash;
}

uint32_t FNV::FromString32(const char *str) {
    uint32_t hash = FNV_OFFSET_32;

    for (int i = 0; str[i] != '\0'; i++) {
        hash *= FNV_PRIME_32;
        hash ^= str[i];
    }

    return hash;
}
