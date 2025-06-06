//
// Created by exozg on 23/05/2025.
//

#ifndef MATERIALBUILDER_H
#define MATERIALBUILDER_H
#include "../materials/Material.h"

#include <unordered_map>
#include <string>

enum class ShaderType: uint32_t {
    casFace = 0xCC1046A7,
    casFaceShiny = 0xA654AB38,
    casFaceShinyModulatedAccents = 0xD91354E0,
    casFaceShinyModulatedAccentsSkinned = 0x94AB09A4,
    casFaceShinyOpaqueAccents = 0x108747D2,
    casFaceShinyOpaqueAccentsSkinned = 0x6548EEC2,
    casFaceShinyReversedAccents = 0xB445141B,
    casFaceShinyReversedAccentsSkinned = 0x4F6AD45D,
    casFaceShinySkinned = 0x5266048C,
    casFaceSkinned = 0x04EB01C1,
    casHat = 0x24F7F0DB,
    casHatSkinned = 0x6683DB1D,
    customMask = 0xFA625054,
    customMaskSkinned = 0x68E561D0,
    Face = 0xA0C00292,
    FaceSkinned = 0x7BF66582,
    idColor = 0x41AD8167,
    idColorSkinned = 0x544B0481,
    lambert = 0x94773578,
    lambertEnvMap = 0x04EFD88D,
    lambertEnvMapSkinned = 0x57B0516B,
    lambertLightMap = 0x11D90EB6,
    lambertLightMapSkinned = 0x535168FE,
    lambertSkinned = 0x2650FECC,
    lambertTransparent = 0x8AAFBD1E,
    lambertTransparentEnvMap = 0x8C801E73,
    lambertTransparentEnvMapSkinned = 0x5A9B2365,
    lambertTransparentSkinned = 0xE16E35F6,
    lambertTritone = 0x9D8D3A6F,
    lineset = 0xC4C534C9,
    phong = 0xB9105A6D,
    phongSkinned = 0x65DC8ACB,
    plumbob = 0xDEF16564,
    shadererror = 0xA2A9B074,
    shadowMappingIDMapFill = 0x286A83BB,
    shadowMappingIDMapLookup_3x3Kernel = 0xDC3D72E4,
    shadowMappingIDMapLookup_4x4Kernel_Pass1 = 0x53B37265,
    shadowMappingIDMapLookup_4x4Kernel_Pass2 = 0x53B37266,
    shadowProjector = 0x293A5873,
    shadow_low = 0x7674D0AE,
    shadow_low_I8 = 0xBAF2CFCA,
    shadow_low_I8_invAlpha = 0x6F44AB1E,
    shadow_low_invAlpha = 0xDA720C6A,
    shadow_med = 0xA07751EA,
    ShinyGlossMap = 0xFF49577E,
    ShinyGlossMapSkinned = 0x8BFF4FD6,
    ShinyGlossMapTransparent = 0x14597B24,
    ShinyGlossMapTransparentSkinned = 0xFA6AD820,
    Sim = 0x22706EFA,
    SimPreBakedTextures_Tex1Mod_Tex2Mod = 0xCC7E68DB,
    SimPreBakedTextures_Tex1Mod_Tex2ModSkinned = 0xD27B831D,
    SimPreBakedTextures_Tex1Mod_Tex2Op = 0xC1C024D4,
    SimPreBakedTextures_Tex1Mod_Tex2OpSkinned = 0x28C2BB50,
    SimPreBakedTextures_Tex1Op_Tex2Mod = 0xCBE11A70,
    SimPreBakedTextures_Tex1Op_Tex2ModSkinned = 0xC6766774,
    SimPreBakedTextures_Tex1Op_Tex2Op = 0x46681DE9,
    SimPreBakedTextures_Tex1Op_Tex2OpSkinned = 0x51C6313F,
    SimSkinned = 0xE70021FA,
    swarmAdditive = 0x0BA3C15B,
    swarmDecal = 0x79F61866,
    swarmDecalIgnoreDepth = 0x80FF4623,
    swarmDepthDecal = 0x39A9B569,
    swarmModulate = 0x2CD7906E,
    terrainLightMapTinted = 0x224E7FEE,
    text = 0xB12BFA38,
    video = 0xB1CC1AF6,
    Water = 0x9E3C3DFA,
    WaterShiny = 0xE3B338AF,
    WaterShinyBumpy = 0xFF8234C4
};

static const std::unordered_map<std::string, ShaderType> stringToEnum = {
    {"casFace", ShaderType::casFace},
    {"casFaceShiny", ShaderType::casFaceShiny},
    {"casFaceShinyModulatedAccents", ShaderType::casFaceShinyModulatedAccents},
    {"casFaceShinyModulatedAccentsSkinned", ShaderType::casFaceShinyModulatedAccentsSkinned},
    {"casFaceShinyOpaqueAccents", ShaderType::casFaceShinyOpaqueAccents},
    {"casFaceShinyOpaqueAccentsSkinned", ShaderType::casFaceShinyOpaqueAccentsSkinned},
    {"casFaceShinyReversedAccents", ShaderType::casFaceShinyReversedAccents},
    {"casFaceShinyReversedAccentsSkinned", ShaderType::casFaceShinyReversedAccentsSkinned},
    {"casFaceShinySkinned", ShaderType::casFaceShinySkinned},
    {"casFaceSkinned", ShaderType::casFaceSkinned},
    {"casHat", ShaderType::casHat},
    {"casHatSkinned", ShaderType::casHatSkinned},
    {"customMask", ShaderType::customMask},
    {"customMaskSkinned", ShaderType::customMaskSkinned},
    {"Face", ShaderType::Face},
    {"FaceSkinned", ShaderType::FaceSkinned},
    {"idColor", ShaderType::idColor},
    {"idColorSkinned", ShaderType::idColorSkinned},
    {"lambert", ShaderType::lambert},
    {"lambertEnvMap", ShaderType::lambertEnvMap},
    {"lambertEnvMapSkinned", ShaderType::lambertEnvMapSkinned},
    {"lambertLightMap", ShaderType::lambertLightMap},
    {"lambertLightMapSkinned", ShaderType::lambertLightMapSkinned},
    {"lambertSkinned", ShaderType::lambertSkinned},
    {"lambertTransparent", ShaderType::lambertTransparent},
    {"lambertTransparentEnvMap", ShaderType::lambertTransparentEnvMap},
    {"lambertTransparentEnvMapSkinned", ShaderType::lambertTransparentEnvMapSkinned},
    {"lambertTransparentSkinned", ShaderType::lambertTransparentSkinned},
    {"lambertTritone", ShaderType::lambertTritone},
    {"lineset", ShaderType::lineset},
    {"phong", ShaderType::phong},
    {"phongSkinned", ShaderType::phongSkinned},
    {"plumbob", ShaderType::plumbob},
    {"shadererror", ShaderType::shadererror},
    {"shadowMappingIDMapFill", ShaderType::shadowMappingIDMapFill},
    {"shadowMappingIDMapLookup_3x3Kernel", ShaderType::shadowMappingIDMapLookup_3x3Kernel},
    {"shadowMappingIDMapLookup_4x4Kernel_Pass1", ShaderType::shadowMappingIDMapLookup_4x4Kernel_Pass1},
    {"shadowMappingIDMapLookup_4x4Kernel_Pass2", ShaderType::shadowMappingIDMapLookup_4x4Kernel_Pass2},
    {"shadowProjector", ShaderType::shadowProjector},
    {"shadow_low", ShaderType::shadow_low},
    {"shadow_low_I8", ShaderType::shadow_low_I8},
    {"shadow_low_I8_invAlpha", ShaderType::shadow_low_I8_invAlpha},
    {"shadow_low_invAlpha", ShaderType::shadow_low_invAlpha},
    {"shadow_med", ShaderType::shadow_med},
    {"ShinyGlossMap", ShaderType::ShinyGlossMap},
    {"ShinyGlossMapSkinned", ShaderType::ShinyGlossMapSkinned},
    {"ShinyGlossMapTransparent", ShaderType::ShinyGlossMapTransparent},
    {"ShinyGlossMapTransparentSkinned", ShaderType::ShinyGlossMapTransparentSkinned},
    {"Sim", ShaderType::Sim},
    {"SimPreBakedTextures_Tex1Mod_Tex2Mod", ShaderType::SimPreBakedTextures_Tex1Mod_Tex2Mod},
    {"SimPreBakedTextures_Tex1Mod_Tex2ModSkinned", ShaderType::SimPreBakedTextures_Tex1Mod_Tex2ModSkinned},
    {"SimPreBakedTextures_Tex1Mod_Tex2Op", ShaderType::SimPreBakedTextures_Tex1Mod_Tex2Op},
    {"SimPreBakedTextures_Tex1Mod_Tex2OpSkinned", ShaderType::SimPreBakedTextures_Tex1Mod_Tex2OpSkinned},
    {"SimPreBakedTextures_Tex1Op_Tex2Mod", ShaderType::SimPreBakedTextures_Tex1Op_Tex2Mod},
    {"SimPreBakedTextures_Tex1Op_Tex2ModSkinned", ShaderType::SimPreBakedTextures_Tex1Op_Tex2ModSkinned},
    {"SimPreBakedTextures_Tex1Op_Tex2Op", ShaderType::SimPreBakedTextures_Tex1Op_Tex2Op},
    {"SimPreBakedTextures_Tex1Op_Tex2OpSkinned", ShaderType::SimPreBakedTextures_Tex1Op_Tex2OpSkinned},
    {"SimSkinned", ShaderType::SimSkinned},
    {"swarmAdditive", ShaderType::swarmAdditive},
    {"swarmDecal", ShaderType::swarmDecal},
    {"swarmDecalIgnoreDepth", ShaderType::swarmDecalIgnoreDepth},
    {"swarmDepthDecal", ShaderType::swarmDepthDecal},
    {"swarmModulate", ShaderType::swarmModulate},
    {"terrainLightMapTinted", ShaderType::terrainLightMapTinted},
    {"text", ShaderType::text},
    {"video", ShaderType::video},
    {"Water", ShaderType::Water},
    {"WaterShiny", ShaderType::WaterShiny},
    {"WaterShinyBumpy", ShaderType::WaterShinyBumpy}
};

ShaderType GetShaderType(const std::string &type);
std::string GetShaderName(uint32_t value);

class MaterialBuilder {
    Material mat{};

public:
    MaterialBuilder& withShader(ShaderType type);
    MaterialBuilder& withKey(const EA::ResourceMan::Key& key);
    MaterialBuilder& withColorParameter(uint32_t name, float x, float y, float z, float w);
    MaterialBuilder& withValueParameter(uint32_t name, int value);
    MaterialBuilder& withKeyParameter(uint32_t name, EA::ResourceMan::Key key);
    Material build();
};



#endif //MATERIALBUILDER_H
