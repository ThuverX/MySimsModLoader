//
// Created by exozg on 11/06/2025.
//

#include "DebugUI.h"

#include <ranges>

#include "ImGuiHook.h"

#include "../resource/IdResolver.h"
#include "../util/StreamUtil.h"


namespace Msml::Core {
    DebugUI &DebugUI::GetInstance() {
        static DebugUI instance;
        return instance;
    }

    void DebugUI::Init() {
        // assets = &Assets::GetInstance();
        // hex_state.ReadOnly = true;
    }

    void DebugUI::Draw() {
        ImGui::ShowDemoWindow();
        // DrawAssets();
        // // DrawDatabase();
        //
        // if (use_hex_buffer) {
        //     ImGui::Begin("Hex Buffer");
        //     ImGui::Text("%s", resource::IdResolver::ToFilename(active_key).c_str());
        //     ImGui::BeginHexEditor("##HexEditor", &hex_state);
        //     ImGui::EndHexEditor();
        //     ImGui::End();
        // }
    }
    //
    // void DebugUI::DrawAssets() {
    //     ImGui::Begin("Assets");
    //
    //     int id = 0;
    //     for (const auto &asset: assets->database->assets | std::views::values) {
    //         ImGui::PushID(id); {
    //             ImGui::LabelText("Name", resource::IdResolver::ToHumanReadable(asset->key).c_str());
    //             switch (asset->type) {
    //                 default:
    //                 case assets::PATH:
    //                     ImGui::LabelText("Type", "PATH");
    //                     ImGui::LabelText("Path", asset->path.string().c_str());
    //                     break;
    //                 case assets::BUFFER:
    //                     ImGui::LabelText("Type", "BUFFER");
    //
    //                     break;
    //                 case assets::REDIRECT:
    //                     ImGui::LabelText("Type", "BUFFER");
    //                     ImGui::LabelText("Redirect", resource::IdResolver::ToHumanReadable(asset->key_redirect).c_str());
    //                     break;
    //             }
    //
    //             if (ImGui::Button("Open")) {
    //                 active_key = asset->key;
    //                 hex_buffer = util::StreamUtil::ReadBytes(asset->GetStream());
    //                 hex_state.Bytes = (void *) hex_buffer.data();
    //                 hex_state.MaxBytes = hex_buffer.size();
    //                 use_hex_buffer = true;
    //             }
    //
    //             ImGui::Separator();
    //         }
    //         ImGui::PopID();
    //
    //         id++;
    //     }
    //
    //     ImGui::End();
    // }
    //
    // void DebugUI::DrawDatabase() {
    //     ImGui::Begin("Database");
    //     if (ImGui::Button("Refresh")) {
    //         keys.clear();
    //         assets->database->GetKeys(keys);
    //         filtered_keys = keys;
    //     }
    //
    //     // if (ImGui::InputText("Search", search_string, IM_ARRAYSIZE(search_string))) {
    //     //     filtered_keys.clear();
    //     //     const auto [instance, type, group] = assets::Asset::GetKey(search_string);
    //     //     EA::ResourceMan::Key filter_key = {
    //     //         .instance = instance != 0 ? instance : uint64_t(-1),
    //     //         .type = type != 0 ? type : uint32_t(-1) ,
    //     //         .group =  group != 0 ? group : uint32_t(-1)
    //     //     };
    //     //     std::ranges::copy_if(keys, std::back_inserter(filtered_keys), [filter_key](const EA::ResourceMan::Key& k) {
    //     //         return (filter_key.instance == uint64_t(-1) || k.instance == filter_key.instance) &&
    //     //                (filter_key.type    == uint32_t(-1) || k.type    == filter_key.type) &&
    //     //                (filter_key.group   == uint32_t(-1) || k.group   == filter_key.group);
    //     //     });
    //     // }
    //
    //     ImGui::Text("%d Keys", filtered_keys.size());
    //
    //     ImGuiListClipper clipper;
    //     clipper.Begin(static_cast<int>(filtered_keys.size()));
    //     while (clipper.Step()) {
    //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
    //             if (auto key = filtered_keys[i]; ImGui::Selectable(resource::IdResolver::ToHumanReadable(key).c_str())) {
    //                 EA::ResourceMan::IRecord *record;
    //                 if (assets->database->OpenRecord2(key, &record, EA::IO::AccessFlags::Read, EA::IO::CD::LoadAllFiles,
    //                                                   0, nullptr)) {
    //                     const auto stream = record->GetStream();
    //                     stream->AddRef();
    //                     active_key = key;
    //                     hex_buffer = util::StreamUtil::ReadBytes(stream);
    //                     hex_state.Bytes = static_cast<void *>(hex_buffer.data());
    //                     hex_state.MaxBytes = hex_buffer.size();
    //                     use_hex_buffer = true;
    //                     stream->Release();
    //                 }
    //             }
    //         }
    //     }
    //
    //     ImGui::End();
    // }
    //
    // // Should this be here? We really don't have another place for the forge functions
    // void writeLogHooked(uint32_t level, const char *filename, int line_number, const char *message, ...) {
    //     va_list args;
    //     va_start(args, message);
    //
    //     std::vector<char> buffer(1024);
    //     int required = vsnprintf(buffer.data(), buffer.size(), message, args);
    //
    //     va_end(args);
    //
    //     if (required < 0) {
    //         return;
    //     }
    //
    //     // Resize if needed
    //     if (static_cast<size_t>(required) >= buffer.size()) {
    //         buffer.resize(required + 1);
    //         va_start(args, message);
    //         vsnprintf(buffer.data(), buffer.size(), message, args);
    //         va_end(args);
    //     }
    //
    //     auto local_level = system::Logger::LogLevel::INFO;
    //
    //     switch (level) {
    //         case 0:
    //         case 1:
    //             return;
    //         case 2: local_level = system::Logger::LogLevel::DEBUG;
    //             break;
    //         case 4: local_level = system::Logger::LogLevel::INFO;
    //             break;
    //         case 8: local_level = system::Logger::LogLevel::WARNING;
    //             break;
    //         case 16: local_level = system::Logger::LogLevel::ERR;
    //             break;
    //         default: local_level = system::Logger::LogLevel::INFO;
    //     }
    //
    //     system::Logger::Log(local_level, false, filename, line_number,
    //                         "%s",
    //                         buffer.data());
    // }

    void DebugUI::Install() {
        // WriteLogHook.Install(&writeLogHooked);
        d3d12_queuePresentHook.Install(&hkPresent);
    }
}
