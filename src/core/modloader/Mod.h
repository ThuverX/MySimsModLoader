//
// Created by exozg on 24/01/2025.
//

#ifndef MOD_H
#define MOD_H

#include <string>
#include <vector>
#include <filesystem>

namespace Msml::Core {
    class Mod {
    public:
        std::string mName;
        std::string mDescription;
        std::string mAuthor;
        std::string mPath;
        int mPriority = 0;

        void RunPostHooks() const;

        void RunPreHooks() const;

        std::vector<std::string> mPostHooks;
        std::vector<std::string> mPreHooks;
        std::vector<std::string> mAllHooks;

        static Mod *FromXML(const std::filesystem::path &path);
    };
}

#endif //MOD_H
