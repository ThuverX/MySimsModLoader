#ifndef OBJECTDEF_H
#define OBJECTDEF_H

#include <string>
#include <vector>
#include <unordered_map>

struct ObjectDef {

    std::string Model;
    std::string Rig;
    std::string Script;
    std::string MaxAttachments;
    std::string CollisionInfo;
    bool NoFootPrint;

    static void Read(ObjectDef &instance, void *data, size_t size);
    
};

#endif // OBJECTDEF_H