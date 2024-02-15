#pragma once
#include <imgui/imgui.h>

// Config system based on ImGui
namespace Config {
    namespace CustomConfigTypes {
        namespace Base {
            struct KeyBind {
                /*
                 * Memory data, dont save to config
                 */

                 /*
                  * Memory data, dont save to config
                  */

                enum class TriggerMode { Disabled, Toggle, Hold };
                enum class ShowMode { None, OnActive, Always };

                //char KeyBindFormat[100 + sizeof("")];
                char KeyBindName[100 + sizeof("")] = "";
                TriggerMode mode = TriggerMode::Disabled;
                ImGuiKey key = ImGuiKey_None;
                ShowMode show = ShowMode::None;
            };

            template <typename ValueT>
            struct BaseT {
                ValueT Value{};

                BaseT() {}
                BaseT(ValueT v) : BaseT() { Value = v; }

                virtual operator ValueT& () { return Value; }
                virtual operator ValueT* () { return &Value; }
                virtual ValueT& operator*() { return Value; }
                virtual ValueT* operator&() { return &Value; }
                virtual ValueT* operator->() { return &Value; }
                // virtual ValueT& operator.() { return Value; }

                virtual operator const ValueT& () const { return Value; }
                virtual operator const ValueT* () const { return &Value; }
                virtual const ValueT& operator*() const { return Value; }
                virtual const ValueT* operator&() const { return &Value; }
                virtual const ValueT* operator->() const { return &Value; }
            };

            struct Bool : BaseT<bool> {
                KeyBind BindingKey{};

                Bool() {}
                Bool(bool v) : BaseT<bool>(v) {}

                Bool(decltype(Value) v, KeyBind k) : Bool(v) { BindingKey = k; }
            };

            struct Int : BaseT<int> {
                Int() {}
                Int(int v) : BaseT<int>(v) {}
            };

            struct Double : BaseT<double> {
                Double() {}
                Double(double v) : BaseT<double>(v) {}
            };

            struct Float : BaseT<float> {
                Float() {}
                Float(float v) : BaseT<float>(v) {}
            };

            struct ColorRGBA : BaseT<ImVec4> {
                ColorRGBA() {}
                ColorRGBA(ImVec4 v) : BaseT<ImVec4>(v) {}
            };
        }  // namespace Base

        struct BoolWithColor : Base::Bool {
            Base::ColorRGBA Color{};

            BoolWithColor() : Base::Bool() {}
            BoolWithColor(bool v, const decltype(decltype(Color)::Value)& c) : Base::Bool{ v }, Color(c) {}
            BoolWithColor(const Base::Bool& b, const decltype(Color)& c) : Base::Bool(b), Color(c) {}

            virtual operator decltype(Base::ColorRGBA::Value)& () { return Color; }
            virtual operator decltype(Base::ColorRGBA::Value)* () { return &Color; }
        };
        struct IntWithColor : Base::Int {
            Base::ColorRGBA Color{};

            IntWithColor() : Base::Int() {}
            IntWithColor(int v, const decltype(decltype(Color)::Value)& c) : Base::Int{ v }, Color(c) {}
            IntWithColor(const Base::Int& b, const decltype(Color)& c) : Base::Int(b), Color(c) {}

            virtual operator decltype(Base::ColorRGBA::Value)& () { return Color; }
            virtual operator decltype(Base::ColorRGBA::Value)* () { return &Color; }
        };
        struct FloatWithColor : Base::Float {
            Base::ColorRGBA Color{};

            FloatWithColor() : Base::Float() {}
            FloatWithColor(float v, const decltype(decltype(Color)::Value)& c) : Base::Float{ v }, Color(c) {}
            FloatWithColor(const Base::Float& b, const decltype(Color)& c) : Base::Float(b), Color(c) {}

            virtual operator decltype(Base::ColorRGBA::Value)& () { return Color; }
            virtual operator decltype(Base::ColorRGBA::Value)* () { return &Color; }
        };

        inline const char* KeyBindTriggerModeTypes[]{ "Disabled", "Toggle", "Hold" };
        inline const char* KeyBindShowModeTypes[]{ "None", "OnAcitve", "Always" };
        inline const char* ColorTypes[]{ "Disabled", "Solid", "LinearGradient" };
        inline const char* AimBotEnableTypes[]{ "Disabled", "Legit", "Rage" };
        inline const char* OutOfViewArrorTypes[]{ "Disabled", "Arrow" };
        inline const char* MenuThemeTypes[]{ "Disabled", "Solid", "RGB" };
        inline const char* AntiAimDirection[]{ "None", "Target" };
        inline const char* AntiAimPitch[]{ "None", "Up", "Straight", "Down" };
        inline const char* AntiAimYaw[]{ "None", "Left", "Backwards", "Right" };
    }  // namespace CustomConfigTypes

    namespace CustomConfig {
        inline const char ExConfigPath[] = "TripIt.cc/config";
        inline const char ExConfigPath_WinFormat[] = "TripIt.cc\\config";
        inline const char ExConfigName[] = ".cfg";

    }  // namespace CustomConfig

    using namespace CustomConfigTypes;
    struct ConfigBase_t {
        enum ConfigType_ {
            ConfigType_Bool,    // Saveable & Readable
            ConfigType_Int,     // Saveable & Readable
            ConfigType_Double,  // Saveable & Readable
            ConfigType_Float,
            ConfigType_Color,
            ConfigType_BoolWithColor,  // Saveable & Readable
            ConfigType_IntWithColor,   // Saveable & Readable
            ConfigType_FloatWithColor  // Saveable & Readable
        } type;

        class unique_voidptr {
            void* ptr;

        public:
            unique_voidptr() { ptr = nullptr; }
            unique_voidptr(void* p) { ptr = p; }
            ~unique_voidptr() {
                // TODO: Memory Leak
                // delete ptr;
            }

            void* get() { return ptr; }
        };
        unique_voidptr pData;

        ConfigBase_t() {
            type = ConfigType_Bool;
            pData = unique_voidptr();
        }
        ConfigBase_t(ConfigType_ t, unique_voidptr p) {
            type = t;
            pData = p;
        }
    };
    typedef std::map<uint32_t, ConfigBase_t> Config_t;
    typedef std::pair<uint32_t, ConfigBase_t> ConfigItem_t;

    inline extern bool SaveConfigToData(const Config_t* config, size_t* const outDataSize, char*** outData);
    inline extern bool SaveConfigToFile(const Config::Config_t* config, const char* path);

    inline extern bool LoadConfigFromData(Config_t* const config, const char* const data, size_t dataSize);
    inline extern bool LoadConfigFromFile(Config_t* const config, const char* path);

    inline extern std::string EncodeDataKeyBind(const Base::KeyBind&);
    template <typename T>
    inline extern Base::KeyBind DecodeDataKeyBind(const T&);
    inline std::string EncodeDataInt(const Base::Int&);
    template <typename T>
    inline extern Base::Int DecodeDataInt(const T&);
    inline extern std::string EncodeDataDouble(const Base::Double&);
    template <typename T>
    inline extern Base::Double DecodeDataDouble(const T&);
    inline extern std::string EncodeDataFloat(const Base::Float&);
    template <typename T>
    inline extern Base::Float DecodeDataFloat(const T&);
    inline extern std::string EncodeDataBool(const Base::Bool&);
    template <typename T>
    inline extern Base::Bool DecodeDataBool(const T&);
    inline extern std::string EncodeDataBoolWithColor(const BoolWithColor&);
    template <typename T>
    inline extern BoolWithColor DecodeDataBoolWithColor(const T&);
    inline extern std::string EncodeDataIntWithColor(const IntWithColor&);
    template <typename T>
    inline extern IntWithColor DecodeDataIntWithColor(const T&);
    inline extern std::string EncodeDataFloatWithColor(const FloatWithColor&);
    template <typename T>
    inline extern FloatWithColor DecodeDataFloatWithColor(const T&);

    inline static std::vector<std::string> splitString(const std::string& str, char delimiter);
}  // namespace Config

using namespace Config;
enum mnames {
    // ESP
    m_ESP_Enable = 0x100,
    m_ESP_Use3DBoxes,

    m_ESP_OtherBoxes = 0x110,
    m_ESP_HostageBoxes,
    m_ESP_WeaponBoxes,
    m_ESP_ChickenBoxes,

    m_ESP_EnablePlayerLocalESP = 0x120,
    m_ESP_PlayerAimLineLocal,
    m_ESP_PlayerGlowLocal,
    m_ESP_PlayerBoxesLocal,
    m_ESP_PlayerNamesLocal,
    m_ESP_PlayerArmorBarLocal,
    m_ESP_PlayerHealthBarLocal,
    m_ESP_PlayerOutOfViewArrowLocal,
    m_ESP_PlayerStatusIndicatorLocal,

    m_ESP_EnablePlayerEnemyESP = 0x130,
    m_ESP_PlayerAimLineEnemy,
    m_ESP_PlayerGlowEnemy,
    m_ESP_PlayerBoxesEnemy,
    m_ESP_PlayerNamesEnemy,
    m_ESP_PlayerArmorBarEnemy,
    m_ESP_PlayerHealthBarEnemy,
    m_ESP_PlayerOutOfViewArrowEnemy,
    m_ESP_PlayerStatusIndicatorEnemy,

    m_ESP_EnablePlayerTeammateESP = 0x140,
    m_ESP_PlayerAimLineTeammate,
    m_ESP_PlayerGlowTeammate,
    m_ESP_PlayerBoxesTeammate,
    m_ESP_PlayerNamesTeammate,
    m_ESP_PlayerArmorBarTeammate,
    m_ESP_PlayerHealthBarTeammate,
    m_ESP_PlayerOutOfViewArrowTeammate,
    m_ESP_PlayerStatusIndicatorTeammate,

    // Player
    m_Player_FastStop = 0x200,
    m_Player_BunnyHop,

    // World
    m_World_HitSound = 0x300,
    m_World_KillSound,
    m_World_RadarHack,
    m_World_ShootSoundsOverride,

    // Visuals
    m_World_Visuals_NoFlash = 0x400,

    // Misc
    m_Misc_FPS_Limiter = 0x500,
    m_Misc_FPS_Limiter_Value,
    m_Misc_BackgroundColor,
    m_Misc_KeyBindLists,
    m_Misc_KeyBindLists_AutoSize,
    m_Misc_MenuTheme,
    m_Misc_WaterMark,

    // Rage
    m_Rage_AimBot_Enable = 0x600,
    m_Rage_AimBot_TargetByDistanceOnly,
    m_Rage_AimBot_AutoShoot,
    m_Rage_AimBot_FriendlyFire,
    m_Rage_AimBot_ManualAim,

    // Legit
    m_Rage_AntiAim_Enable = 0x700,
    m_Rage_AntiAim_Pitch,
    m_Rage_AntiAim_Yaw,
    m_Rage_AntiAim_PitchOffset,
    m_Rage_AntiAim_YawOffset,
    m_Rage_AntiAim_Direction,

    // Others
    m_FastRender = 0x5000
};

inline Base::Bool gDEBUG_bAimbotVisual;
inline Base::Int gDEBUG_nAimSorter;
/*  Resolver Data
 * Offsets with Vector type:
 *   x for forward(+)/backward(-) offset;
 *   y for left(-)/right(+) offset;
 *   z for up(+)/down(-) offset;
 */
#include <utils/math/types/vector.hpp>
inline Vector gVecResolverOffset = { 6.5f, 0.f, 0.f };

extern Config::Config_t* NewDefaultConfig();

inline Config::Config_t* g_Config = NewDefaultConfig();

// Used by render loop
inline std::string KeyBindListIndicatorString = "";
extern void UpdateConfigsKeyBind(Config::Config_t* pConfigs);
extern void UpdateConfigKeyBind(Config::ConfigBase_t* pConfigItem);
