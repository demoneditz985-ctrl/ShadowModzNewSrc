#pragma once
// AUTO-GENERATED host stub of the game globals used by ShadowMenu.hpp.
// The X-macro field lists in SHADOWMODZ MODS/ShadowMenu.hpp are the source
// of truth, so this stub and the real ShadowBindMenu() stay in sync.
// Regenerate with: python3 tools/uipreview/generate_stub.py

#include "imgui.h"

// ---- nested feature configs (Config.ESP.*, Mass.*, ...) ----
struct Struct_Config
{
struct Struct_Config_ESP
{
    bool Line = false;
    bool Box = false;
    bool Box3d = false;
    bool BoxFilled = false;
    bool BoxHalfFilled = false;
    bool Health = false;
    bool MAFIA = false;
    bool Distance = false;
};
    Struct_Config_ESP ESP;
};
inline Struct_Config Config;

struct Struct_Mass
{
    bool Sucks = false;
    float Sucks1 = 0.0f;
    float FrontOffset = 0.0f;
};
inline Struct_Mass Mass;

// ---- plain globals ----
inline bool Enable = false;
inline bool Aimbot = false;
inline bool SilentAim = false;
inline bool AimSilent = false;
inline bool Headshot = false;
inline bool AimVisible2 = false;
inline bool AimKill1 = false;
inline bool AutoKill = false;
inline bool AimLock = false;
inline bool AimLockThroughWall = false;
inline bool CustomFireRate = false;
inline bool SpinBot = false;
inline bool aimVisibleEnabled = false;
inline bool ANTIDEV = false;
inline bool EspSkeleton2 = false;
inline bool DrawFov = false;
inline bool SniperAlert = false;
inline bool ESPGrenade = false;
inline bool EnableNightMode = false;
inline bool SpeedHack = false;
inline bool speedrun = false;
inline bool Fastswitch = false;
inline bool AutoRun = false;
inline bool InfiniteRange = false;
inline bool norecoil = false;
inline bool SniperFast = false;
inline bool PcSwapAnim = false;
inline bool Setindia = false;
inline bool camxa = false;
inline bool Reset = false;
inline bool FreeMove = false;
inline bool StopFireAnim = false;
inline bool isParachuting = false;
inline bool MedkitMoving = false;
inline bool FastSensivity = false;
inline bool QuickReloadLakiyaxMods = false;
inline bool ShurikenKill = false;
inline bool FUCKMODI = false;
inline bool LUNDUP = false;
inline bool autoReviveEnabled = false;
inline bool gliderHackEnabled = false;
inline bool gliderInfiniteEnabled = false;
inline bool SpoofNameEnabled = false;
inline bool g_LoginBypass400 = false;
inline bool g_FirewallEnabled = false;
inline bool g_FirewallVerbose = false;
inline bool RenderWatermark = false;
inline float Fov_Aim = 0.0f;
inline float Aimdis = 0.0f;
inline float AimSpeed = 0.50f;
inline float AimKillFov = 0.0f;
inline float AimKillDistance = 0.0f;
inline float AutoKillDelayMs = 0.0f;
inline float AimLockFov = 0.0f;
inline float AimLockSpeed = 0.0f;
inline float FireRateMs = 0.0f;
inline float ShotsPerBurst = 0.0f;
inline float SpinBotSpeed = 0.0f;
inline float LineThickness = 1.5f;
inline float BoxThickness = 0.0f;
inline float BoxRoundness = 0.0f;
inline float active = 0.0f;
inline float Sensivity = 0.0f;
inline float FUCKMODIRate = 0.0f;
inline float AiFov = 0.0f;
inline int AimWhen = 0;
inline int LineMode = 0;
inline int BoxStyle = 0;
inline int DONTCarValue = 0;
inline int g_AimHeightIndex = 4;

// ---- harness extras ----
inline const char* g_urlOpened = nullptr;   // records hooks.openURL()

namespace ShadowUI
{
inline bool CreateLogoTexture() { return false; }   // no GL context on the host
inline void ReleaseLogoTexture() {}
}
