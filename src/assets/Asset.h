//
// Created by exozg on 24/01/2025.
//

#ifndef ASSET_H
#define ASSET_H

#include <unordered_map>

#include "../signatures/sigdef.h"

enum DDFFileType {
    MODEL = 0x01661233,
    REVOMODEL = 0xF9E50586,
    WINDOWSMODEL = 0xB359C791, // 23653912
    DOWNLOAD = 0xD86F5E67,
    RIG = 0x8EAF13DE,
    CLIP = 0x6B20C4F3,
    KEYNAMEMAP = 0x0166038C,
    GEOMETRY = 0x015A1849,
    MATERIAL = 0x01D0E75D,
    MATERIALSET = 0x02019972,
    OLDSPEEDTREE = 0x00B552EA,
    SPEEDTREE = 0x021D7E8C,
    DDS = 0x00B2D882,
    COMPOSITETEXTURE = 0x8E342417,
    SIMOUTFIT = 0x025ED6F4,
    LEVEL_XML = 0x585EE310,
    LEVEL_BIN = 0x58969018,
    PHYSICS = 0xD5988020,
    LIGHTSET_XML = 0x50182640,
    LIGHTSET_BIN = 0x50002128,
    XML = 0xDC37E964,
    FOOTPRINTSET = 0x2C81B60A,
    OBJECTCONSTRUCTION_XML = 0xC876C85E,
    OBJECTCONSTRUCTION_BIN = 0xC08EC0EE,
    SLOT_XML = 0x4045D294,
    SLOT_BIN = 0x487BF9E4,
    SWM = 0xCF60795E,
    SWARM_BIN = 0x9752E396,
    XML_BIN = 0xE0D83029,
    CAB_XML = 0xA6856948,
    CAB_BIN = 0xC644F440,
    BIG = 0x5BCA8C06,
    BNK = 0xB6B5C271,
    LUA = 0x474999B4,
    LUO = 0x2B8E2411,
    LIGHTBOX_XML = 0xB61215E9,
    LIGHTBOX_BIN = 0xD6215201,
    XMB = 0x1E1E6516,
    TTF = 0xFD72D418,
    TTC = 0x35EBB959,
    RUNTIMESETTINGS_XML = 0x6D3E3FB4,
    FX = 0x6B772503,
    UNKNOWN = 0x00000000,
};

static const std::unordered_map<std::string, DDFFileType> stringToEnum = {
    {"model", DDFFileType::MODEL},
    {"revomodel", DDFFileType::REVOMODEL},
    {"windowsmodel", DDFFileType::WINDOWSMODEL},
    {"download", DDFFileType::DOWNLOAD},
    {"rig", DDFFileType::RIG},
    {"clip", DDFFileType::CLIP},
    {"keynamemap", DDFFileType::KEYNAMEMAP},
    {"geometry", DDFFileType::GEOMETRY},
    {"material", DDFFileType::MATERIAL},
    {"materialset", DDFFileType::MATERIALSET},
    {"oldspeedtree", DDFFileType::OLDSPEEDTREE},
    {"speedtree", DDFFileType::SPEEDTREE},
    {"dds", DDFFileType::DDS},
    {"compositetexture", DDFFileType::COMPOSITETEXTURE},
    {"simoutfit", DDFFileType::SIMOUTFIT},
    {"level_xml", DDFFileType::LEVEL_XML},
    {"level_bin", DDFFileType::LEVEL_BIN},
    {"physics", DDFFileType::PHYSICS},
    {"lightset_xml", DDFFileType::LIGHTSET_XML},
    {"lightset_bin", DDFFileType::LIGHTSET_BIN},
    {"xml", DDFFileType::XML},
    {"footprintset", DDFFileType::FOOTPRINTSET},
    {"objectconstruction_xml", DDFFileType::OBJECTCONSTRUCTION_XML},
    {"objectconstruction_bin", DDFFileType::OBJECTCONSTRUCTION_BIN},
    {"slot_xml", DDFFileType::SLOT_XML},
    {"slot_bin", DDFFileType::SLOT_BIN},
    {"swm", DDFFileType::SWM},
    {"swarm_bin", DDFFileType::SWARM_BIN},
    {"xml_bin", DDFFileType::XML_BIN},
    {"cab_xml", DDFFileType::CAB_XML},
    {"cab_bin", DDFFileType::CAB_BIN},
    {"big", DDFFileType::BIG},
    {"bnk", DDFFileType::BNK},
    {"lua", DDFFileType::LUA},
    {"luo", DDFFileType::LUO},
    {"lightbox_xml", DDFFileType::LIGHTBOX_XML},
    {"lightbox_bin", DDFFileType::LIGHTBOX_BIN},
    {"xmb", DDFFileType::XMB},
    {"ttf", DDFFileType::TTF},
    {"ttc", DDFFileType::TTC},
    {"runtimesettings_xml", DDFFileType::RUNTIMESETTINGS_XML},
    {"fx", DDFFileType::FX}
};

class Asset {
public:
    EA::ResourceMan::Key key;
    mutable EA::ResourceMan::Key selfKey={0,0,0};
    std::filesystem::path path;

    static DDFFileType GetFileType(const std::string &extension);
    static std::string GetTypeName(uint32_t value);
};



#endif //ASSET_H
