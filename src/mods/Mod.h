//
// Created by exozg on 24/01/2025.
//

#ifndef MOD_H
#define MOD_H

#include <string>
#include <vector>


struct LuaHook {
    std::string nativeFile;
    std::string replacementFile;
};

class Mod {
public:
    std::string name;
    std::string description;
    std::string author;
    std::string path;

    static Mod* fromXML(std::string path);
    void LoadAssets();
    void LoadHooks();

    std::vector<LuaHook> hooks;
    std::vector<std::string> assets;

};



#endif //MOD_H
