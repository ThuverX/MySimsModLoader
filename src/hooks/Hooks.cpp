//
// Created by exozg on 24/01/2025.
//

#include "Hooks.h"
#include "MinHook.h"
#include "../signatures/Signatures.h"
#include "../util/Logger.h"

Hooks & Hooks::GetInstance() {
    static Hooks instance;
    return instance;
}

void Hooks::Install(void* target, void *detour, void** original) {
    if (target == nullptr) {
        MSML_LOG_ERROR("Failed to install hook");
    }

    if (MH_CreateHook(target, detour, original) != MH_OK) {
        MSML_LOG_ERROR("Failed to install hook");
    }
}

bool Hooks::Initialize() {
    if (MH_Initialize() != MH_OK) {
        MSML_LOG_ERROR("Failed to initialize MinHook!");
        return false;
    }
    return true;
}

bool Hooks::Enable() {
    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
        MSML_LOG_ERROR("Failed to enable hook!");
        return false;
    }
    return true;
}

void Hooks::Uninitialize() {
    MH_Uninitialize();
}
