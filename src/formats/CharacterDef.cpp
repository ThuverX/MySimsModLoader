#include "CharacterDef.h"

#include "pugixml.hpp"
#include "../util/Logger.h"

bool CharacterDef::Read(CharacterDef &instance, void *data, size_t size) {

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(static_cast<const char *>(data), size);

    if (!result) {
        MSML_LOG_ERROR("Failed to parse CharacterDef XML: %s", result.description());
        return false;
    }

    auto CharacterDef = doc.child("CharacterDef");

    instance.Script = CharacterDef.child("Script").text().as_string();
    instance.BodyModel = CharacterDef.child("BodyModel").text().as_string();
    instance.BodyDM = CharacterDef.child("BodyDM").text().as_string();
    instance.HeadModel = CharacterDef.child("HeadModel").text().as_string();
    instance.HairIndex = CharacterDef.child("HairIndex").text().as_int();
    instance.HatIndex = CharacterDef.child("HatIndex").text().as_int();
    instance.EyesModel = CharacterDef.child("EyesModel").text().as_string();
    instance.MouthModel = CharacterDef.child("MouthModel").text().as_string();
    instance.SkinToneIndex = CharacterDef.child("SkinToneIndex").text().as_int();
    instance.MiscOpIndex = CharacterDef.child("MiscOpIndex").text().as_int();
    instance.MiscModIndex = CharacterDef.child("MiscModIndex").text().as_int();
    instance.VoxGroup = CharacterDef.child("VoxGroup").text().as_string();
    instance.PitchAdjustment = CharacterDef.child("PitchAdjustment").text().as_double();
    instance.Rig = CharacterDef.child("Rig").text().as_string();
    instance.CollisionInfo = CharacterDef.child("CollisionInfo").text().as_string();

    // Read clips
    for (pugi::xml_node clip : CharacterDef.child("Clips").children()) {
        instance.Clips[clip.name()] = clip.text().as_string();
    }

    // Read influences
    for (pugi::xml_node interest : CharacterDef.child("Influences").children()) {
        instance.Influences.Interests.push_back(Interest{
            interest.attribute("type").as_string(),
            interest.text().as_int()
        });
    }

    return true;
}
