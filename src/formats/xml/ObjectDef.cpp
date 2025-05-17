#include "ObjectDef.h"

#include "pugixml.hpp"
#include "../../core/system/Logger.h"

bool ObjectDef::Read(ObjectDef &instance, void *data, size_t size) {

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(static_cast<const char *>(data), size);

    if (!result) {
        MSML_LOG_ERROR("Failed to parse ObjectDef XML: %s", result.description());
        return false;
    }

    auto ObjectDef = doc.child("ObjectDef");

    instance.Model = ObjectDef.child("Model").text().as_string();
    instance.Rig = ObjectDef.child("Rig").text().as_string();
    instance.Script = ObjectDef.child("Script").text().as_string();
    instance.MaxAttachments = ObjectDef.child("MaxAttachments").text().as_string();
    instance.CollisionInfo = ObjectDef.child("CollisionInfo").text().as_string();
    instance.NoFootPrint = ObjectDef.child("NoFootPrint").text().as_bool();

    return true;
}

