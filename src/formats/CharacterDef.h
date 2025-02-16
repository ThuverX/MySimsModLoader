#ifndef CHARACTERDEF_H
#define CHARACTERDEF_H

#include <string>
#include <vector>
#include <unordered_map>

struct LuaData {};

struct Interest {
    std::string Type;
    int Value;
};

struct Influences {
    std::vector<Interest> Interests;
};

struct CharacterDef {
    std::string Script;
    std::string BodyModel;
    std::string BodyDM;
    std::string HeadModel;
    int HairIndex;
    int HatIndex;
    std::string EyesModel;
    std::string MouthModel;
    int SkinToneIndex;
    int MiscOpIndex;
    int MiscModIndex;
    std::string VoxGroup;
    double PitchAdjustment;
    std::string Rig;
    std::string CollisionInfo;
    std::unordered_map<std::string, std::string> Clips;
    LuaData LuaData;
    Influences Influences;

    static bool Read(CharacterDef &instance, void *data, size_t size);
};

#endif // CHARACTERDEF_H
