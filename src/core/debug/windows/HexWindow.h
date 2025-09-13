//
// Created by exozg on 03/08/2025.
//

#ifndef HEXWINDOW_H
#define HEXWINDOW_H

#include "imgui.h"
#include "imgui_hex.h"
#include "../DebugUI.h"
#include "../../resource/IdResolver.h"
#include "../../util/StreamUtil.h"

class HexWindow final : public DebugWindow {
public:
    std::string mName = "Hex Viewer";
    ImGuiHexEditorState mHexState;
    std::vector<uint8_t> mData;

    HexWindow() {
        mHexState.ReadOnly = true;
    }

    void draw() override {
        if (ImGui::Begin((mName + "##window_hexviewer").c_str(), &mIsVisible)) {
            ImGui::BeginHexEditor("##HexEditor", &mHexState);
            ImGui::EndHexEditor();
        }
        ImGui::End();
    };

    void Open(const EA::ResourceMan::Key kKey, EA::IO::IStream* stream) {
        mData = Msml::Core::Util::StreamUtil::ReadBytes(stream);
        mHexState.Bytes = static_cast<void *>(mData.data());
        mHexState.MaxBytes = mData.size();
        mName = "Hex Viewer - " + Msml::Core::Resource::IdResolver::ToHumanReadable(kKey);
    }

    const std::string & getName() const override {
        return mName;
    };
};

#endif //HEXWINDOW_H
