//
// Created by exozg on 08/08/2025.
//

#ifndef SIGNATURES_MACROS_H
#define SIGNATURES_MACROS_H

#include <sigmatch/sigmatch.hpp>

#include "Signatures.h"
#include "../hooks/Hooks.h"
#include "../system/Logger.h"

template<typename OriginalFuncPtr>
class SigSearch : public Msml::Core::SigSearchBase {
public:
    std::string mName;
    sigmatch::signature mSignature;
    void *&mAddress;
    OriginalFuncPtr Original = nullptr; // this is a function, not a member
    SigSearch(const std::string &name, const std::string_view &sig, void *&address): mName(name), mSignature(sigmatch::impl::parse_sig_str_runtime(sig).value()), mAddress(address) {

        Msml::Core::Signatures::GetInstance().Append(name, this);
    }

    [[nodiscard]] bool Search() const override {
        if (!Msml::Core::Signatures::GetInstance().Search(mSignature, mAddress, 0, false)) {
            MSML_LOG_ERROR("Failed to find an address for %s", mName.c_str());
            return false;
        }
#ifdef BUILD_DEBUG
        MSML_LOG_INFO("Found %s %p", mName.c_str(), mAddress);
#endif
        return true;
    }

    [[nodiscard]] std::string GetName() const override {
        return mName;
    }

    [[nodiscard]] void *GetAddress() const override {
        return mAddress;
    }

    void ApplyAddress(void *addr) const override {
        mAddress = addr;
    }

    void Install(void *detour) {
        Msml::Core::Hooks::Install(mAddress, detour, reinterpret_cast<void **>(&Original));
    }
};

class Constant : public Msml::Core::SigSearchBase
{
public:
    std::string mName;
    void *&mAddress;
    uint64_t mLocation;
    Constant(const std::string &name, void *&address, uint64_t location) : mName(name), mAddress(address), mLocation(location)
    {
        Msml::Core::Signatures::GetInstance().Append(name, this);
    }

    [[nodiscard]] bool Search() const override
    {
        Msml::Core::Signatures::GetInstance().GetAddress(mAddress, mLocation);
#ifdef BUILD_DEBUG
        MSML_LOG_INFO("Set constant %s at %p", mName.c_str(), mAddress);
#endif
        return true;
    };

    [[nodiscard]] std::string GetName() const override
    {
        return mName;
    };

    [[nodiscard]] void* GetAddress() const override
    {
        return mAddress;
    };

    void ApplyAddress(void* pAddr) const override
    {
        mAddress = pAddr;
    };
};

#ifdef SIG_DEFINE

#define VIRTUAL(return_type, cls, name, ...) \
    typedef return_type (__thiscall *name##_t)(cls * this_ptr, __VA_ARGS__); \
    name##_t name = nullptr; \
    SigSearch<name##_t> name ## Hook(#cls "::" #name, SIG_##cls##_##name, (void*&)name);

#define GLOBAL(return_type, name, ...) \
    typedef return_type (*name##_t)(__VA_ARGS__); \
    name##_t name = nullptr; \
    SigSearch<name##_t> name ## Hook(#name, SIG_##name, (void*&)name);

#define STATIC(return_type, cls, name, ...) \
    typedef return_type (*name##_t)(__VA_ARGS__); \
    name##_t name = nullptr; \
    SigSearch<name##_t> name ## Hook(#cls "::" #name, SIG_##cls##_##name, (void*&)name);

#define CONSTANT(type, name) \
    type** name = nullptr; \
    Constant name ## Constant (#name, (void*&)name, CONST_##name);
#else

#define VIRTUAL(return_type, cls, name, ...) \
    typedef return_type (__thiscall *name##_t)(cls *this_ptr, __VA_ARGS__); \
    extern name##_t name; \
    extern SigSearch<name##_t> name ## Hook;

#define GLOBAL(return_type, name, ...) \
    typedef return_type (*name##_t)(__VA_ARGS__); \
    extern name##_t name; \
    extern SigSearch<name##_t> name ## Hook;

#define STATIC(return_type, cls, name, ...) \
    typedef return_type (*name##_t)(__VA_ARGS__); \
    extern name##_t name; \
    extern SigSearch<name##_t> name ## Hook;

#define CONSTANT(type, name) \
    extern type** name; \
    extern Constant name ## Constant;

#endif

#define SINGLETON_INSTANCE(type, size) \
    STATIC(type*, type, Constructor, void*); \
    CONSTANT(type, g ## type);\
    inline type* Instance() \
    { \
        if (g ## type == nullptr) \
            return nullptr; \
        if (*g ## type == nullptr) \
        { \
            if (const auto data = malloc(size); data != nullptr) \
            { \
                *g ## type = Constructor(data); \
            } \
        } \
        return *g ## type; \
    }

#endif //SIGNATURES_MACROS_H
