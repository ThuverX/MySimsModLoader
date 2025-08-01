//
// Created by exozg on 24/01/2025.
//

#include "Hooks.h"
#include "MinHook.h"
#include "../modloader/ModLoader.h"
#include "../signatures/Signatures.h"
#include "../system/Logger.h"

namespace Msml::Core::Hooks {
    void Install(void* pTarget, void * pDetour, void** ppOriginal) {
        if (pTarget == nullptr) {
            MSML_LOG_ERROR("Failed to install hook");
            Signatures::CleanSignatureCache();
            ModLoader::MessageAndExit("Failed to install hook, deleting signature cache");
        }

        if (MH_CreateHook(pTarget, pDetour, ppOriginal) != MH_OK) {
            MSML_LOG_ERROR("Failed to install hook");
            Signatures::CleanSignatureCache();
            ModLoader::MessageAndExit("Failed to install hook, deleting signature cache");
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