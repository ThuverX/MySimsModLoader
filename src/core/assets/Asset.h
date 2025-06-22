//
// Created by exozg on 24/01/2025.
//

#ifndef ASSET_H
#define ASSET_H

#include <unordered_map>
#include <utility>
#include <filesystem>

#include "../../EA/ResourceMan/ResourceKey.h"

namespace Msml::Core {
    enum class FileType: uint32_t {
        MODEL = 0x01661233,
        REVOMODEL = 0xF9E50586,
        WINDOWSMODEL = 0xB359C791,
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

    static const std::unordered_map<std::string, FileType> kStringToEnum = {
        {"model", FileType::MODEL},
        {"revomodel", FileType::REVOMODEL},
        {"windowsmodel", FileType::WINDOWSMODEL},
        {"download", FileType::DOWNLOAD},
        {"rig", FileType::RIG},
        {"clip", FileType::CLIP},
        {"keynamemap", FileType::KEYNAMEMAP},
        {"geometry", FileType::GEOMETRY},
        {"material", FileType::MATERIAL},
        {"materialset", FileType::MATERIALSET},
        {"oldspeedtree", FileType::OLDSPEEDTREE},
        {"speedtree", FileType::SPEEDTREE},
        {"dds", FileType::DDS},
        {"compositetexture", FileType::COMPOSITETEXTURE},
        {"simoutfit", FileType::SIMOUTFIT},
        {"level_xml", FileType::LEVEL_XML},
        {"level_bin", FileType::LEVEL_BIN},
        {"physics", FileType::PHYSICS},
        {"lightset_xml", FileType::LIGHTSET_XML},
        {"lightset_bin", FileType::LIGHTSET_BIN},
        {"xml", FileType::XML},
        {"footprintset", FileType::FOOTPRINTSET},
        {"objectconstruction_xml", FileType::OBJECTCONSTRUCTION_XML},
        {"objectconstruction_bin", FileType::OBJECTCONSTRUCTION_BIN},
        {"slot_xml", FileType::SLOT_XML},
        {"slot_bin", FileType::SLOT_BIN},
        {"swm", FileType::SWM},
        {"swarm_bin", FileType::SWARM_BIN},
        {"xml_bin", FileType::XML_BIN},
        {"cab_xml", FileType::CAB_XML},
        {"cab_bin", FileType::CAB_BIN},
        {"big", FileType::BIG},
        {"bnk", FileType::BNK},
        {"lua", FileType::LUA},
        {"luo", FileType::LUO},
        {"lightbox_xml", FileType::LIGHTBOX_XML},
        {"lightbox_bin", FileType::LIGHTBOX_BIN},
        {"xmb", FileType::XMB},
        {"ttf", FileType::TTF},
        {"ttc", FileType::TTC},
        {"runtimesettings_xml", FileType::RUNTIMESETTINGS_XML},
        {"fx", FileType::FX}
    };

    enum class AssetType : uint8_t {
        kPath,
        kBuffer,
        kRedirect
    };

    class Asset {
    public:
        EA::ResourceMan::Key mKey;
        std::filesystem::path mPath;
        EA::ResourceMan::Key mKeyRedirect = {};
        std::vector<uint8_t> mBuffer;
        AssetType mType = AssetType::kPath;

        explicit Asset(const EA::ResourceMan::Key kKey, const AssetType kType = AssetType::kPath): mKey(kKey), mType(kType) {}

        EA::IO::IStream* GetStream() const;

        static FileType GetFileType(const std::string &extension);

        static std::string GetTypeName(uint32_t value);

        static EA::ResourceMan::Key GetKey(const std::filesystem::path &path);
    };
}
#endif
