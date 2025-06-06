//
// Created by exozg on 24/01/2025.
//

#include "Hooks.h"
#include "MinHook.h"
#include "../system/Logger.h"

namespace msml::core::Hooks {
    void Install(void* target, void *detour, void** original) {
        if (target == nullptr) {
            MSML_LOG_ERROR("Failed to install hook");
        }

        if (MH_CreateHook(target, detour, original) != MH_OK) {
            MSML_LOG_ERROR("Failed to install hook");
        }
    }

    bool Initialize() {
        if (MH_Initialize() != MH_OK) {
            MSML_LOG_ERROR("Failed to initialize MinHook!");
            return false;
        }
        return true;
    }

    bool Enable() {
        if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
            MSML_LOG_ERROR("Failed to enable hooks!");
            return false;
        }
        return true;
    }

    void Uninitialize() {
        MH_Uninitialize();
    }
}