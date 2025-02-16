#include "CharacterDef.h"

#include "pugixml.hpp"
#include <iostream>

void CharacterDef::Read(CharacterDef &instance, const std::string &path) {

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.c_str());

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

    // TODO: Import the rest of the fields
}
