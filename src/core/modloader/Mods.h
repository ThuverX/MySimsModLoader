//
// Created by exozg on 24/01/2025.
//

#ifndef MODS_H
#define MODS_H

#include <vector>

#include "Mod.h"

namespace Msml::Core {
    class Mods {
        explicit Mods(std::string rootPath);
        std::string mRootPath;
    public:
        static Mods& GetInstance();
        void Find();
        [[nodiscard]] std::vector<Mod*> GetModsSorted() const;
        std::vector<Mod*> mMods;
        void RunPostHooks() const;
        void RunPreHooks() const;
    };
}


#endif //MODS_H
