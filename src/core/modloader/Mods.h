//
// Created by exozg on 24/01/2025.
//

#ifndef MODS_H
#define MODS_H

#include <vector>

#include "Mod.h"

namespace msml::core::modloader {
    class Mods {
    public:
        static Mods& GetInstance();
        void Find();
        [[nodiscard]] std::vector<Mod*> GetModsSorted() const;
        std::vector<Mod*> mods;
        void RunPostHooks() const;
        void RunPreHooks() const;
    private:
        explicit Mods(std::string rootPath);
        std::string rootPath;
    };
}


#endif //MODS_H
