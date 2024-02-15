#include "pch.hpp"

#include <utils/config/config.hpp>

#include <utils/logger/logger.hpp>

#include <utils/stringbuilder/stringbuilder.hpp>

#include <fstream>
#include <filesystem>


Config::Config_t* NewDefaultConfig() {
    return new Config::Config_t{
        {m_ESP_Enable, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}},

        {m_ESP_Use3DBoxes, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{true})}},

        {m_ESP_OtherBoxes,
         {ConfigBase_t::ConfigType_BoolWithColor,
          ConfigBase_t::unique_voidptr(new BoolWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(255, 255, 255, 255))})}},
        {m_ESP_HostageBoxes,
         {ConfigBase_t::ConfigType_BoolWithColor,
          ConfigBase_t::unique_voidptr(new BoolWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(255, 119, 0, 255))})}},
        {m_ESP_WeaponBoxes,
         {ConfigBase_t::ConfigType_BoolWithColor,
          ConfigBase_t::unique_voidptr(new BoolWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(255, 255, 0, 255))})}},
        {m_ESP_ChickenBoxes,
         {ConfigBase_t::ConfigType_BoolWithColor,
          ConfigBase_t::unique_voidptr(new BoolWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(206, 66, 245, 255))})}},

        {m_ESP_EnablePlayerLocalESP, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}},
        {m_ESP_PlayerAimLineLocal,
         {ConfigBase_t::ConfigType_FloatWithColor,
          ConfigBase_t::unique_voidptr(new FloatWithColor{0.f, ImGui::ColorConvertU32ToFloat4(IM_COL32(52, 131, 235, 255))})}},
        {m_ESP_PlayerGlowLocal,
         {ConfigBase_t::ConfigType_BoolWithColor,
          ConfigBase_t::unique_voidptr(new BoolWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(52, 131, 235, 255))})}},
        {m_ESP_PlayerBoxesLocal,
         {ConfigBase_t::ConfigType_BoolWithColor,
          ConfigBase_t::unique_voidptr(new BoolWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(52, 131, 235, 255))})}},
        {m_ESP_PlayerNamesLocal,
         {ConfigBase_t::ConfigType_BoolWithColor,
          ConfigBase_t::unique_voidptr(new BoolWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(52, 131, 235, 255))})}},
        {m_ESP_PlayerArmorBarLocal,
         {ConfigBase_t::ConfigType_IntWithColor,
          ConfigBase_t::unique_voidptr(new IntWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(255, 255, 255, 255))})}},
        {m_ESP_PlayerHealthBarLocal,
         {ConfigBase_t::ConfigType_IntWithColor,
          ConfigBase_t::unique_voidptr(new IntWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(255, 255, 255, 255))})}},
        {m_ESP_PlayerOutOfViewArrowLocal,
         {ConfigBase_t::ConfigType_IntWithColor,
          ConfigBase_t::unique_voidptr(new IntWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(45, 126, 200, 125))})}},
        {m_ESP_PlayerStatusIndicatorLocal,
         {ConfigBase_t::ConfigType_BoolWithColor, ConfigBase_t::unique_voidptr(new BoolWithColor{0, ImVec4{1.f, 1.f, 1.f, 1.f}})}},

        {m_ESP_EnablePlayerEnemyESP, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}},
        {m_ESP_PlayerAimLineEnemy,
         {ConfigBase_t::ConfigType_FloatWithColor,
          ConfigBase_t::unique_voidptr(new FloatWithColor{0.f, ImGui::ColorConvertU32ToFloat4(IM_COL32(255, 199, 0, 255))})}},
        {m_ESP_PlayerGlowEnemy,
         {ConfigBase_t::ConfigType_BoolWithColor,
          ConfigBase_t::unique_voidptr(new BoolWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(255, 0, 0, 255))})}},
        {m_ESP_PlayerBoxesEnemy,
         {ConfigBase_t::ConfigType_BoolWithColor,
          ConfigBase_t::unique_voidptr(new BoolWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(232, 0, 255, 255))})}},
        {m_ESP_PlayerNamesEnemy,
         {ConfigBase_t::ConfigType_BoolWithColor,
          ConfigBase_t::unique_voidptr(new BoolWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(255, 255, 255, 255))})}},
        {m_ESP_PlayerArmorBarEnemy,
         {ConfigBase_t::ConfigType_IntWithColor,
          ConfigBase_t::unique_voidptr(new IntWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(255, 255, 255, 255))})}},
        {m_ESP_PlayerHealthBarEnemy,
         {ConfigBase_t::ConfigType_IntWithColor,
          ConfigBase_t::unique_voidptr(new IntWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(255, 255, 255, 255))})}},
        {m_ESP_PlayerOutOfViewArrowEnemy,
         {ConfigBase_t::ConfigType_IntWithColor,
          ConfigBase_t::unique_voidptr(new IntWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(200, 50, 45, 195))})}},
        {m_ESP_PlayerStatusIndicatorEnemy,
         {ConfigBase_t::ConfigType_BoolWithColor,
          ConfigBase_t::unique_voidptr(new BoolWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(255, 135, 20, 255))})}},

        {m_ESP_EnablePlayerTeammateESP, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}},
        {m_ESP_PlayerAimLineTeammate,
         {ConfigBase_t::ConfigType_FloatWithColor,
          ConfigBase_t::unique_voidptr(new FloatWithColor{0.f, ImGui::ColorConvertU32ToFloat4(IM_COL32(0, 255, 0, 150))})}},
        {m_ESP_PlayerGlowTeammate,
         {ConfigBase_t::ConfigType_BoolWithColor,
          ConfigBase_t::unique_voidptr(new BoolWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(0, 255, 0, 150))})}},
        {m_ESP_PlayerBoxesTeammate,
         {ConfigBase_t::ConfigType_BoolWithColor,
          ConfigBase_t::unique_voidptr(new BoolWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(0, 255, 0, 150))})}},
        {m_ESP_PlayerNamesTeammate,
         {ConfigBase_t::ConfigType_BoolWithColor,
          ConfigBase_t::unique_voidptr(new BoolWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(94, 198, 67, 150))})}},
        {m_ESP_PlayerArmorBarTeammate,
         {ConfigBase_t::ConfigType_IntWithColor,
          ConfigBase_t::unique_voidptr(new IntWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(255, 255, 255, 150))})}},
        {m_ESP_PlayerHealthBarTeammate,
         {ConfigBase_t::ConfigType_IntWithColor,
          ConfigBase_t::unique_voidptr(new IntWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(255, 255, 255, 150))})}},
        {m_ESP_PlayerOutOfViewArrowTeammate,
         {ConfigBase_t::ConfigType_IntWithColor,
          ConfigBase_t::unique_voidptr(new IntWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(50, 200, 45, 125))})}},
        {m_ESP_PlayerStatusIndicatorTeammate,
         {ConfigBase_t::ConfigType_BoolWithColor,
          ConfigBase_t::unique_voidptr(new BoolWithColor{0, ImGui::ColorConvertU32ToFloat4(IM_COL32(255, 255, 255, 125))})}},

        {m_World_HitSound, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}},
        {m_World_KillSound, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}},
        {m_World_ShootSoundsOverride, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}},
        {m_World_RadarHack, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}},
        {m_World_Visuals_NoFlash, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}},

        {m_Player_FastStop, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}},
        {m_Player_BunnyHop, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}},

        {m_Misc_FPS_Limiter, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}},
        {m_Misc_FPS_Limiter_Value, {ConfigBase_t::ConfigType_Int, ConfigBase_t::unique_voidptr(new Base::Float{60})}},
        {m_Misc_BackgroundColor,
         {ConfigBase_t::ConfigType_BoolWithColor,
          ConfigBase_t::unique_voidptr(new BoolWithColor{true, ImGui::ColorConvertU32ToFloat4(IM_COL32(11, 1, 25, 161))})}},
        {m_Misc_KeyBindLists, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}},
        {m_Misc_KeyBindLists_AutoSize, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{true})}},
        {m_Misc_MenuTheme,
         {ConfigBase_t::ConfigType_IntWithColor,
          ConfigBase_t::unique_voidptr(new IntWithColor{1, ImGui::ColorConvertU32ToFloat4(IM_COL32(165, 88, 222, 20))})}},
        {m_Misc_WaterMark, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{true})}},

        {m_Rage_AimBot_Enable, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}},
        {m_Rage_AimBot_AutoShoot, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}},
        {m_Rage_AimBot_FriendlyFire, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}},
        {m_Rage_AimBot_ManualAim,
         {ConfigBase_t::ConfigType_Bool,
          ConfigBase_t::unique_voidptr(
              new Base::Bool{false, {"Manual Aim", Base::KeyBind::TriggerMode::Hold, ImGuiKey_E, Base::KeyBind::ShowMode::OnActive}})}},
        {m_Rage_AimBot_TargetByDistanceOnly,
         {ConfigBase_t::ConfigType_Bool,
          ConfigBase_t::unique_voidptr(
              new Base::Bool{false, {"TargetByDistance", Base::KeyBind::TriggerMode::Toggle, ImGuiKey_F, Base::KeyBind::ShowMode::OnActive}})}},

        {m_Rage_AntiAim_Enable, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}},
        {m_Rage_AntiAim_Pitch, {ConfigBase_t::ConfigType_Int, ConfigBase_t::unique_voidptr(new Base::Int{0})}},
        {m_Rage_AntiAim_Yaw, {ConfigBase_t::ConfigType_Int, ConfigBase_t::unique_voidptr(new Base::Int{0})}},
        {m_Rage_AntiAim_PitchOffset, {ConfigBase_t::ConfigType_Float, ConfigBase_t::unique_voidptr(new Base::Float{0.f})}},
        {m_Rage_AntiAim_YawOffset, {ConfigBase_t::ConfigType_Float, ConfigBase_t::unique_voidptr(new Base::Float{0.f})}},
        {m_Rage_AntiAim_Direction, {ConfigBase_t::ConfigType_Int, ConfigBase_t::unique_voidptr(new Base::Int{0})}},

        {m_FastRender, {ConfigBase_t::ConfigType_Bool, ConfigBase_t::unique_voidptr(new Base::Bool{false})}} };
}

void UpdateConfigKeyBind(Config::ConfigBase_t* pConfigItem) {
    if (pConfigItem->type != Config::ConfigBase_t::ConfigType_Bool) return;

    using TriggerMode = Base::KeyBind::TriggerMode;
    using ShowMode = Base::KeyBind::ShowMode;

    Base::Bool* pData = static_cast<Base::Bool*>(pConfigItem->pData.get());

    ImGui::GetIO().WantCaptureKeyboard = true;

    switch (pData->BindingKey.mode) {
    default:
        break;

    case TriggerMode::Disabled:
        break;


    case TriggerMode::Toggle:
        if (ImGui::IsKeyPressed(pData->BindingKey.key, false))
            pData->Value = !pData->Value;
        break;

    case TriggerMode::Hold:
        pData->Value = ImGui::IsKeyDown(pData->BindingKey.key);
        break;
    }

    constexpr const char* ItemFormat = "[{2}] {0} - {1}\n";
    switch (pData->BindingKey.show) {
    default:
        break;

    case ShowMode::None:
        break;

    case ShowMode::OnActive:
        if (pData->Value) {
            KeyBindListIndicatorString.append(std::format(ItemFormat, pData->BindingKey.KeyBindName, (pData->Value ? "ON" : "OFF"),
                ImGui::GetKeyName(pData->BindingKey.key)));
        }
        break;

    case ShowMode::Always:
        KeyBindListIndicatorString.append(std::format(ItemFormat, pData->BindingKey.KeyBindName, (pData->Value ? "ON" : "OFF"),
            ImGui::GetKeyName(pData->BindingKey.key)));
        break;
    }
}

void UpdateConfigsKeyBind(Config::Config_t* pConfigs) {
    KeyBindListIndicatorString.clear();
    for (auto item : *pConfigs) UpdateConfigKeyBind(&item.second);
}


bool Config::SaveConfigToData(const Config::Config_t* config, size_t* const outDataSize, char*** outData) {
    using namespace Config;
    size_t size = 0;

    StringBuilder<char> builder;
    {
        static const char* const formater = "{};{}={}";

        for (auto kv : *config) {
            switch (kv.second.type) {
            default:
                break;

            case ConfigBase_t::ConfigType_Bool:
                builder.AppendLine(std::format(formater, (int)kv.second.type, EncodeDataInt(kv.first),
                    EncodeDataBool(*((Base::Bool*)kv.second.pData.get()))));
                break;

            case ConfigBase_t::ConfigType_Int:
                builder.AppendLine(std::format(formater, (int)kv.second.type, EncodeDataInt(kv.first),
                    EncodeDataInt(*((Base::Int*)kv.second.pData.get()))));
                break;

            case ConfigBase_t::ConfigType_Double:
                builder.AppendLine(std::format(formater, (int)kv.second.type, EncodeDataInt(kv.first),
                    EncodeDataDouble(*((Base::Double*)kv.second.pData.get()))));
                break;

            case ConfigBase_t::ConfigType_BoolWithColor:
                builder.AppendLine(std::format(formater, (int)kv.second.type, EncodeDataInt(kv.first),
                    EncodeDataBoolWithColor(*((BoolWithColor*)kv.second.pData.get()))));
                break;

            case ConfigBase_t::ConfigType_IntWithColor:
                builder.AppendLine(std::format(formater, (int)kv.second.type, EncodeDataInt(kv.first),
                    EncodeDataIntWithColor(*((IntWithColor*)kv.second.pData.get()))));
                break;

            case ConfigBase_t::ConfigType_FloatWithColor:
                builder.AppendLine(std::format(formater, (int)kv.second.type, EncodeDataInt(kv.first),
                    EncodeDataFloatWithColor(*((FloatWithColor*)kv.second.pData.get()))));
                break;
            }

            continue;
        };
    }

    auto string = builder.getString();
    size_t length = string.length() + 1;
    char* cstring = new char[length];
    strcpy_s(cstring, length, string.c_str());
    size = length;

    *outDataSize = size;
    *outData = &cstring;
    CLogger::Log3("[Config] Saving Config To Data...\nData:\n{}", cstring);
    return true;
}

bool Config::SaveConfigToFile(const Config::Config_t* config, const char* path) {
    if (!std::filesystem::exists(CustomConfig::ExConfigPath))
        if (!std::filesystem::create_directory(CustomConfig::ExConfigPath)) CLogger::Log3("[Config] [Init] Cannot create config dir");

    std::fstream file;
    file.open(path, std::ios::out);

    if (!file.is_open()) {
        return false;
    }

    size_t size = 0;
    char** data;
    if (!SaveConfigToData(config, &size, &data)) {
        return false;
    }

    file.write(*data, size);
    file.flush();
    file.close();

    // TODO: Memory Leak
    // delete[] *data;

    if (!file.good()) {
        return false;
    }

    return true;
}

bool Config::LoadConfigFromData(Config_t* const config, const char* const data, size_t dataSize) {
    CLogger::Log3("[Config] Resolving Config From Data...\nData:\n{}", data);
    /* std::string data_str(data);
    int c = 0;
    size_t i = 0;
    size_t _i = 0;
    while ((i = data_str.find(';', c)) != std::string::npos) {
        try {
            c++;
            auto line = data_str.substr(_i, i);
            //line = line.substr(line.rfind('\n')-1,1);

           _i += i+1;

           size_t __i = line.find('=');
           if (__i == std::string::npos) continue;

           auto key = line.substr(0, __i);
           auto value = line.substr(__i + 1);

           if (strcmp(key.c_str(), "m_EnableESP")) {
               config->m_EnableESP = DecodeDataBool("value");
               continue;
           }
       } catch (const std::out_of_range& e) {
           CLogger::Log3("Error: {}", e.what());
       }
   }*/
    std::istringstream iss(data);
    std::string line;
    while (std::getline(iss, line)) {
        size_t posT = line.find_first_of(';');

        if (posT != std::string::npos) {
            Config::ConfigBase_t::ConfigType_ t = (Config::ConfigBase_t::ConfigType_)std::stoi(line.substr(0, posT));
            line = line.substr(posT + 1);

            size_t posKV = line.find_first_of('=');
            if (posKV != std::string::npos) {
                std::string k = line.substr(0, posKV);
                std::string v = line.substr(posKV + 1);
                uint32_t kk = std::stoul(k);

                switch (t) {
                case Config::ConfigBase_t::ConfigType_Bool:
                    /*if ((*config)[kk].type == t && (*config)[kk].pData.get())
                        (*(Base::Bool*)(*config)[kk].pData.get()) = DecodeDataBool(v);
                    else*/
                    (*config)[kk] = { Config::ConfigBase_t::ConfigType_Bool,
                                     ConfigBase_t::unique_voidptr(new Base::Bool(DecodeDataBool(v))) };
                    break;

                case Config::ConfigBase_t::ConfigType_Int:
                    (*config)[kk] = { Config::ConfigBase_t::ConfigType_Int,
                                     ConfigBase_t::unique_voidptr(new Base::Int(DecodeDataInt(v))) };
                    break;

                case Config::ConfigBase_t::ConfigType_Double:
                    (*config)[kk] = { Config::ConfigBase_t::ConfigType_Double,
                                     ConfigBase_t::unique_voidptr(new Base::Double(DecodeDataDouble(v))) };
                    break;

                case Config::ConfigBase_t::ConfigType_BoolWithColor:
                    (*config)[kk] = { Config::ConfigBase_t::ConfigType_BoolWithColor,
                                     ConfigBase_t::unique_voidptr(new BoolWithColor(DecodeDataBoolWithColor(v))) };
                    break;

                case Config::ConfigBase_t::ConfigType_IntWithColor:
                    (*config)[kk] = { Config::ConfigBase_t::ConfigType_IntWithColor,
                                     ConfigBase_t::unique_voidptr(new IntWithColor(DecodeDataIntWithColor(v))) };
                    break;

                case Config::ConfigBase_t::ConfigType_FloatWithColor:
                    (*config)[kk] = { Config::ConfigBase_t::ConfigType_FloatWithColor,
                                     ConfigBase_t::unique_voidptr(new FloatWithColor(DecodeDataFloatWithColor(v))) };
                    break;

                default:
                    break;
                }
            }
        }
    }

    CLogger::Log3("[Config] Config Read!");
    return true;
}

bool Config::LoadConfigFromFile(Config_t* const config, const char* path) {
    CLogger::Log3("[Config] Loading Config From File... File Path: {}", path);

    std::fstream file;
    file.open(path, std::ios::in);

    if (!file.is_open()) {
        return false;
    }

    std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    file.close();

    if (!file.good()) {
        return false;
    }

    size_t size = content.length();
    const char* data = content.c_str();
    if (!LoadConfigFromData(config, data, size)) {
        return false;
    }

    return true;
}

std::string Config::EncodeDataKeyBind(const Base::KeyBind& v) {
    return std::format("Name:{},Show:{},Mode:{},Key:{},", v.KeyBindName, EncodeDataInt(static_cast<int>(v.show)), EncodeDataInt(static_cast<int>(v.mode)),
        EncodeDataInt(v.key));
}

template <typename T>
Config::CustomConfigTypes::Base::KeyBind Config::DecodeDataKeyBind(const T& data) {
    Config::CustomConfigTypes::Base::KeyBind result;

    auto vars = splitString(data, ',');
    for (auto var : vars) {
        auto varParts = splitString(var, ':');
        if (varParts.size() < 2) continue;

        if (varParts[0] == "Name") {
            strcpy_s(result.KeyBindName, varParts[1].c_str());
        }
        else if (varParts[0] == "Show") {
            result.show = (Config::CustomConfigTypes::Base::KeyBind::ShowMode)(*DecodeDataInt(varParts[1]));
        }
        else if (varParts[0] == "Mode") {
            result.mode = (Config::CustomConfigTypes::Base::KeyBind::TriggerMode)(*DecodeDataInt(varParts[1]));
        }
        else if (varParts[0] == "Key") {
            result.key = (ImGuiKey)(*DecodeDataInt(varParts[1]));
        }
    }
    return result;
}

std::string Config::EncodeDataBool(const Config::CustomConfigTypes::Base::Bool& v) {
    return std::format("Value={};KeyBind={};", v ? "1" : "0", EncodeDataKeyBind(v.BindingKey));
}

template <typename T>
Config::CustomConfigTypes::Base::Bool Config::DecodeDataBool(const T& data) {
    Config::CustomConfigTypes::Base::Bool result;

    auto vars = splitString(data, ';');
    for (auto var : vars) {
        auto varParts = splitString(var, '=');
        if (varParts.size() < 2) continue;

        if (varParts[0] == "Value") {
            result.Value = (varParts[1] != "0");
        }
        else if (varParts[0] == "KeyBind") {
            result.BindingKey = DecodeDataKeyBind(varParts[1]);
        }
    }
    return result;
}

std::string Config::EncodeDataBoolWithColor(const BoolWithColor& v) {
    return std::format("Value={};Color={};", (v.Value ? "1" : "0"), ImGui::ColorConvertFloat4ToU32(v.Color));
}

template <typename T>
Config::CustomConfigTypes::BoolWithColor Config::DecodeDataBoolWithColor(const T& data) {
    BoolWithColor result;

    auto vars = splitString(data, ';');
    for (auto var : vars) {
        auto varParts = splitString(var, '=');
        if (varParts.size() < 2) continue;

        if (varParts[0] == "Value") {
            result.Value = std::stoi(varParts[1]) == 1;
        }
        else if (varParts[0] == "Color") {
            ImU32 colorValue = std::stoul(varParts[1]);
            result.Color = ImGui::ColorConvertU32ToFloat4(colorValue);
        }
    }
    return result;
}

std::string Config::EncodeDataInt(const Base::Int& v) { return std::format("{}", static_cast<decltype(Base::Int::Value)>(v)); }
template <typename T>
Base::Int Config::DecodeDataInt(const T& data) {
    std::istringstream iss(data);
    int v;
    iss >> std::dec >> v;
    return v;
}

std::string Config::EncodeDataDouble(const Base::Double& v) { return std::format("{}", static_cast<decltype(Base::Double::Value)>(v)); }
template <typename T>
Base::Double Config::DecodeDataDouble(const T& data) {
    return std::stod(data);
}

std::string Config::EncodeDataFloat(const Base::Float& v) { return std::format("{}", static_cast<decltype(Base::Float::Value)>(v)); }
template <typename T>
Base::Float Config::DecodeDataFloat(const T& data) {
    return std::stof(data);
}

std::string Config::EncodeDataIntWithColor(const IntWithColor& v) {
    return std::format("Value={};Color={};", EncodeDataInt(v.Value), ImGui::ColorConvertFloat4ToU32(v.Color));
}
template <typename T>
Config::CustomConfigTypes::IntWithColor Config::DecodeDataIntWithColor(const T& data) {
    IntWithColor result;

    auto vars = splitString(data, ';');
    for (auto var : vars) {
        auto varParts = splitString(var, '=');
        if (varParts.size() < 2) continue;

        if (varParts[0] == "Value") {
            result.Value = DecodeDataInt(varParts[1]);
        }
        else if (varParts[0] == "Color") {
            ImU32 colorValue = std::stoul(varParts[1]);
            result.Color = ImGui::ColorConvertU32ToFloat4(colorValue);
        }
    }
    return result;
}

std::string Config::EncodeDataFloatWithColor(const FloatWithColor& v) {
    return std::format("Value={};Color={};", EncodeDataFloat(v.Value), ImGui::ColorConvertFloat4ToU32(v.Color));
}
template <typename T>
Config::CustomConfigTypes::FloatWithColor Config::DecodeDataFloatWithColor(const T& data) {
    FloatWithColor result;

    auto vars = splitString(data, ';');
    for (auto var : vars) {
        auto varParts = splitString(var, '=');
        if (varParts.size() < 2) continue;

        if (varParts[0] == "Value") {
            result.Value = DecodeDataFloat(varParts[1]);
        }
        else if (varParts[0] == "Color") {
            ImU32 colorValue = std::stoul(varParts[1]);
            result.Color = ImGui::ColorConvertU32ToFloat4(colorValue);
        }
    }
    return result;
}

std::vector<std::string> Config::splitString(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}
