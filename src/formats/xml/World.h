#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <vector>

#include "GameObjectDef.h"
#include "GameObjectListHolder.h"

struct LevelRef {
    std::string Name;
    std::string BoundingBoxInfoString;
    std::string FillLightColorString;
    std::string RimLightColorString;
    float RimLightAngle;
};

struct BuildingDef : public GameObjectDef {
    std::string SaveName;
    int LotID;
};

struct SlotGameObjectDef : public GameObjectDef {
    int SlotID;
};

struct PortalDef : public GameObjectDef {
    bool TeleportPlayerOnCollision;
    bool TeleportNPCOnCollision;
    bool Locked;
};

struct SlotsDef {
    std::vector<SlotGameObjectDef> Slots;
};

struct ConstructedGameObjectDef : public GameObjectDef {
    std::string SaveName;
};

struct World : public GameObjectListHolder {
    LevelRef Level;
    std::vector<BuildingDef> Buildings;
    std::vector<ConstructedGameObjectDef> ConstructedGameObjects;
    std::vector<PortalDef> Portals;
    SlotsDef Slots; // TODO: This field is optional... std::optional vs SlotsDef*?

    static bool Read(World &instance, void *data, size_t size);
};

#endif // WORLD_H
