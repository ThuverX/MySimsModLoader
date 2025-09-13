//
// Created by exozg on 02/08/2025.
//

#ifndef ASSETWINDOW_H
#define ASSETWINDOW_H
#include "HexWindow.h"
#include "imgui.h"
#include "../DebugUI.h"
#include "../../assets/Assets.h"
#include "../../resource/IdResolver.h"


class AssetWindow final : public DebugWindow {
    Msml::Core::Assets *mAssets = nullptr;
    std::vector<EA::ResourceMan::Key> mKeys;
    bool mIsInitialized = false;

    std::string mName = "Assets";

    void Refresh() {
        mAssets->GetKeys(mKeys);
    }

    void draw() override {
        if (!mIsInitialized) {
            mAssets = &Msml::Core::Assets::GetInstance();
            Refresh();
            mIsInitialized = true;
        }

        if (ImGui::Begin((mName + "##window_assets").c_str(), &mIsVisible)) {

            if (ImGui::Button("Refresh")) {
                Refresh();
            }

            ImGuiListClipper clipper;
            clipper.Begin(static_cast<int>(mKeys.size()));
            while (clipper.Step()) {
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
                    ImGui::PushID(i);

                    if (auto key = mKeys[i];ImGui::Selectable(
                        Msml::Core::Resource::IdResolver::ToFilename(key).c_str())) {
                        EA::ResourceMan::IRecord *record = nullptr;
                        if (mAssets->mDatabase->OpenRecord2(key, &record, EA::IO::AccessFlags::kRead,
                                                          EA::IO::CD::kLoadAllFiles,
                                                          0, nullptr)) {
                            const auto kStream = record->GetStream();
                            kStream->AddRef();
                            if (const auto kWindow = Msml::Core::DebugUI::GetInstance().GetWindow<HexWindow>("hexviewer"); kWindow != nullptr) {
                                kWindow->Open(key, kStream);
                            }
                            kStream->Release();
                        }
                    }
                    ImGui::PopID();
                }
            }
        }
        ImGui::End();
    }

    const std::string &getName() const override {
        return mName;
    }
};

#endif //ASSETWINDOW_H
