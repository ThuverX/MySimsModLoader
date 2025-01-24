//
// Created by exozg on 24/01/2025.
//

#ifndef SIGNATURES_H
#define SIGNATURES_H

#include <sigmatch/sigmatch.hpp>
#include "../hooks/Hooks.h"
#include "../util/Logger.h"

using namespace sigmatch_literals;

class Signatures {
public:
    static Signatures& GetInstance();
    bool Search(const sigmatch::signature &sig, void*& address, uint32_t offset);
private:
    Signatures();
    sigmatch::search_context context;
};
#define CREATE_NORMAL_CALLABLE_SIGNATURE(name, retn, signature, offset, ...) \
    typedef retn(*name ## ptr)(__VA_ARGS__); \
    static name ## ptr name = nullptr; \
    static SigSearch<name ## ptr> name ## Hook(#name, signature ## _sig, (void*&)name, offset);

#define CREATE_MEMBER_CALLABLE_SIGNATURE(name, retn, signature, offset, ...) \
    typedef retn(__thiscall *name ## ptr)(void*, __VA_ARGS__); \
    static name ## ptr name = nullptr; \
    static SigSearch<name ## ptr> name ## Hook(#name, signature ## _sig, (void*&)name, offset);

// TODO: Set the og (name_ptr) too, not just address


template <typename OriginalFuncPtr>
class SigSearch {
public:
    const char* name;
    sigmatch::signature signature;
    void*& address;
    OriginalFuncPtr Original = nullptr;
    uint32_t offset;

    SigSearch(const char* name, const sigmatch::signature &sig, void*& address, const int offset): name(name), signature(sig), address(address), offset(offset) {
        Search();
    }

    void Search() const {
        if (!Signatures::GetInstance().Search(signature, address, offset)) {
            MSML_LOG_INFO("Failed to find an address for %s", name);
        }
    }
    void Install(void *detour) {
        if (address == nullptr)
            Search();
        Hooks::GetInstance().Install(address, detour, reinterpret_cast<void**>(&Original));
    }

};

namespace EA::ResourceMan {
    struct Key {
        uint64_t instance;
        uint32_t type;
        uint32_t group;
    };
    struct DatabasePackedFile {
    };
    namespace PFRecordRead {
        CREATE_MEMBER_CALLABLE_SIGNATURE(PFRecordRead_1, void*, "56 8B F1 C7 06 04 E7 E4 00 33 C0 8D 4E 04 87 01 8B 44 24 10 C7", 0, uint32_t, uint32_t, Key*, DatabasePackedFile*);
        CREATE_MEMBER_CALLABLE_SIGNATURE(PFRecordRead_2, void*, "56 8B F1 57 C7 06 04 E7 E4 00 33 C0 8D 4E 04 87 01 8B 44 24 18 8B", 0, void*, uint32_t, bool, Key*, DatabasePackedFile*);
    }
}


#endif //SIGNATURES_H