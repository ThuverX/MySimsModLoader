//
// Created by exozg on 24/01/2025.
//

#ifndef MOD_H
#define MOD_H

#include <string>
#include <vector>


struct ScriptHook {
    std::string nativeFile;
    std::string hookFile;
};

class Mod {
public:
    std::string name;
    std::string description;
    std::string author;
    std::string path;
    std::string assetsPath;
    int priority = 0;

    static Mod* fromXML(std::string path);
    void RunPostHooks(const std::string& modulePath) const;
    void RunPreHooks(const std::string& modulePath) const;

    std::vector<ScriptHook> postHooks;
    std::vector<ScriptHook> preHooks;
};



#endif //MOD_H
