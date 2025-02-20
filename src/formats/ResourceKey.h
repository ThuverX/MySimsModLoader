#ifndef RESOURCEKEY_H
#define RESOURCEKEY_H

#include <cstdint>

struct ResourceKey {
    uint64_t instance;
    uint32_t type;
    uint32_t group;
};

#endif // RESOURCEKEY_H