//
// Created by exozg on 24/01/2025.
//

#ifndef FNV_H
#define FNV_H
#include <cstdint>
#include <string>


class FNV {
public:
    static uint64_t FromString64(const char* str);
    static uint32_t FromString32(const char* str);
};


#endif //FNV_H
