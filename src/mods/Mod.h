//
// Created by exozg on 24/01/2025.
//

#ifndef MOD_H
#define MOD_H

#include <string>
#include <vector>

class Mod {
public:
    std::string name;
    std::string description;
    std::string author;
    std::string path;
    std::string assetsPath;
    int priority = 0;

    static Mod* fromXML(std::string path);
    void RunPostHooks() const;
    void RunPreHooks() const;

    std::vector<std::string> postHooks;
    std::vector<std::string> preHooks;
};



#endif //MOD_H
