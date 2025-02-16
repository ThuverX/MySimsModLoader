#include "ObjectDef.h"

#include "pugixml.hpp"
#include <iostream>

void ObjectDef::Read(ObjectDef &instance, void *data, size_t size) {

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(static_cast<const char *>(data), size);

    /*std::string Model;
    std::string Rig;
    std::string Script;
    std::string MaxAttachments;
    std::string CollisionInfo;
    bool NoFootPrint;*/
    instance.Model = doc.child("ObjectDef").child("Model").text().as_string();
    instance.Rig = doc.child("ObjectDef").child("Rig").text().as_string();
    instance.Script = doc.child("ObjectDef").child("Script").text().as_string();
    instance.MaxAttachments = doc.child("ObjectDef").child("MaxAttachments").text().as_string();
    instance.CollisionInfo = doc.child("ObjectDef").child("CollisionInfo").text().as_string();
    instance.NoFootPrint = doc.child("ObjectDef").child("NoFootPrint").text().as_bool();
}

