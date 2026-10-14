#pragma once
// AUTO-GENERATED host-test stub of the mod globals (see shadowmodz_main.cpp)

#include <string>
#include <vector>
#include <mutex>

struct HostEsp {
    bool Line = false;
    bool Box = false;
    bool Box3d = false;
    bool BoxFilled = false;
    bool BoxHalfFilled = false;
    bool Health = false;
    bool MAFIA = false;
    bool Distance = false;
};

struct HostCfg { bool aimbot = false; HostEsp ESP; };
inline HostCfg Config;

struct HostMass { bool Sucks = false; float Sucks1 = 3.0f; float FrontOffset = 1.0f; };
inline HostMass Mass;

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
inline float AimSpeed = 0.0f;
inline float AimKillFov = 0.0f;
inline float AimKillDistance = 0.0f;
inline float AutoKillDelayMs = 0.0f;
inline float AimLockFov = 0.0f;
inline float AimLockSpeed = 0.0f;
inline float FireRateMs = 0.0f;
inline float ShotsPerBurst = 0.0f;
inline float SpinBotSpeed = 0.0f;
inline float LineThickness = 0.0f;
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

inline int g_AimHeightIndex = 4;   // matches the real project global

struct FW_Stats { int totalChecks=0,totalBlocked=0,totalAllowed=0,anogsBlocked=0,domainBlocked=0,portBlocked=0,privIpBlocked=0,libsHooked=0; bool hooksOk=true; };
inline FW_Stats g_FwStats;
struct FW_LogEntry { std::string time, libName, target, port, reason; bool isBlocked=false; };
inline std::vector<FW_LogEntry> g_fwLogs;
inline std::mutex g_fwLogMutex;
inline const int FW_MAX_LOGS = 200;
inline const char* FW_ALLOWED_DOMAIN = "lixuim.net";
inline char g_fwAllowedIpCache[128] = "10.2.0.7";
