//
// Created by exozg on 24/01/2025.
//

#ifndef MOD_H
#define MOD_H

#include <pugixml.hpp>
#include <string>
#include <vector>

#include "../tweakers/Tweaker.h"

class Mod {
public:
    std::string name;
    std::string description;
    std::string author;
    std::string path;
    std::string assetsPath;
    int priority = 0;

    std::vector<Tweaker*> tweakers;

    static Mod* fromXML(const std::string &path);
    void RunPostHooks() const;
    void RunPreHooks() const;

    void ApplyTweaks(const EA::ResourceMan::Key &key) const;

    std::vector<std::string> postHooks;
    std::vector<std::string> preHooks;
private:
    static void TweakersFromXML(pugi::xml_node modNode, Mod *mod);
};



#endif //MOD_H
