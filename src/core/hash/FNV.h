//
// Created by exozg on 24/01/2025.
//

#ifndef FNV_H
#define FNV_H

#include <cstdint>
#include <string>

namespace Msml::Hash::FNV {
    uint64_t FromString64(const std::string_view &kStr);
    uint32_t FromString32(const std::string_view &kStr);
}


#endif //FNV_H
