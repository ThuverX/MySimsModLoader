#include "CharacterDef.h"

#include "pugixml.hpp"
#include <iostream>

void CharacterDef::Read(CharacterDef &instance, void *data, size_t size) {

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(static_cast<const char *>(data), size);

    // TODO: Add error handling

    instance.Script = doc.child("CharacterDef").child("Script").text().as_string();
    instance.BodyModel = doc.child("CharacterDef").child("BodyModel").text().as_string();
    instance.BodyDM = doc.child("CharacterDef").child("BodyDM").text().as_string();
    instance.HeadModel = doc.child("CharacterDef").child("HeadModel").text().as_string();
    instance.HairIndex = doc.child("CharacterDef").child("HairIndex").text().as_int();
    instance.HatIndex = doc.child("CharacterDef").child("HatIndex").text().as_int();
    instance.EyesModel = doc.child("CharacterDef").child("EyesModel").text().as_string();
    instance.MouthModel = doc.child("CharacterDef").child("MouthModel").text().as_string();
    instance.SkinToneIndex = doc.child("CharacterDef").child("SkinToneIndex").text().as_int();
    instance.MiscOpIndex = doc.child("CharacterDef").child("MiscOpIndex").text().as_int();
    instance.MiscModIndex = doc.child("CharacterDef").child("MiscModIndex").text().as_int();
    instance.VoxGroup = doc.child("CharacterDef").child("VoxGroup").text().as_string();
    instance.PitchAdjustment = doc.child("CharacterDef").child("PitchAdjustment").text().as_double();
    instance.Rig = doc.child("CharacterDef").child("Rig").text().as_string();
    instance.CollisionInfo = doc.child("CharacterDef").child("CollisionInfo").text().as_string();

    // Read clips
    for (pugi::xml_node clip : doc.child("CharacterDef").child("Clips").children()) {
        instance.Clips[clip.name()] = clip.text().as_string();
    }

    // Read influences
    for (pugi::xml_node interest : doc.child("CharacterDef").child("Influences").children()) {
        instance.Influences.Interests.push_back(Interest{
            interest.attribute("type").as_string(),
            interest.text().as_int()
        });
    }
}
