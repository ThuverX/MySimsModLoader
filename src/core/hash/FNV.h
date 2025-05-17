//
// Created by exozg on 24/01/2025.
//

#ifndef FNV_H
#define FNV_H
#include <cstdint>
#include <string>

namespace msml::hash::fnv {
    uint64_t FromString64(const char* str);
    uint32_t FromString32(const char* str);
}


#endif //FNV_H
