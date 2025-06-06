#ifndef GAMEOBJECTDEF_H
#define GAMEOBJECTDEF_H

#include <string>

struct GameObjectDef {
    std::string Script;
    std::string Translation;
    std::string Rotation;
    std::string ObjectDef;
    std::vector<GameObjectDef> Children;
    std::string Guid;
    // std::string LuaTable; // TODO: Is an XMLElement in Core
    std::string Name;
};

#endif // GAMEOBJECTDEF_H
