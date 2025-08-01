//
// Created by exozg on 13/03/2025.
//

#include "SwarmTweaker.h"

#include "../../core/hash/FNV.h"
#include "../../core/system/Logger.h"
#include "../../core/util/StreamUtil.h"
#include "../../EA/IO/FileStream.h"
#include "../../EA/IO/MemoryStream.h"

const EA::ResourceMan::Key kMasterSwarmKey32 = {
    .mInstance = Msml::Hash::FNV::FromString64("MySims"),
    .mType = static_cast<uint32_t>(Msml::Core::FileType::SWM),
    .mGroup = 0,
};

const EA::ResourceMan::Key kMasterSwarmKey64 = {
    .mInstance = 0xb63f54437ba837ff,
    .mType = static_cast<uint32_t>(Msml::Core::FileType::SWM),
    .mGroup = 0,
};

bool SwarmTweaker::OnLoad(Msml::Core::Resource::CustomRecord &asset) {
    if (asset.GetKey() == kMasterSwarmKey32 || asset.GetKey() == kMasterSwarmKey64) {
        const size_t kCount = mSwarmsToAdd.size();

        if (kCount == 0) {
            return false;
        }

        MSML_LOG_INFO("Patching Master Swarm with %d file(s)", kCount);

        std::string data = Msml::Core::Util::StreamUtil::ReadString(asset.GetStream());

        for (const auto &swarmToAdd: mSwarmsToAdd) {
            auto *const kStream = new EA::IO::FileStream(swarmToAdd->mPath);
            std::string stringToAdd = Msml::Core::Util::StreamUtil::ReadString(kStream);

            data += "\n" + stringToAdd;
        }

        auto *const kStream = new EA::IO::MemoryStream(data.data(), data.size());

        asset.SetStream(kStream);

        return true;
    }

    return false;
}

bool SwarmTweaker::OnRegister(Msml::Core::Asset &asset) {
    if (asset.mKey.mType == static_cast<uint32_t>(Msml::Core::FileType::SWM)) {
        mSwarmsToAdd.push_back(&asset);
        return true;
    }

    return false;
}
