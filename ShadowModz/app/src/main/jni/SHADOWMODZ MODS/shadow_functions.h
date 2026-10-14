#pragma once
#ifndef SHADOWMODZ_H
#define SHADOWMODZ_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <Il2Cpp.h>
#include <Tools.h>
#include <Vector3.hpp>
#include <Color.h>
#include <math.h>
#include <string>
#include <vector>
#include <mutex>
#include <atomic>
#include <cstring>
#include <cstdint>
#include <chrono>
#include <thread>
#include <sched.h>
#include <ctime>

#include <SHADOWMODZ MODS/main.h>
#include "obfuscate.h"
#include "Unity/unity.h"
#include "Class.h"
#include "Rect.h"
#include "Bools.h"

inline static int g_GlHeight = 0, g_GlWidth = 0;
typedef char PACKAGENAME;

struct sColor {
    float line[4] = { 0.0f, 1.0f, 0.5f, 1.0f };
    float box[4]  = { 1.0f, 0.0f, 0.3f, 1.0f };
};
inline sColor Color;

template <typename T>
struct UnityArray {
    void* klass;
    void* monitor;
    void* bounds;
    int max_length;
    T vector[64];
};

struct Vector2 {
    float x, y;
};

struct DictionaryEntry {
    int hashCode;
    int next;
    void* key;
    void* value;
};

struct MyDictionary {
    void* klass;
    void* monitor;
    void* buckets;
    UnityArray<DictionaryEntry>* entries;
    int count;
};

extern bool Enable;
extern bool Aimbot;
extern int  AimWhen;
extern float Fov_Aim;
extern float Aimdis;

extern bool SpeedHack;

extern bool AimHead35, AimHead50, AimHead75, AimHead90, AimHead100;
extern int  aimPosition;
extern float NECK_OFFSET;
extern void* enemyTarget;
extern void* LockedEnemy;
extern float LockedEnemyTime;
extern float lastFireTime;
extern float lastAutoKillTime;
//extern ImU32 die, alive;

extern bool EnableSmartAimbot;
extern bool menu1, menu2, menu3, menu4, showWatermark;
extern bool rPritam, TPBR, Pritamaim, PritamAimbot, Reset, Guest, camxa;
extern bool SniperFast, PcSwapAnim, Setindia, Fastswitch, AutoRun, InfiniteRange;
extern bool speedrun, isSpeed, Firewall, health, Headshot, AimNewF;
extern bool ESPLine, ESPBox, EspBox3d, SniperAlert, SelectAim, AimBody, showAlert;
extern bool DrawFov, SilentAim, AimVisible2, ActiveFeature, ShubhHuRe;
extern bool Garena_ki_mkc, ESPGrenade, AimEnable, AimWhen1, AimWhen2, AimWhen3;
extern bool AimSilent, camcao, box, showbox1, showline1, showline2, showline;
extern bool norecoil, isParachuting, MedkitMoving, FreeMove, StopFireAnim;
extern bool FastSensivity, ShurikenKill, AutoKill, AimKill1;
extern bool AimKillIgnoreVisible, AimKillHeadOnly, AimLock, AimLockThroughWall;
extern bool CustomFireRate, SpinBot, autoReviveEnabled;
extern bool gliderHackEnabled, gliderInfiniteEnabled;
extern bool aimVisibleEnabled, QuickReloadLakiyaxMods, LUNDUP, FUCKMODI;
extern bool EnableNightMode;
extern bool EspSkeleton2;
extern bool ANTIDEV;
extern bool RenderWatermark;

extern float AutoKillDelayMs, AimKillFov, AimKillDistance;
extern float AimLockFov, AimLockSpeed, FireRateMs, ShotsPerBurst;
extern float SpinBotSpeed, aimVisibleFov, aimVisibleMaxDist;
extern float FUCKMODIRate, Sensivity, AiFov;
extern int  SetDamage, LineMode, BoxStyle, FovStyle, HealthBoxStyle;
extern int  DONTCarValue, damageCooldown, delayCounter, delayLimit, delayLimit2, DamageDelay;
extern float LineThickness, BoxThickness, BoxRoundness, RGBSpeed;
extern float AimSpeed, visual_esp_box, visual_esp_boxth;
extern float AimFov2, DistanceTele;
extern bool  aimVisibleIgnoreKnocked;
extern ImVec4 ThemeColer;
extern void* mainCamera;
extern bool  Startgs;

extern int Fov_Aim_Old;
extern float Aimdis_Old;

struct MassKill {
    float Sucks1 = 9.0f;
    bool Sucks = false;
    float FrontOffset = 0.5f;
};
extern MassKill Mass;

inline bool isOutsideScreen(ImVec2 pos, ImVec2 screen) {
    return (pos.y < 0 || pos.x > screen.x || pos.y > screen.y || pos.x < 0);
}

inline ImVec2 pushToScreenBorder(ImVec2 Pos, ImVec2 screen, int offset) {
    int x = (int) Pos.x;
    int y = (int) Pos.y;
    if (Pos.y < 0) y = -offset;
    if (Pos.x > screen.x) x = (int) screen.x + offset;
    if (Pos.y > screen.y) y = (int) screen.y + offset;
    if (Pos.x < 0) x = -offset;
    return ImVec2(x, y);
}

inline static float tm_esp = 127 / 255.f;

inline void DrawCircleHealth(ImVec2 position, int health, int max_health, float radius) {
    if (max_health <= 0) return;
    float a_max = (3.14159265359f * 2.0f);
    ImU32 healthColor = IM_COL32(0, 255, 128, 255);
    if (health <= (max_health * 0.6)) healthColor = IM_COL32(255, 160, 0, 255);
    if (health < (max_health * 0.3))  healthColor = IM_COL32(255, 0, 50, 255);
    ImGui::GetForegroundDrawList()->PathArcTo(position, radius,
        (-(a_max / 4.0f)) + (a_max / max_health) * (max_health - health),
        a_max - (a_max / 4.0f));
    ImGui::GetForegroundDrawList()->PathStroke(healthColor, ImDrawFlags_None, 3.5f);
}

inline void drawText2(int x, int y, long color, const char *str, float size) {
    if (!str) return;
    ImGui::GetBackgroundDrawList()->AddText(NULL, size, ImVec2(x, y), color, str);
}

inline void DrawRectFilled(int x, int y, int w, int h, ImColor color) {
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
}

inline void DrawRectFilledHealth(int x, int y, int w, int h, ImVec4 color) {
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h),
        ImGui::ColorConvertFloat4ToU32(color), 0, 0);
}

inline void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color, int T) {
    ImGui::GetBackgroundDrawList()->AddTriangleFilled(
        ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), color);
}

inline void DrawHealths(Rect box, float entityHealth, float maxHealth, int TeamID,
                        const char* name, long color, float distance) {
    float x = box.x - (140 - box.w) / 2;
    float y = box.y;
    char dis[32];
    sprintf(dis, "%.0fM", distance);
    char TeamText[20];
    sprintf(TeamText, "%d", (int)TeamID);
    ImVec4 HPColor = ImVec4{0.0f, 1.0f, 0.5f, 0.8f};
    if (maxHealth > 0 && entityHealth < maxHealth) {
        float hp = entityHealth / maxHealth;
        if (hp < 0.3f) HPColor = ImVec4{1.0f, 0.0f, 0.2f, 0.8f};
        else if (hp < 0.6f) HPColor = ImVec4{1.0f, 0.5f, 0.0f, 0.8f};
    }
    size_t nameLen = name ? strlen(name) : 5;
    DrawRectFilled(x - nameLen, y - 41, 120 + nameLen * 2, 20, IM_COL32(5, 5, 10, 200));
    DrawRectFilled(x - nameLen, box.y - 41, 25, 20, IM_COL32(255, 0, 100, 220));
    if (strlen(TeamText) < 2)
        drawText2(x + 6 - nameLen, box.y - 42, ImColor(255, 255, 255), TeamText, 18.943);
    else
        drawText2(x - nameLen, box.y - 42, ImColor(255, 255, 255), TeamText, 18.943);
    drawText2(x + 28 - nameLen, y - 43, ImColor(255, 255, 255), name ? name : "Target", 18.943);
    float maxWidth = 120;
    float healthBarWidth = (maxHealth > 0) ? (entityHealth * maxWidth / maxHealth) : maxWidth;
    DrawRectFilledHealth(x - maxWidth / 2, y - 18, healthBarWidth, 5, HPColor);
    drawText2(x + 125 + nameLen, y - 43, IM_COL32(0, 255, 150, 255), dis, 18.943);
    DrawTriangle(box.x + box.w / 2 - 10, y - 8, box.x + box.w / 2 + 15 - 10, y - 8,
                 box.x + box.w / 2 - 2, y, IM_COL32(255, 0, 100, 255), 1);
}

inline static float SetFieldOfView = 0, GetFieldOfView = 0;
inline void DroneView() {
    if (GetFieldOfView == 0) GetFieldOfView = get_fieldOfView();
    if (SetFieldOfView > 0 && GetFieldOfView != 0)
        set_fieldOfView((float)GetFieldOfView + SetFieldOfView);
    if (SetFieldOfView == 0 && GetFieldOfView != 0)
        set_fieldOfView((float)GetFieldOfView);
}

inline bool IsVisible(void * player) {
    if (player != NULL) {
        void *hitObj = NULL;
        void* camera = Camera_main();
        if (!camera) return false;
        Vector3 cameraLocation = Transform_GetPosition(Component_GetTransform(camera));
        void* headCollider = Player_GetHeadCollider(player);
        if (!headCollider) return false;
        Vector3 headLocation = Transform_GetPosition(Component_GetTransform(headCollider));
        return !Physics_Raycast(cameraLocation, headLocation, 12, &hitObj);
    }
    return false;
}

inline bool isVisible_Old(void *enemy) {
    if (enemy != NULL) {
        void *hitObj = NULL;
        auto Camera = Transform_GetPosition(Component_GetTransform(Camera_main()));
        auto Target = Transform_GetPosition(Component_GetTransform(Player_GetHeadCollider(enemy)));
        return !Physics_Raycast(Camera, Target, 12, &hitObj);
    }
    return false;
}

inline bool Visible_Check(void *closestEnemy) {
    void *Object = static_cast<void *>(nullptr);
    Vector3 Camera = Transform_INTERNAL_GetPosition(Component_GetTransform(Camera_main()));
    Vector3 Collider = Transform_INTERNAL_GetPosition(Component_GetTransform(Player_GetHeadCollider(closestEnemy)));
    if (!Physics_Raycast(Camera, Collider, 12, &Object)) return true;
    return false;
}

inline void *GetClosestEnemy() {
    float shortestDistance = Aimdis;
    void* closestEnemy = NULL;
    void* get_MatchGame = Curent_Match();
    if (!get_MatchGame) return NULL;
    void* LocalPlayer = GetLocalPlayer(get_MatchGame);
    if (LocalPlayer != NULL && Enable) {
        MyDictionary* dict = *(MyDictionary**)((uintptr_t)get_MatchGame + ListPlayer);
        if (dict != nullptr && dict->entries != nullptr) {
            void* camera = Camera_main();
            if (!camera) return NULL;
            Vector3 cameraLocation = Transform_GetPosition(Component_GetTransform(camera));
            int total = dict->count;
            for (int u = 0; u < total; u++) {
                void* Player = dict->entries->vector[u].value;
                if (Player != NULL && Player != LocalPlayer &&
                    !get_isLocalTeam(Player) && !get_IsDieing(Player) && get_MaxHP(Player)) {
                    Vector3 PlayerPos = GetHeadPosition(Player);
                    float distance = Vector3::Distance(cameraLocation, PlayerPos);
                    if (distance < shortestDistance) {
                        Vector3 targetDir = Vector3::Normalized(PlayerPos - cameraLocation);
                        float angle = Vector3::Angle(targetDir,
                            GetForward(Component_GetTransform(camera))) * 100.0;
                        if (angle <= Fov_Aim) {
                            shortestDistance = distance;
                            closestEnemy = Player;
                        }
                    }
                }
            }
        }
    }
    return closestEnemy;
}

inline void *GetClosestEnemy2(void *match) {
    if (!match || !ActiveFeature) return nullptr;
    float shortestDistance = 99999.0f;
    float maxAngle = AimFov2;
    void* closestEnemy = NULL;
    void* LocalPlayer = GetLocalPlayer(match);
    if (LocalPlayer != nullptr) {
        auto* players = *(MyDictionary**)((uintptr_t)match + ListPlayer);
        if (!players || !players->entries) return nullptr;
        for (int u = 0; u < players->count; u++) {
            void* Player = players->entries->vector[u].value;
            if (Player != NULL && !get_isLocalTeam(Player) && !get_IsDieing(Player) &&
                get_isVisible(Player) && get_MaxHP(Player)) {
                Vector3 PlayerPos = GetHeadPosition(Player);
                Vector3 LocalPlayerPos = GetHeadPosition(LocalPlayer);
                Vector3 targetDir = Vector3::Normalized(PlayerPos - LocalPlayerPos);
                float angle = Vector3::Angle(targetDir,
                    GetForward(Component_GetTransform(Camera_main()))) * 100.0;
                if (!get_God(Player)) {
                    if (angle <= maxAngle && angle < shortestDistance) {
                        shortestDistance = angle;
                        closestEnemy = Player;
                    }
                }
            }
        }
    }
    return closestEnemy;
}

inline void *EnemyVisible(void *match) {
    if (!match || !ActiveFeature) return nullptr;
    float shortestDistance = 99999.0f;
    float maxAngle = AimFov2;
    void* closestEnemy = NULL;
    void* LocalPlayer = GetLocalPlayer(match);
    if (LocalPlayer != nullptr) {
        auto* players = *(MyDictionary**)((uintptr_t)match + ListPlayer);
        if (!players || !players->entries) return nullptr;
        for (int u = 0; u < players->count; u++) {
            void* Player = players->entries->vector[u].value;
            if (Player != NULL && !get_isLocalTeam(Player) && !get_IsDieing(Player) &&
                get_isVisible(Player) && get_MaxHP(Player)) {
                Vector3 PlayerPos = GetHeadPosition(Player);
                Vector3 LocalPlayerPos = GetHeadPosition(LocalPlayer);
                Vector3 targetDir = Vector3::Normalized(PlayerPos - LocalPlayerPos);
                float angle = Vector3::Angle(targetDir,
                    GetForward(Component_GetTransform(Camera_main()))) * 100.0;
                if (!get_God(Player)) {
                    if (AimVisible2) {
                        if (Visible_Check(Player)) {
                            if (angle <= maxAngle && angle < shortestDistance) {
                                shortestDistance = angle;
                                closestEnemy = Player;
                            }
                        }
                    } else {
                        if (angle <= maxAngle && angle < shortestDistance) {
                            shortestDistance = angle;
                            closestEnemy = Player;
                        }
                    }
                }
            }
        }
    }
    return closestEnemy;
}

inline void Aimbot2(void *LocalPlayer, void *Player) {
    void* CurrentMatch = Curent_Match();
    if (CurrentMatch == nullptr) return;
    void* localPlayer = GetLocalPlayer(CurrentMatch);
    if (localPlayer == nullptr) return;
    void *WeaponHand = GetWeaponOnHand1(Current_Local_Player());
    void *camera = Camera_main();

    void* closestEnemy = GetClosestEnemy2(CurrentMatch);
    if (closestEnemy != nullptr) {
        void* LocalPlayerPos2 = GetAttackableCenterWS(LocalPlayer);
        (void)LocalPlayerPos2;
        Vector3 LocalPlayerPos = GetHeadPosition(LocalPlayer);
        Vector3 PlayerLocation = CameraMain(LocalPlayer);
        Vector3 PlayerHeadPos = GetHeadPosition(closestEnemy);
        Vector3 PositionRoot = WorldToScreenPoint(camera,
            Transform_INTERNAL_GetPosition(Component_GetTransform(closestEnemy)));
        Vector3 PositionHead = WorldToScreenPoint(camera,
            Transform_INTERNAL_GetPosition(Component_GetTransform(closestEnemy)) + Vector3(0, 1.9f, 0));
        float distance = Vector3::Distance(PlayerHeadPos, LocalPlayerPos);
        DistanceTele = distance;
        auto *Player2 = EnemyVisible(CurrentMatch);
        if (Player2 != nullptr && Player2 != LocalPlayer) {
            if (SetDamage == 1) SetDamage = 1;
            else if (SetDamage == 0) SetDamage = 0;
        }
    }
}

inline bool isEnemyInRangeWeapon(void *player, void *enemy, void* weapon) {
    if (player != nullptr && enemy != nullptr && weapon != nullptr) {
        Vector3 EnemyHeadPosition = GetHeadPosition(enemy);
        Vector3 PlayerHeadPosition = GetHeadPosition(player);
        float distance = Vector3::Distance(PlayerHeadPosition, EnemyHeadPosition);
        float range = get_Range(weapon);
        if (distance <= range) return true;
    }
    return false;
}

inline int (*old_BLAGCMCGEJG1)(void *, HitObjectInfo *) = nullptr;

inline int BLAGCMCGEJG1(void *ist, HitObjectInfo *HitObject) {
    if (SilentAim) {
        if (HitObject != nullptr) {
            void *current_match = Curent_Match();
            if (current_match != NULL) {
                void* local_player = GetLocalPlayer(current_match);
                if (local_player != NULL) {
                    void* WeaponHand = GetWeaponOnHand1(local_player);
                    auto *ClosestEnemy = EnemyVisible(current_match);
                    if (ClosestEnemy != nullptr) {
                        if (isEnemyInRangeWeapon(local_player, ClosestEnemy, WeaponHand)) {
                            Vector3 EnemyLocation;
                            if (SetDamage == 1) EnemyLocation = GetHeadPosition(ClosestEnemy);
                            else if (SetDamage == 0) EnemyLocation = GetHipPosition(ClosestEnemy);
                            else EnemyLocation = GetHeadPosition(ClosestEnemy);
                            Vector3 PlayerLocation = CameraMain(local_player);
                            HitObject->HitObject = get_gameObject(Player_GetHeadCollider(ClosestEnemy));
                            HitObject->HitCollider = Player_GetHeadCollider(ClosestEnemy);
                            HitObject->HitLocation = EnemyLocation;
                            HitObject->HitNormal = EnemyLocation;
                            HitObject->RayDir = Vector3::Normalized(EnemyLocation - PlayerLocation);
                            HitObject->StartPosition = PlayerLocation;
                            HitObject->OrigStartPosition = PlayerLocation;
                            HitObject->SpecialHitType = 0;
                            HitObject->HitGroup = 1;
                            HitObject->IgnoreHappens = false;
                            HitObject->ViewBlocked = false;
                        }
                    }
                }
            }
        }
    }
    if (old_BLAGCMCGEJG1) return old_BLAGCMCGEJG1(ist, HitObject);
    return 0;
}

inline int (*orig_PlayerNetwork_TakeDamage)(
    void *ClosestEnemy, int baseDamage, PlayerID damager, DamageInfo2_o *damageInfo,
    int weaponDataID, Vector3 firePos, Vector3 hitPos,
    monoList<float> checkParams, void *damagerWeaponDynamicInfo, int damagerVehicleID) = nullptr;

inline int hook_PlayerNetwork_TakeDamage(
    void *ClosestEnemy, int baseDamage, PlayerID damager, DamageInfo2_o *damageInfo,
    int weaponDataID, Vector3 firePos, Vector3 hitPos,
    monoList<float> checkParams, void *damagerWeaponDynamicInfo, int damagerVehicleID) {

    void* CurrentMatch = Curent_Match();
    void* LocalPlayer = GetLocalPlayer(CurrentMatch);
    firePos = GetHeadPosition(LocalPlayer);
    hitPos  = GetHeadPosition(ClosestEnemy);
    if (ClosestEnemy != NULL && Headshot) {
        if (damageInfo != NULL) {
            *(int *)((long) damageInfo + offset_BodyPart) = 1;
        }
    }
    if (orig_PlayerNetwork_TakeDamage)
        return orig_PlayerNetwork_TakeDamage(ClosestEnemy, baseDamage, damager, damageInfo,
            weaponDataID, firePos, hitPos, checkParams, damagerWeaponDynamicInfo, damagerVehicleID);
    return 0;
}

inline void (*orig_mrSilentAim)(void *, float, float) = nullptr;
inline void hook_mrAimSilent(void *_this, float a1, float a2) {
    if (_this != nullptr && AimEnable && AimSilent) {
        void *CurrentMatch = Curent_Match();
        if (CurrentMatch != NULL) {
            void* LocalPlayer = GetLocalPlayer(CurrentMatch);
            if (LocalPlayer != NULL) {
                bool visible = isVisible_Old(LocalPlayer);
                if (visible) {
                    if (get_IsFiring(GetLocalPlayer(CurrentMatch))) return;
                    else if (orig_mrSilentAim) orig_mrSilentAim(_this, a1, a2);
                    return;
                }
            }
        }
    }
    if (orig_mrSilentAim) orig_mrSilentAim(_this, a1, a2);
}

inline void _DarkUpdateX(void *player) {
    if (player != nullptr) {
        void *current_match = Curent_Match();
        if (current_match != NULL) {
            void *local_player = Current_Local_Player();
            if (local_player == NULL) local_player = GetLocalPlayerOrObServer();
            if (local_player != NULL) {
                void *fakeCamPlayer = get_MyFollowCamera(local_player);
                void *fakeCamEnemy  = get_MyFollowCamera(player);
                if (fakeCamPlayer != NULL && fakeCamEnemy != NULL) {
                    void *fakeCamPlayerTF = Component_GetTransform(fakeCamPlayer);
                    void *fakeCamEnemyTF  = Component_GetTransform(player);
                    if (fakeCamPlayerTF != NULL && fakeCamEnemyTF != NULL) {
                        Vector3 fakeCamPlayerPos = Transform_INTERNAL_GetPosition(fakeCamPlayerTF);
                        Vector3 fakeCamEnemyPos  = Transform_INTERNAL_GetPosition(fakeCamEnemyTF);
                        float distance = Vector3::Distance(fakeCamPlayerPos, fakeCamEnemyPos);
                        if (player != local_player && distance > 0.1f) {
                            bool isSameTeam = get_isLocalTeam(player);
                            bool isVis      = get_isVisible(player);
                            bool isDying    = get_IsDieing(player);
                            if (!isSameTeam && isVis && !isDying) {
                                if (ActiveFeature) Aimbot2(local_player, player);
                            }
                        }
                    }
                }
            }
        }
    }
}

inline int GetAimHeightPercent() {
    if (AimHead35) return 35;
    if (AimHead50) return 50;
    if (AimHead75) return 75;
    if (AimHead90) return 90;
    if (AimHead100) return 100;
    return 100;
}

inline void (*LateUpdate_Orig)(void *Player) = nullptr;
inline void _LateUpdate(void *Player) {
    if (Player != NULL && Aimbot) {
        void* CurrentMatch = Curent_Match();
        void* closestEnemy = GetClosestEnemy();
        void* LocalPlayer = GetLocalPlayer(CurrentMatch);
        if (closestEnemy != NULL && LocalPlayer != NULL && CurrentMatch != NULL) {
            Vector3 EnemyHead = GetHeadPosition(closestEnemy);
            Vector3 RootPosition = GetHeadPosition(closestEnemy);
            int AimHeight = GetAimHeightPercent();
            float heightDelta = EnemyHead.y - RootPosition.y;
            EnemyHead.y = RootPosition.y + (heightDelta * (AimHeight / 100.0f));
            Vector3 PlayerLocation = CameraMain(LocalPlayer);
            Quaternion PlayerLook = GetRotationToTheLocation(EnemyHead, AimSpeed, PlayerLocation);
            bool IsScopeOn = get_IsSighting(LocalPlayer);
            bool IsFiring  = get_IsFiring(LocalPlayer);
            if (AimWhen == 0 || (AimWhen == 1 && IsFiring) || (AimWhen == 2 && IsScopeOn)) {
                set_aim(LocalPlayer, PlayerLook);
            }
        }
    }
    if (LateUpdate_Orig) LateUpdate_Orig(Player);
}

inline bool saved = false;
inline float savedScreenWidth = 0;
inline float savedScreenHeight = 0;
inline float active = 0.0f;
inline float desactive = 0.0f;

inline void UpdateSpeedhack() {
    if (Enable) {
        void* Simulation = GetSimulationTimer();
        if (Simulation != nullptr) {
            float FixedDeltaTime = GetTimer(Simulation);
            if (!saved) {
                active = FixedDeltaTime * 1.82f;
                desactive = FixedDeltaTime;
                saved = true;
            }
            if (SpeedHack) {
                if (FixedDeltaTime != active) SetTimer(Simulation, active);
            } else {
                if (FixedDeltaTime != desactive) SetTimer(Simulation, desactive);
            }
        }
    }
}

inline float (*get_deltaTime)() = nullptr;
inline float smoothSpeedAIM = 5;
inline void *MyPlayer = NULL;
inline bool Drone = false;
inline const float RADAR_SIZE = 200.0f;
inline const ImVec2 RADAR_CENTER(100.0f, 100.0f);
inline float x, y;
inline float enemy1 = 2.0f;

inline bool block_connections = true;

inline ImVec4 colorTextBackground = ImVec4(0.02f, 0.02f, 0.05f, 0.85f);
inline ImVec4 colorBox          = ImVec4(1.0f, 0.0f, 0.35f, 1.0f);
inline ImVec4 colorHealth       = ImVec4(0.0f, 1.0f, 0.5f, 1.0f);
inline ImVec4 colorDistance     = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
inline ImVec4 colorLine         = ImVec4(0.0f, 1.0f, 0.8f, 0.6f);
inline ImVec4 colorHexagono     = ImVec4(1.0f, 0.0f, 0.5f, 1.0f);
inline ImVec4 colorMenuText     = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
inline ImVec4 NeonColor         = ImVec4(0.0f, 1.0f, 0.7f, 1.0f);

inline void* CurrentMatch = nullptr;
inline void* closestEnemy_Internal = nullptr;
inline void* LocalPlayer_Internal = nullptr;
inline float aimSmoothness = 0.1f;

inline void DrawAIMBOT(float screenWidth, float screenHeight) {
    if (!Enable || !Aimbot) return;
    void* currentMatch = Curent_Match();
    if (!currentMatch) return;
    void* localPlayer = GetLocalPlayer(currentMatch);
    if (!localPlayer || get_IsDieing(localPlayer)) return;
    void* camera = Camera_main();
    if (!camera) return;
    Vector3 cameraLocation = Transform_GetPosition(Component_GetTransform(camera));
    float closestDist = Aimdis;
    void* bestTarget = nullptr;
    MyDictionary* dict = *(MyDictionary**)((uintptr_t)currentMatch + ListPlayer);
    if (dict && dict->entries) {
        int total = dict->count;
        for (int u = 0; u < total; u++) {
            void* enemy = dict->entries->vector[u].value;
            if (!enemy || enemy == localPlayer) continue;
            if (get_isLocalTeam(enemy)) continue;
            if (get_IsDieing(enemy)) continue;
            Vector3 enemyHeadPos = GetHeadPosition(enemy);
            float dist = Vector3::Distance(cameraLocation, enemyHeadPos);
            if (dist < closestDist) {
                Vector3 targetDir = Vector3::Normalized(enemyHeadPos - cameraLocation);
                float angle = Vector3::Angle(targetDir,
                    GetForward(Component_GetTransform(camera))) * 100.0f;
                if (angle <= Fov_Aim) {
                    closestDist = dist;
                    bestTarget = enemy;
                }
            }
        }
    }
    if (bestTarget) {
        Vector3 targetHead = GetHeadPosition(bestTarget);
        Quaternion targetRot = GetRotationToTheLocation(targetHead, 0.1f, cameraLocation);
        bool isScopeOn = get_IsSighting(localPlayer);
        bool isFiring  = get_IsFiring(localPlayer);
        if (AimWhen == 0) set_aim(localPlayer, targetRot);
        else if (AimWhen == 1 && isFiring) set_aim(localPlayer, targetRot);
        else if (AimWhen == 2 && isScopeOn) set_aim(localPlayer, targetRot);
    }
}

inline void* GetClosestEnemyForAimKill() {
    float shortestDistance = 99999.0f;
    void* bestEnemy = nullptr;
    void* match = Curent_Match();
    if (!match) return nullptr;
    void* LocalPlayer = GetLocalPlayer(match);
    if (!LocalPlayer) return nullptr;
    auto players = *(MyDictionary**)((uintptr_t)match + ListPlayer);
    if (!players || !players->entries) return nullptr;
    auto entries = players->entries;
    void* camera = Camera_main();
    if (!camera) return nullptr;
    Vector3 camPos = CameraMain(LocalPlayer);
    Vector3 camFwd = GetForward(Component_GetTransform(camera));
    for (int u = 0; u < players->count; u++) {
        DictionaryEntry& entry = entries->vector[u];
        if (entry.hashCode < 0) continue;
        void* Player = entry.value;
        if (!Player || Player == LocalPlayer) continue;
        if (get_isLocalTeam(Player)) continue;
        if (get_IsDieing(Player)) continue;
        if (get_God(Player)) continue;
        if (get_MaxHP(Player) <= 0) continue;
        Vector3 enemyPos = AimKillHeadOnly ? GetHeadPosition(Player) : getPosition(Player);
        float distance = Vector3::Distance(camPos, enemyPos);
        if (distance > AimKillDistance) continue;
        if (distance > Aimdis) continue;
        Vector3 targetDir = Vector3::Normalized(enemyPos - camPos);
        float angle = Vector3::Angle(targetDir, camFwd) * 100.0f;
        if (angle > AimKillFov) continue;
        if (!AimKillIgnoreVisible) {
            if (!IsVisible(Player)) continue;
        }
        if (angle < shortestDistance) {
            shortestDistance = angle;
            bestEnemy = Player;
        }
    }
    return bestEnemy;
}

inline void *GKHECDLGAJA(void *pthis, void* a1) {
    return ((void* (*)(void *,void *))(offset_GKHECDLGAJA))(pthis, a1);
}
inline monoList<float *> *LCLHHHKFCFP(void *Weapon, void *CAGCICACKCF, void *HFBDJJDICLN,
                                       bool LDGHPOPPPNL, DamageInfo2_o *DamageInfo) {
    return ((monoList<float *> * (*)(void*,void*,void*,bool,DamageInfo2_o*))(offset_LCLHHHKFCFP))
        (Weapon, CAGCICACKCF, HFBDJJDICLN, LDGHPOPPPNL, DamageInfo);
}
inline uint32_t CFFPIACECIG(COW_GamePlay_IHAAMHPPLMG_o id) {
    return ((uint32_t (*)(COW_GamePlay_IHAAMHPPLMG_o))(offset_CFFPIACECIG))(id);
}
inline message_PVector3IntPos_o *PackVector3Pos(Vector3 v) {
    return ((message_PVector3IntPos_o* (*)(Vector3))offset_LHGGPDLOPAH)(v);
}
inline bool GameFacade_Send(uint32_t messageID, void *msg, uint8_t sendOption, bool cacheMsgAnyWay) {
    return ((bool (*)(uint32_t,void *,uint8_t,bool))offset_SendFunction)(messageID,msg,sendOption,cacheMsgAnyWay);
}
inline void StartFiring_H(void *Player, void *Weapon) {
    void (*_f)(void *, void *) = (void (*)(void *, void *))(offset_StartFiring);
    _f(Player, Weapon);
}
inline void StopFire1_H(void* Player, void* Weapon) {
    void(*_f)(void*,void*) = (void(*)(void*,void*))(offset_StopFire);
    _f(Player, Weapon);
}
inline void StartWholeBodyFiring_H(void* Player, void* Weapon) {
    void(*_f)(void*,void*) = (void(*)(void*,void*))(offset_StartWholeBodyFiring);
    _f(Player, Weapon);
}
inline void StartonFiring_H(void *LocalPlayer, void *WeaponHand) {
    StartFiring_H(LocalPlayer, WeaponHand);
    StartWholeBodyFiring_H(LocalPlayer, WeaponHand);
}
inline float get_realtimeSinceStartup_H() {
    float (*_f)(void *nuls) = (float (*)(void *))(offset_get_realtimeSinceStartup);
    return _f(NULL);
}
inline void* CurrentGameSimulationTimer_H() {
    return ((void* (*)(void *))(offset_SimulationTimer))(NULL);
}
inline COW_GamePlay_IHAAMHPPLMG_o GetplayerID_H(void *_this) {
    return ((COW_GamePlay_IHAAMHPPLMG_o (*)(void *))(GetPlayerID111))(_this);
}
inline void *get_gameObject_H(void *Pthis) {
    return ((void* (*)(void *))(offset_get_gameObject))(Pthis);
}

inline int32_t TakeDamageFixed(
    void* enemyNetwork, int32_t weaponID, void* weaponInstance, void* damageInfo,
    int32_t baseDamage, Vector3 firePos, Vector3 hitPos,
    monoList<float*>* paramCheck, void* objectPool, uint32_t extraData) {
    return ((int32_t (*)(
        void*, int32_t, void*, void*, int32_t,
        Vector3, Vector3, monoList<float*>*, void*, uint32_t))(offset_TakeDamage))
        (enemyNetwork, weaponID, weaponInstance, damageInfo, baseDamage,
         firePos, hitPos, paramCheck, objectPool, extraData);
}

inline void AutoKillLogic() {
    if (!AutoKill) return;
    void* match = Curent_Match();
    if (!match) return;
    void* LocalPlayer = GetLocalPlayer(match);
    if (!LocalPlayer) return;
    if (get_IsDieing(LocalPlayer)) return;
    void* target = GetClosestEnemyForAimKill();
    if (!target) return;
    float now = get_realtimeSinceStartup_H();
    float elapsedMs = (now - lastAutoKillTime) * 1000.0f;
    if (elapsedMs < AutoKillDelayMs) return;
    lastAutoKillTime = now;

    void* WeaponHand = GetWeaponOnHand1(LocalPlayer);
    if (!WeaponHand) return;
    int baseDamage = *(int*)((uintptr_t)WeaponHand + offset_BaseDamage);
    if (baseDamage <= 0) baseDamage = 100;
    void* objectPool = *(void**)((uintptr_t)LocalPlayer + offset_GEGFCFDGGGP);
    if (!objectPool) return;
    void* headCollider = Player_GetHeadCollider(target);
    if (!headCollider) return;
    void* Class_message_DamageInfo_c = Classdamageinfo;
    if (!Class_message_DamageInfo_c) return;
    auto damageBaseInfo = (DamageInfo2_o *)
        ((void* (*)(void*))(BNJIPPIIMDF1616))(Class_message_DamageInfo_c);
    if (!damageBaseInfo) return;

    Vector3 firePos = GetHeadPosition(LocalPlayer);
    Vector3 hitPos  = AimKillHeadOnly ? GetHeadPosition(target) : getPosition(target);
    int weaponID    = GetWeaponID(WeaponHand);

    ((void (*)(void*, void*))offset_SetStartDamage)(WeaponHand, objectPool);
    damageBaseInfo->HitColliderType = 1;
    damageBaseInfo->Weapon          = WeaponHand;
    damageBaseInfo->WeaponDataID    = weaponID;
    damageBaseInfo->FirePos         = firePos;
    damageBaseInfo->HitPos          = hitPos;
    damageBaseInfo->Damager         = GetplayerID_H(LocalPlayer);
    damageBaseInfo->SpecialHitType  = 0;
    damageBaseInfo->ForceNoHeadshot = false;
    damageBaseInfo->isBackArea      = false;
    damageBaseInfo->ExtraInfo       = 0;

    *(void**)((uintptr_t)objectPool + offset_JMKMBNIBFNA) = get_gameObject_H(headCollider);
    *(void**)((uintptr_t)objectPool + offset_GHACJPMCEDK) = headCollider;
    *(int*)((uintptr_t)objectPool + offset_OJKBBAOPPIN)   = 1;

    void* sl3 = GKHECDLGAJA(LocalPlayer, objectPool);
    if (!sl3) return;
    monoList<float*>* paramCheck = LCLHHHKFCFP(WeaponHand, sl3, headCollider, false, damageBaseInfo);
    if (!paramCheck) return;
    StartonFiring_H(LocalPlayer, WeaponHand);
    TakeDamageFixed(target, weaponID, WeaponHand, damageBaseInfo, baseDamage,
                    firePos, hitPos, paramCheck, objectPool, 0);
    StopFire1_H(LocalPlayer, WeaponHand);
}

inline void StartTakeDamage(void *ClosestEnemy) {
    if (ClosestEnemy == nullptr || !AimKill1) return;
    void* trandau = Curent_Match();
    if (!trandau) return;
    void* LocalPlayer = GetLocalPlayer(trandau);
    if (!LocalPlayer) return;
    if (get_IsDieing(LocalPlayer)) return;
    if (get_IsDieing(ClosestEnemy)) return;
    void *WeaponHand = GetWeaponOnHand1(LocalPlayer);
    if (!WeaponHand) return;
    int baseDamage = *(int*)((uintptr_t)WeaponHand + offset_BaseDamage);
    if (baseDamage <= 0) baseDamage = 100;
    void* objectPool = *(void**)((uintptr_t)LocalPlayer + offset_GEGFCFDGGGP);
    if (!objectPool) return;
    void* headCollider = Player_GetHeadCollider(ClosestEnemy);
    if (!headCollider) return;
    void *Class_message_DamageInfo_c = Classdamageinfo;
    if (!Class_message_DamageInfo_c) return;
    auto damageBaseInfo = (DamageInfo2_o *)
        ((void * (*)(void *))(BNJIPPIIMDF1616))(Class_message_DamageInfo_c);
    if (!damageBaseInfo) return;
    Vector3 firePos = GetHeadPosition(LocalPlayer);
    Vector3 hitPos  = GetHeadPosition(ClosestEnemy);
    int weaponID    = GetWeaponID(WeaponHand);
    ((void (*)(void*, void*))offset_SetStartDamage)(WeaponHand, objectPool);
    damageBaseInfo->HitColliderType = 1;
    damageBaseInfo->Weapon          = WeaponHand;
    damageBaseInfo->WeaponDataID    = weaponID;
    damageBaseInfo->FirePos         = firePos;
    damageBaseInfo->HitPos          = hitPos;
    damageBaseInfo->Damager         = GetplayerID_H(LocalPlayer);
    damageBaseInfo->SpecialHitType  = 0;
    damageBaseInfo->ForceNoHeadshot = false;
    damageBaseInfo->isBackArea      = false;
    damageBaseInfo->ExtraInfo       = 0;
    *(void**)((uintptr_t)objectPool + offset_JMKMBNIBFNA) = get_gameObject_H(headCollider);
    *(void**)((uintptr_t)objectPool + offset_GHACJPMCEDK) = headCollider;
    *(int*)((uintptr_t)objectPool + offset_OJKBBAOPPIN)   = 1;
    void* sl3 = GKHECDLGAJA(LocalPlayer, objectPool);
    if (!sl3) return;
    monoList<float*>* paramCheck = LCLHHHKFCFP(WeaponHand, sl3, headCollider, false, damageBaseInfo);
    if (!paramCheck) return;
    StartonFiring_H(LocalPlayer, WeaponHand);
    TakeDamageFixed(ClosestEnemy, weaponID, WeaponHand, damageBaseInfo, baseDamage,
                    firePos, hitPos, paramCheck, objectPool, 0);
    StopFire1_H(LocalPlayer, WeaponHand);
}

inline void* GetEnemyThroughWall(void* match, void* localPlayer) {
    if (!match || !localPlayer || !Startgs) return nullptr;
    float shortestDistance = 99999.0f;
    void* closestEnemy = nullptr;
    auto players = *(MyDictionary**)((uintptr_t)match + ListPlayer);
    if (!players || !players->entries) return nullptr;
    auto entries = players->entries;
    Vector3 camPos = CameraMain(localPlayer);
    for (int u = 0; u < players->count; u++) {
        DictionaryEntry& entry = entries->vector[u];
        if (entry.hashCode < 0) continue;
        void* Player = entry.value;
        if (!Player || Player == localPlayer) continue;
        if (!get_isLocalTeam(Player) && !get_IsDieing(Player) &&
            get_MaxHP(Player) > 0 && !get_God(Player)) {
            Vector3 PlayerPos = GetHeadPosition(Player);
            float distance = Vector3::Distance(camPos, PlayerPos);
            if (distance > Aimdis) continue;
            Vector3 targetDir = Vector3::Normalized(PlayerPos - camPos);
            float angle = Vector3::Angle(targetDir,
                GetForward(Component_GetTransform(Camera_main()))) * 100.0f;
            if (angle <= AimLockFov && angle < shortestDistance) {
                if (!AimLockThroughWall && !IsVisible(Player)) continue;
                shortestDistance = angle;
                closestEnemy = Player;
            }
        }
    }
    return closestEnemy;
}

inline void AimLockLogic(void* localPlayer, void* match) {
    if (!AimLock || !localPlayer) return;
    void* target = nullptr;
    if (AimLockThroughWall) target = GetEnemyThroughWall(match, localPlayer);
    else                    target = EnemyVisible(match);
    if (target) {
        LockedEnemy = target;
        LockedEnemyTime = (float)ImGui::GetTime();
    } else {
        float now = (float)ImGui::GetTime();
        if (LockedEnemy && (now - LockedEnemyTime) < 0.5f) {
            if (get_MaxHP(LockedEnemy) > 0 && !get_IsDieing(LockedEnemy)) target = LockedEnemy;
            else LockedEnemy = nullptr;
        }
    }
    if (!target) return;
    Vector3 enemyPos = GetHeadPosition(target);
    enemyPos.y += 0.08f;
    Vector3 camPos = CameraMain(localPlayer);
    Quaternion targetRot = GetRotationToTheLocation(enemyPos, AimLockSpeed, camPos);
    set_aim(localPlayer, targetRot);
}

inline void CustomFireRateLogic(void* localPlayer, void* lockedEnemy) {
    if (!CustomFireRate || !localPlayer || !lockedEnemy) return;
    void* WeaponHand = GetWeaponOnHand1(localPlayer);
    if (!WeaponHand) return;
    if (!AimLockThroughWall) {
        if (!IsVisible(lockedEnemy)) return;
    }
    float now = (float)ImGui::GetTime();
    float interval = FireRateMs / 1000.0f;
    if (now - lastFireTime < interval) return;
    lastFireTime = now;
    int burst = (int)ShotsPerBurst;
    for (int i = 0; i < burst; i++) {
        StartFiring_H(localPlayer, WeaponHand);
        StopFire1_H(localPlayer, WeaponHand);
    }
}

inline const char* MY_NAME = "[FF0000]Okomodz [0000FF]God [FFFFFF]of functions";

inline void SpoofAllNames() {
    void* matchGame = Curent_Match();
    if (!matchGame) return;
    static monoString* cachedSpoofName = nullptr;
    if (!cachedSpoofName) {
        cachedSpoofName = reinterpret_cast<monoString*>(Il2CppString::Create(MY_NAME));
    }
    if (!cachedSpoofName) return;

    void* localPlayer = GetLocalPlayer(matchGame);
    if (localPlayer) {
        monoString** nameFieldPtr = (monoString**)((uintptr_t)localPlayer + _NickName);
        if (nameFieldPtr && *nameFieldPtr != cachedSpoofName) *nameFieldPtr = cachedSpoofName;
    }

    MyDictionary* dict = *(MyDictionary**)((uintptr_t)matchGame + ListPlayer);
    if (!dict || !dict->entries) return;
    for (int u = 0; u < dict->count; u++) {
        void* Player = dict->entries->vector[u].value;
        if (!Player) continue;
        monoString** nameFieldPtr = (monoString**)((uintptr_t)Player + _NickName);
        if (nameFieldPtr && *nameFieldPtr != cachedSpoofName) *nameFieldPtr = cachedSpoofName;
    }
}

inline monoString* SpoofMsg(monoString* original) {
    if (!original) return original;
    std::string msg = MonoToStr(original);
    if (msg.empty()) return original;
    if (msg.find(MY_NAME) != std::string::npos) return original;
    void* match = Curent_Match();
    if (!match) return original;
    MyDictionary* dict = *(MyDictionary**)((uintptr_t)match + ListPlayer);
    if (!dict || !dict->entries) return original;
    bool changed = false;
    for (int i = 0; i < dict->count; i++) {
        void* p = dict->entries->vector[i].value;
        if (!p) continue;
        monoString** nameFieldPtr = (monoString**)((uintptr_t)p + _NickName);
        if (!nameFieldPtr || !*nameFieldPtr) continue;
        std::string name = MonoToStr(*nameFieldPtr);
        if (name.empty() || name == MY_NAME) continue;
        if (name.length() < 3) continue;
        if (msg.find(name) == std::string::npos) continue;
        msg = ReplaceAll(msg, name, MY_NAME);
        changed = true;
    }
    if (!changed) return original;
    return StrToMono(msg);
}

inline std::vector<BonePos> GetAllNodePositions(void* enemy) {
    static const struct { const char* name; uintptr_t offset; } bones[] = {
        {"Head",          m_HeadNode},
        {"Neck",          m_Neck},
        {"Hip",           m_HipNode},
        {"LeftShoulder",  m_LeftShoulder},
        {"RightShoulder", m_RightShoulder},
        {"LeftElbow",     m_LeftElbow},
        {"RightElbow",    m_RightElbow},
        {"LeftHand",      m_LeftHand},
        {"RightHand",     m_RightHand},
        {"LeftKnee",      m_LeftKnee},
        {"RightKnee",     m_RightKnee},
        {"LeftFoot",      m_LeftFoot},
        {"RightFoot",     m_RightFoot},
        {"Root",          m_Root},
    };
    std::vector<BonePos> result;
    result.reserve(std::size(bones));
    for (auto& b : bones) {
        void* iTransformNode = *(void**)((uintptr_t)enemy + b.offset);
        if (!iTransformNode) continue;
        void* transform = get_itransform(iTransformNode);
        if (!transform) continue;
        Vector3 pos = Transform_GetPosition(transform);
        result.push_back({ b.name, pos });
    }
    return result;
}

inline void DrawBones(const std::vector<BonePos>& bones, ImDrawList* drawList,
                      float distance, float screenHeight) {
    if (distance <= 0.0f) return;
    const float lineThickness = 0.05f;
    const ImU32 boneColor = ImGui::ColorConvertFloat4ToU32(ThemeColer);

    auto findIdx = [&](const char* name) -> int {
        for (int i = 0; i < (int)bones.size(); ++i)
            if (bones[i].name == name) return i;
        return -1;
    };

    std::vector<ImVec2> screen(bones.size());
    std::vector<bool>   visible(bones.size());

    for (size_t i = 0; i < bones.size(); ++i) {
        Vector3 s3 = WorldToScreenPoint(mainCamera, bones[i].pos);
        visible[i] = (s3.z > 0.0f);
        screen[i]  = ImVec2(s3.x, screenHeight - s3.y);
    }

    auto Line = [&](const char* a, const char* b) {
        int ia = findIdx(a), ib = findIdx(b);
        if (ia >= 0 && ib >= 0 && visible[ia] && visible[ib])
            drawList->AddLine(screen[ia], screen[ib], boneColor, lineThickness);
    };

    Line("Neck", "Hip");
    Line("Neck", "LeftShoulder");
    Line("Neck", "RightShoulder");
    Line("LeftShoulder", "LeftElbow");
    Line("RightShoulder", "RightElbow");
    Line("LeftElbow", "LeftHand");
    Line("RightElbow", "RightHand");
    Line("Hip", "LeftKnee");
    Line("Hip", "RightKnee");
    Line("LeftKnee", "LeftFoot");
    Line("RightKnee", "RightFoot");

    int headIdx = findIdx("Head");
    int neckIdx = findIdx("Neck");
    if (headIdx >= 0 && neckIdx >= 0 && visible[headIdx] && visible[neckIdx]) {
        ImVec2 neckPos = screen[neckIdx];
        ImVec2 headPos = screen[headIdx];
        float dx = headPos.x - neckPos.x;
        float dy = headPos.y - neckPos.y;
        ImVec2 extendedHead = ImVec2(neckPos.x + (dx * 2.5f), neckPos.y + (dy * 2.5f));
        drawList->AddLine(neckPos, extendedHead, boneColor, lineThickness);
    }
}

inline void* originalSkybox = nullptr;
inline bool  skyboxSaved = false;

inline void BlackSkyUpdate() {
    if (EnableNightMode) {
        if (!skyboxSaved) {
            originalSkybox = get_skybox_Original();
            skyboxSaved = true;
        }
        set_skybox_Null(nullptr);
        set_ambientMode_Black(3);
    } else {
        if (skyboxSaved && originalSkybox) {
            set_skybox_Null(originalSkybox);
            set_ambientMode_Black(0);
            skyboxSaved = false;
            originalSkybox = nullptr;
        }
    }
}

inline void* LineGrenade = nullptr;
inline void* RenderLine  = nullptr;
inline void (*UpdateGranada)(void* _this) = nullptr;

inline void _UpdateGranada(void* _this) {
    if (_this) {
        LineGrenade = _this;
        *(bool*)((uintptr_t)_this + 0x30) = true;
        RenderLine = *(void**)((uintptr_t)_this + 0x38);
    }
    if (UpdateGranada) UpdateGranada(_this);
}

inline void UpdateShurikenKill(void* localPlayer) {
    if (!localPlayer) return;
    void* playerAttributes = *(void**)((uintptr_t)localPlayer + 0x770);
    if (playerAttributes) {
        float* buffDamagePtr = (float*)((uintptr_t)playerAttributes + 0x128);
        if (ShurikenKill) *buffDamagePtr = 9014280192.0f;
        else              *buffDamagePtr = 1.0f;
    }
}

inline void* (*orig_PackVector3)(Vector3 pVehicle) = nullptr;
inline void* hook_PackVector3(Vector3 pVehicle) {
    if (DONTCarValue > 0) {
        void* match = Curent_Match();
        if (match) {
            void* PlayerClient = GetLocalPlayer(match);
            if (PlayerClient) {
                void* vehicle = GetLocalCar(PlayerClient);
                if (vehicle != nullptr && !get_IsPassenger(PlayerClient)) {
                    pVehicle.y -= (float)DONTCarValue;
                }
            }
        }
    }
    if (orig_PackVector3) return orig_PackVector3(pVehicle);
    return nullptr;
}

inline bool (*PCcheck)() = nullptr;
inline bool _PCcheck() {
    return false;
}

inline int (*get_UserLevel)(void *player) = nullptr;
inline int _get_UserLevel(void *player) {
    if (player != nullptr) {
        if (LUNDUP) return 12;
    }
    return get_UserLevel(player);
}

inline float (*orig_get_FSModeUseMedikitFasterRate)(void *thiz) = nullptr;
inline float my_get_FSModeUseMedikitFasterRate(void *thiz) {
    if (FUCKMODI) return FUCKMODIRate;
    if (orig_get_FSModeUseMedikitFasterRate)
        return orig_get_FSModeUseMedikitFasterRate(thiz);
    return 0.0f;
}

inline bool (*old_IsFreeMove)(void *instance) = nullptr;
inline bool hook_IsFreeMove(void *instance) {
    if (instance != NULL && FreeMove) return true;
    if (old_IsFreeMove) return old_IsFreeMove(instance);
    return false;
}

inline void (*old_PlayStopFire)(void *instance) = nullptr;
inline void hook_PlayStopFire(void *instance) {
    if (instance != NULL && StopFireAnim) return;
    if (old_PlayStopFire) old_PlayStopFire(instance);
}

inline bool (*orig_get_IsParachuting)(void* instance) = nullptr;
inline bool hook_get_IsParachuting(void* instance) {
    if (Enable && instance && isParachuting) return true;
    if (orig_get_IsParachuting) return orig_get_IsParachuting(instance);
    return false;
}

inline bool (*orig_IsMoving_1)(void *thiz) = nullptr;
inline bool my_IsMoving_Hook_1(void *thiz) {
    if (Enable && MedkitMoving) return false;
    if (orig_IsMoving_1) return orig_IsMoving_1(thiz);
    return false;
}

inline float (*orig_GetSensitivity)() = nullptr;
inline float hook_GetSensitivity() {
    if (Enable && FastSensivity) return Sensivity;
    if (orig_GetSensitivity) return orig_GetSensitivity();
    return 1.0f;
}

inline float (*orig_GetReloadScale)(void*, int) = nullptr;
inline float _GetReloadScale(void* thiz, int NJMMDDMIION) {
    if (QuickReloadLakiyaxMods) return 999.0f;
    if (orig_GetReloadScale) return orig_GetReloadScale(thiz, NJMMDDMIION);
    return 1.0f;
}

inline Quaternion YawToQuaternion(float yawDeg) {
    float rad  = yawDeg * 0.017453292f;
    float half = rad * 0.5f;
    Quaternion q;
    q.x = 0.0f;
    q.y = sinf(half);
    q.z = 0.0f;
    q.w = cosf(half);
    return q;
}

inline void UpdateSpinBot() {
    if (!Enable || !SpinBot) return;
    void* match = Curent_Match();
    if (!ptr_ok(match)) return;
    void* local = GetLocalPlayer(match);
    if (!ptr_ok(local)) return;
    void* tf = Component_GetTransform(local);
    if (!ptr_ok(tf)) return;
    float delta = ImGui::GetIO().DeltaTime;
    if (delta <= 0.0f) return;
    static float yaw = 0.0f;
    yaw += SpinBotSpeed * delta;
    if (yaw >= 360.0f) yaw -= 360.0f;
    Transform_SetRotation(tf, YawToQuaternion(yaw));
}

inline Vector3 GetAdjustedPosition(void* closestEnemy) {
    Vector3 headPos = GetHeadPosition(closestEnemy);
    if (aimPosition == 1) headPos.y += NECK_OFFSET;
    else if (aimPosition == 2) {
        Vector3 hipPos = GetHipPosition(closestEnemy);
        headPos = Vector3::Lerp(headPos, hipPos, 0.5f);
    } else if (aimPosition == 3) {
        headPos = GetHipPosition(closestEnemy);
    }
    return headPos;
}

inline Quaternion GetCameraRotation(void* localPlayer) {
    return GetRotation(Component_GetTransform(Camera_main()));
}

inline void AntidevAimbotVoid() {
    if (!Enable || !ANTIDEV) return;
    void* current_Match = Curent_Match();
    if (!current_Match) return;
    void* local_player = GetLocalPlayer(current_Match);
    if (!local_player) return;
    void* closestEnemy = GetClosestEnemy();
    if (!closestEnemy) return;
    Vector3 enemyLocation = GetAdjustedPosition(closestEnemy);
    Vector3 playerLocation = CameraMain(local_player);
    void* currentCollider = *(void**)((uintptr_t)closestEnemy + 0x78);
    void* headCollider = Player_GetHeadCollider(closestEnemy);
    if (!headCollider) return;
    if (currentCollider != headCollider) {
        static auto lastSwitch = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSwitch).count();
        int minDelay = 50 + (rand() % 100);
        if (elapsed < minDelay) return;
        SetAimCollider(closestEnemy, headCollider);
        lastSwitch = now;
    }
}

inline void StartAutoRevive() {
    while (true) {
        usleep(3000);
        if (!autoReviveEnabled || !Enable) continue;
        void* match = Curent_Match();
        if (!match) continue;
        void* local = GetLocalPlayer(match);
        if (!local) continue;
    }
}

inline void StartGlider() {
    while (true) {
        usleep(10000);
        if (!gliderHackEnabled || !gliderInfiniteEnabled) continue;
        void* match = Curent_Match();
        if (!match) continue;
        void* local = GetLocalPlayer(match);
        if (!local) continue;
    }
}

inline void StartAimbotVisible() {
    while (true) {
        usleep(5000);
        if (!aimVisibleEnabled || !Enable) continue;
        void* match = Curent_Match();
        if (!match) continue;
        void* enemy = EnemyVisible(match);
        if (!enemy) continue;
        void* headCollider = Player_GetHeadCollider(enemy);
        if (!headCollider) continue;
        SetAimCollider(enemy, headCollider);
    }
}

inline void FlyHackThread() {
}

inline void DrawESP(float screenWidth, float screenHeight) {
    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    if (!draw) return;

    char fpsBuffer[32];
    sprintf(fpsBuffer, "FPS: %.1f", ImGui::GetIO().Framerate);
    draw->AddRectFilled(ImVec2(10, 10), ImVec2(110, 35), IM_COL32(10, 10, 20, 200), 6.0f);
    draw->AddRect(ImVec2(10, 10), ImVec2(110, 35), IM_COL32(0, 255, 150, 255), 6.0f, 0, 1.2f);
    draw->AddText(NULL, 16.0f, ImVec2(22, 14), IM_COL32(255, 255, 255, 255), fpsBuffer);

    int totalEnemies = 0;
    bool enemyInFOV = false;

    if (Enable) {
        void* current_Match = Curent_Match();
        void* local_player = GetLocalPlayer(current_Match);
        ImU32 electric_ruby = IM_COL32(255, 0, 90, 255);
        ImU32 liquid_emerald = IM_COL32(0, 255, 160, 255);

        if (local_player && current_Match) {
            MyDictionary* players = *(MyDictionary**)((uintptr_t)current_Match + ListPlayer);
            void* camera = Camera_main();
            if (players && players->entries && camera) {
                int total = players->count;
                for (int u = 0; u < total; u++) {
                    void* enemyPtr = players->entries->vector[u].value;
                    if (enemyPtr && enemyPtr != local_player &&
                        get_isVisible(enemyPtr) && !get_isLocalTeam(enemyPtr)) {
                        totalEnemies++;
                        Vector3 Toepos = getPosition(enemyPtr);
                        Vector3 Toeposi = WorldToScreenPoint(camera, Toepos);
                        if (Toeposi.z < 1) continue;
                        Vector3 HeadPos = Toepos + Vector3(0, 1.9f, 0);
                        Vector3 HeadPosition = WorldToScreenPoint(camera, HeadPos);
                        if (HeadPosition.z < 1) continue;

                        draw->AddCircle(ImVec2(screenWidth / 2, screenHeight / 2),
                                        Fov_Aim, liquid_emerald, 120, 1.8f);
                        enemyInFOV = true;

                        float distance = Vector3::Distance(getPosition(local_player), Toepos);
                        float Height = abs(HeadPosition.y - Toeposi.y) * 1.09f;
                        float Width = Height * 0.55f;
                        Rect rect = Rect(HeadPosition.x - Width / 2.f,
                                         screenHeight - HeadPosition.y, Width, Height);

                        ImVec2 screenCenter(screenWidth / 2, screenHeight / 2);
                        ImVec2 targetHead(HeadPosition.x, screenHeight - HeadPosition.y);
                        draw->AddLine(screenCenter, targetHead, IM_COL32(255, 255, 0, 220), 1.5f);
                        draw->AddCircleFilled(targetHead, 4.0f, IM_COL32(255, 255, 0, 255));

                        if (Config.ESP.Line) {
                            draw->AddLine(ImVec2(rect.x + rect.w / 2, 0),
                                          ImVec2(rect.x + rect.w / 2, rect.y),
                                          IM_COL32(0, 255, 160, 130), 1.5f);
                        }

                        if (Config.ESP.Health) {
                            long clr = IM_COL32(0, 255, 150, 255);
                            int maxHp = get_MaxHP(enemyPtr);
                            int currentHp = GetHp(enemyPtr);
                            if (maxHp > 0) {
                                if (currentHp <= (maxHp * 0.6)) clr = IM_COL32(255, 160, 0, 255);
                                if (currentHp < (maxHp * 0.3))  clr = IM_COL32(255, 0, 60, 255);
                                int xx = rect.x - 8;
                                draw->AddRectFilled(ImVec2(xx, rect.y), ImVec2(xx + 3, rect.y + rect.h),
                                                    IM_COL32(10, 10, 15, 220), 2.0f);
                                draw->AddRectFilled(ImVec2(xx, rect.y + rect.h - (rect.h * ((float)currentHp / maxHp))),
                                                    ImVec2(xx + 3, rect.y + rect.h), clr, 2.0f);
                            }
                        }

                        if (Config.ESP.Box) {
                            float lW = rect.w * 0.25f;
                            float lH = rect.h * 0.25f;
                            ImVec2 topLeft(rect.x, rect.y);
                            ImVec2 bottomRight(rect.x + rect.w, rect.y + rect.h);
                            draw->AddLine(topLeft, ImVec2(topLeft.x + lW, topLeft.y), electric_ruby, 2.5f);
                            draw->AddLine(topLeft, ImVec2(topLeft.x, topLeft.y + lH), electric_ruby, 2.5f);
                            draw->AddLine(ImVec2(bottomRight.x, rect.y), ImVec2(bottomRight.x - lW, rect.y), electric_ruby, 2.5f);
                            draw->AddLine(ImVec2(bottomRight.x, rect.y), ImVec2(bottomRight.x, rect.y + lH), electric_ruby, 2.5f);
                            draw->AddLine(ImVec2(rect.x, bottomRight.y), ImVec2(rect.x + lW, bottomRight.y), electric_ruby, 2.5f);
                            draw->AddLine(ImVec2(rect.x, bottomRight.y), ImVec2(rect.x, bottomRight.y - lH), electric_ruby, 2.5f);
                            draw->AddLine(bottomRight, ImVec2(bottomRight.x - lW, bottomRight.y), electric_ruby, 2.5f);
                            draw->AddLine(bottomRight, ImVec2(bottomRight.x, bottomRight.y - lH), electric_ruby, 2.5f);
                            draw->AddRect(topLeft, bottomRight, IM_COL32(255, 0, 90, 40), 0, 0, 1.0f);
                        }

                        if (Config.ESP.MAFIA) {
                            monoString* Nick = get_NickName(enemyPtr);
                            std::string name = (Nick != nullptr) ? "TARGET" : "TARGET";
                            std::string distText = std::to_string((int)distance) + "M";
                            ImVec2 nameSize = ImGui::CalcTextSize(name.c_str());
                            float posX = rect.x + rect.w * 0.5f - (nameSize.x * 0.5f);
                            float posY = rect.y - 30.0f;
                            draw->AddRectFilled(ImVec2(posX - 10, posY - 2),
                                                ImVec2(posX + nameSize.x + 50, posY + 18),
                                                IM_COL32(10, 10, 20, 230), 4.0f);
                            draw->AddRect(ImVec2(posX - 10, posY - 2),
                                          ImVec2(posX + nameSize.x + 50, posY + 18),
                                          liquid_emerald, 4.0f, 0, 1.2f);
                            draw->AddText(NULL, 15.0f, ImVec2(posX, posY),
                                          IM_COL32(255, 255, 255, 255), name.c_str());
                            draw->AddText(NULL, 15.0f, ImVec2(posX + nameSize.x + 12, posY),
                                          liquid_emerald, distText.c_str());
                        }

                        if (EspSkeleton2) {
                            std::vector<BonePos> bonePositions = GetAllNodePositions(enemyPtr);
                            DrawBones(bonePositions, draw, distance, screenHeight);
                        }
                    }
                }
            }
        }
    }

    if (Enable) {
        ImU32 crosshairColor = enemyInFOV ? IM_COL32(255, 255, 0, 255) : IM_COL32(0, 255, 160, 200);
        draw->AddCircleFilled(ImVec2(screenWidth / 2, screenHeight / 2), 4.0f, crosshairColor);
    }

    if (RenderWatermark) {
        ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        ImVec2 wmSize = ImVec2(240, 38);
        ImVec2 wmPos = ImVec2((displaySize.x - wmSize.x) / 2, 20);
        ImVec2 wmEnd = ImVec2(wmPos.x + wmSize.x, wmPos.y + wmSize.y);
        draw->AddRectFilled(wmPos, wmEnd, IM_COL32(10, 10, 18, 230), 12.0f);
        draw->AddRect(wmPos, wmEnd, IM_COL32(0, 255, 160, 255), 12.0f, 0, 1.5f);
        std::string watermarkText = "SHADOW MODZ";
        ImVec2 textSize = ImGui::CalcTextSize(watermarkText.c_str());
        ImVec2 textPos = ImVec2(wmPos.x + (wmSize.x - textSize.x) / 2,
                                wmPos.y + (wmSize.y - textSize.y) / 2);
        draw->AddText(textPos, IM_COL32(255, 255, 255, 255), watermarkText.c_str());
    }

    if (totalEnemies > 0) {
        std::string enemyText = "ALIVE ENEMIES: " + std::to_string(totalEnemies);
        ImVec2 textSize = ImGui::CalcTextSize(enemyText.c_str());
        ImVec2 padding(16.0f, 6.0f);
        ImVec2 center(screenWidth * 0.5f, 85.0f);
        ImVec2 boxMin(center.x - textSize.x * 0.5f - padding.x, center.y - textSize.y * 0.5f - padding.y);
        ImVec2 boxMax(center.x + textSize.x * 0.5f + padding.x, center.y + textSize.y * 0.5f + padding.y);
        draw->AddRectFilled(boxMin, boxMax, IM_COL32(15, 10, 25, 220), 6.0f);
        draw->AddRect(boxMin, boxMax, IM_COL32(255, 0, 90, 255), 6.0f, 0, 1.5f);
        ImVec2 textPos(center.x - textSize.x * 0.5f, center.y - textSize.y * 0.5f);
        draw->AddText(textPos, IM_COL32(0, 255, 160, 255), enemyText.c_str());
    }
}

inline bool (*ResetGuest)(void* _this) = nullptr;
inline bool _ResetGuest(void* _this) {
    if (Guest) return true;
    if (ResetGuest) return ResetGuest(_this);
    return false;
}

inline float (*GetCameraHeightRateValue)(void *player) = nullptr;
inline float _GetCameraHeightRateValue(void *player) {
    if (player != nullptr) {
        if (camxa) return 4.0f;
    }
    if (GetCameraHeightRateValue) return GetCameraHeightRateValue(player);
    return 1.0f;
}

inline bool (*Bypass)(void* bypass) = nullptr;
inline bool _Bypass(void* bypass) { return false; }

inline bool (*_IsContinuousGen)(void *instance) = nullptr;
inline bool IsContinuousGen(void *instance) {
    if (instance != NULL && Enable && SniperFast) return true;
    if (_IsContinuousGen) return _IsContinuousGen(instance);
    return false;
}

inline void (*orig_PlayChangeWeaponAnim)(void*, void*, float) = nullptr;
inline void hook_PlayChangeWeaponAnim(void* _this, void* a, float b) {
    if (Enable && PcSwapAnim) return;
    if (orig_PlayChangeWeaponAnim) orig_PlayChangeWeaponAnim(_this, a, b);
}

inline int (*orig_ClientVersion)(void* _this) = nullptr;
inline int hook_ClientVersion(void* _this) {
    if (Setindia) return 3;
    if (orig_ClientVersion) return orig_ClientVersion(_this);
    return 0;
}

inline bool (*get_InSwapWeaponCD)(void* thiz) = nullptr;
inline bool _get_InSwapWeaponCD(void* thiz) {
    if (Fastswitch && Enable) return false;
    if (get_InSwapWeaponCD) return get_InSwapWeaponCD(thiz);
    return true;
}

inline bool (*orig_get_IsFastRunning)(void *instance) = nullptr;
inline bool hook_get_IsFastRunning(void *instance) {
    if (AutoRun) return true;
    if (orig_get_IsFastRunning) return orig_get_IsFastRunning(instance);
    return false;
}

inline float (*orig_get_Range)(void *instance) = nullptr;
inline float hook_get_Range(void *instance) {
    if (Enable && instance && InfiniteRange) return 9999.0f;
    if (orig_get_Range) return orig_get_Range(instance);
    return 1.0f;
}

inline void _Set_Aim(void* _this) { return; }

inline bool _FixGame(void* _this) {
    if (_this != NULL) return true;
    return false;
}
inline bool _BypassESP(void* _this) { return true; }
inline bool _Bypass2(void* _this) { return true; }

inline bool (*IsFoldWingGliding)(void *player) = nullptr;
inline bool _IsFoldWingGliding(void *player) {
    if (player != NULL) {
        if (Enable) {
            if (speedrun) return true;
        }
    }
    if (IsFoldWingGliding) return IsFoldWingGliding(player);
    return false;
}

inline int _LEBIPIGPEEP(void* GLDLCOBLGNF) {
    if (Enable) {
        if (speedrun) return 1;
    }
    return 0;
}

inline int (*MappingFromPhysXState)(void* state) = nullptr;
inline int _MappingFromPhysXState(void* state) {
    if (Enable && speedrun) return 1;
    if (MappingFromPhysXState) return MappingFromPhysXState(state);
    return 0;
}

inline bool (*get_ResetGuest)(void* thiz) = nullptr;
inline bool _get_ResetGuest(void* thiz) {
    if (Reset) {
        remove("/storage/emulated/0/com.garena.msdk/guest100067.dat");
        return true;
    }
    if (get_ResetGuest) return get_ResetGuest(thiz);
    return false;
}

inline bool (*GetCombineType)(void *thiz) = nullptr;
inline bool _GetCombineType(void *thiz) {
    if (thiz != NULL) {
        if (Enable && Garena_ki_mkc) return true;
    }
    if (GetCombineType) return GetCombineType(thiz);
    return false;
}

inline bool (*norecoil0)(void*_this, int value) = nullptr;
inline bool _norecoil0(void*_this, int value) {
    if (_this != NULL) {
        if (norecoil) return 2;
    }
    if (norecoil0) return norecoil0(_this, value);
    return false;
}

#endif