//
// Created by exozg on 24/01/2025.
//

#ifndef HOOKS_H
#define HOOKS_H

namespace Msml::Core::Hooks {
    void Install(void * pTarget, void * pDetour, void ** ppOriginal);
    bool Initialize();
    bool Enable();
    void Uninitialize();
}



#endif //HOOKS_H
