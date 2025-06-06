//
// Created by exozg on 13/03/2025.
//

#include "SwarmTweaker.h"

#include "../../core/hash/FNV.h"
#include "../../core/system/Logger.h"
#include "../../core/util/StreamUtil.h"
#include "../../EA/IO/FileStream.h"
#include "../../EA/IO/MemoryStream.h"

EA::ResourceMan::Key MasterSwarmKey32 = {
    .instance = msml::hash::fnv::FromString64("MySims"),
    .type = msml::core::assets::DDFFileType::SWM,
    .group = 0,
};

EA::ResourceMan::Key MasterSwarmKey64 = {
    .instance = 0xb63f54437ba837ff,
    .type = msml::core::assets::DDFFileType::SWM,
    .group = 0,
};

bool SwarmTweaker::OnLoad(msml::core::resource::CustomRecord &asset) {
    if (asset.GetKey() == MasterSwarmKey32 || asset.GetKey() == MasterSwarmKey64) {
        const size_t count = swarmsToAdd.size();

        if (count == 0) return false;

        MSML_LOG_INFO("Patching Master Swarm with %d file(s)", count);

        std::string data = msml::core::util::StreamUtil::ReadString(asset.GetStream());

        for (const auto & swarm_to_add : swarmsToAdd) {

            const auto stream = new EA::IO::FileStream(swarm_to_add->path);
            std::string string_to_add = msml::core::util::StreamUtil::ReadString(stream);

            data += "\n" + string_to_add;
        }

        const auto stream = new EA::IO::MemoryStream(data.data(), data.size());

        asset.SetStream(stream);

        return true;
    }

    return false;
}

bool SwarmTweaker::OnRegister(msml::core::assets::Asset &asset) {
    if (asset.key.type == msml::core::assets::DDFFileType::SWM) {
        swarmsToAdd.push_back(&asset);
        return true;
    }

    return false;
}
