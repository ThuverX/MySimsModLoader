//
// Created by exozg on 03/08/2025.
//

#ifndef LUAWINDOW_H
#define LUAWINDOW_H

#include "imgui.h"
#include "../DebugUI.h"
#include "../../hooks/LuaHook.h"
#include <map>
#include "../../include/lua.h"
#include "../signatures/sigdef.h"

enum class LuaObjectType {
    kString,
    kNumber,
    kBoolean,
    kNil,
    kUserdata,
    kFunction,
    kTable
};

const char *LuaObjectTypeToString(LuaObjectType e) {
    switch (e) {
        case LuaObjectType::kString: return "string";
        case LuaObjectType::kNumber: return "number";
        case LuaObjectType::kBoolean: return "boolean";
        case LuaObjectType::kNil: return "nil";
        case LuaObjectType::kUserdata: return "userdata";
        case LuaObjectType::kTable: return "table";
        default: return "unknown";
    }
}

struct LuaObject {
    bool mIsLoaded = false;
    bool mIsOpen = false;
    LuaObjectType mType;
    std::map<std::string, std::shared_ptr<LuaObject> > mChildren;

    std::string mStringValue;
    float mNumberValue = 0;
    bool mBooleanValue = false;
    int mIndex = 0;
    int mLuaRef = LUA_NOREF;
};


inline std::string LuaObjectToString(LuaObject *object) {
    switch (object->mType) {
        case LuaObjectType::kString:
            return object->mStringValue;
        case LuaObjectType::kNumber:
            return std::to_string(object->mNumberValue);
        case LuaObjectType::kBoolean:
            return object->mBooleanValue ? "true" : "false";
        case LuaObjectType::kFunction:
            return "function";
        case LuaObjectType::kNil:
            return "nil";
        case LuaObjectType::kUserdata:
            return "{}";
        case LuaObjectType::kTable:
            return "{}";
    }
    return "nil";
}

inline void LuaToObject(lua_State *L, LuaObject *object, int index = -1) {
    if (object == nullptr) {
        return;
    }

    index = lua_absindex(L, index);
    object->mIndex = index;

    const int kType = lua_type(L, index);
    object->mIsLoaded = true;

    switch (kType) {
        case LUA_TSTRING:
            object->mType = LuaObjectType::kString;
            object->mStringValue = lua_tostring(L, index);
            break;

        case LUA_TNUMBER:
            object->mType = LuaObjectType::kNumber;
            object->mNumberValue = static_cast<float>(lua_tonumber(L, index));
            break;

        case LUA_TBOOLEAN:
            object->mType = LuaObjectType::kBoolean;
            object->mBooleanValue = lua_toboolean(L, index);
            break;

        case LUA_TFUNCTION:
            object->mType = LuaObjectType::kFunction;
            break;

        case LUA_TNIL:
            object->mType = LuaObjectType::kNil;
            break;

        case LUA_TTABLE:
            object->mType = LuaObjectType::kTable;

            lua_pushnil(L);
            while (lua_next(L, index) != 0) {
                auto key = std::make_shared<LuaObject>();
                LuaToObject(L, key.get(), -2);

                auto keyString = LuaObjectToString(key.get());
                auto value = std::make_shared<LuaObject>();

                if (lua_istable(L, -1)) {
                    lua_pushvalue(L, -1);
                    value->mLuaRef = luaL_ref(L, LUA_REGISTRYINDEX);
                    value->mType = LuaObjectType::kTable;
                    value->mIsLoaded = false;
                } else {
                    LuaToObject(L, value.get(), -1);
                    value->mIsLoaded = true;
                }

                object->mChildren.insert({keyString, value});

                lua_pop(L, 1);
            }
            break;

        case LUA_TUSERDATA:
            object->mType = LuaObjectType::kUserdata;
            break;

        default:
            object->mType = LuaObjectType::kNil;
            break;
    }
}

inline bool LoadTable(lua_State *L, LuaObject *object) {
    if (object->mIsLoaded || object->mType != LuaObjectType::kTable || object->mLuaRef == LUA_NOREF) {
        return false;
    }

    lua_rawgeti(L, LUA_REGISTRYINDEX, object->mLuaRef);
    LuaToObject(L, object, -1);
    lua_pop(L, 1);

    object->mIsLoaded = true;
    return true;
}

class LuaWindow final : public DebugWindow {
public:
    std::string mName = "Lua";
    lua_State *L = nullptr;

    char mLuaInputBuf[512] = "";
    LuaObject *mLuaOutput = nullptr;

    LuaWindow() : L(Msml::Core::Hooks::LuaHook::sGlobalState) {
    }

    void DrawLuaObject(LuaObject *object) {
        if (mLuaOutput == nullptr) {
            return;
        }
        ImGui::BeginGroup();

        switch (object->mType) {
            case LuaObjectType::kString:
                ImGui::Text("\"%s\"", object->mStringValue.c_str());

                break;

            case LuaObjectType::kNumber:
                ImGui::Text("%f", object->mNumberValue);
                break;

            case LuaObjectType::kBoolean:
                ImGui::Text("%s", object->mBooleanValue ? "true" : "false");
                break;

            case LuaObjectType::kFunction:
                ImGui::Text("function");

            case LuaObjectType::kNil:
                ImGui::Text("nil");
                break;

            case LuaObjectType::kTable:
                ImGui::Text("{");
                ImGui::Indent();

                for (const auto &child: object->mChildren) {
                    if (child.second->mType == LuaObjectType::kTable) {
                        if (ImGui::Selectable(child.first.c_str(), &child.second->mIsOpen)) {
                            LoadTable(L, child.second.get());
                        }
                        ImGui::SameLine();
                        ImGui::TextUnformatted("=");
                        ImGui::SameLine();

                        if (child.second->mIsOpen) {
                            DrawLuaObject(child.second.get());
                        } else {
                            ImGui::Text("%s,", LuaObjectToString(child.second.get()).c_str());
                        }
                    } else {
                        ImGui::Text("%s", child.first.c_str());
                        ImGui::SameLine();
                        ImGui::TextUnformatted("=");
                        ImGui::SameLine();
                        ImGui::Text("%s,", LuaObjectToString(child.second.get()).c_str());
                    }
                }

                ImGui::Unindent();
                ImGui::Text("}");
                break;

            case LuaObjectType::kUserdata:
                ImGui::Text("userdata");
                break;
        }

        ImGui::EndGroup();

        // if (ImGui::BeginItemTooltip()) {
        //     ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0F);
        //     ImGui::Text("type: %s", LuaObjectTypeToString(object->mType));
        //     ImGui::Text("index: %i", object->mIndex);
        //     ImGui::PopTextWrapPos();
        //     ImGui::EndTooltip();
        // }
    }


    void draw() override {
        if (ImGui::Begin((mName + "##window_lua").c_str(), &mIsVisible)) {
            ImGui::InputText("##input", mLuaInputBuf, sizeof(mLuaInputBuf), ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::SameLine();

            if (ImGui::Button("Run") || ImGui::IsItemDeactivatedAfterEdit()) {
                std::string code = "return " + std::string(mLuaInputBuf);

                if (Msml::Core::Hooks::LuaHook::RunString(code)) {
                    mLuaOutput = new LuaObject;
                    mLuaOutput->mIsOpen = true;
                    LuaToObject(L, mLuaOutput);
                    lua_pop(L, 1);
                } else {
                    mLuaOutput = nullptr;
                    lua_pop(L, 1);
                }

                mLuaInputBuf[0] = '\0';
            }

            ImGui::Separator();

            if (mLuaOutput != nullptr) {
                DrawLuaObject(mLuaOutput);
            }
        }
        ImGui::End();
    };


    const std::string &getName() const override {
        return mName;
    };
};

#endif //LUAWINDOW_H
