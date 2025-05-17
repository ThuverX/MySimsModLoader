//
// Created by exozg on 24/01/2025.
//

#ifndef MOD_H
#define MOD_H

#include <string>
#include <vector>
#include <filesystem>

namespace msml::core::modloader {
    class Mod {
    public:
        std::string name;
        std::string description;
        std::string author;
        std::string path;
        int priority = 0;

        void RunPostHooks() const;

        void RunPreHooks() const;

        std::vector<std::string> postHooks;
        std::vector<std::string> preHooks;

        static Mod *fromXML(const std::filesystem::path &path);
    };
}

#endif //MOD_H
