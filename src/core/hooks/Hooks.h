﻿//
// Created by exozg on 24/01/2025.
//

#ifndef HOOKS_H
#define HOOKS_H

namespace msml::core::Hooks {
    void Install(void *target, void *detour, void **original);
    bool Initialize();
    bool Enable();
    void Uninitialize();
}



#endif //HOOKS_H
