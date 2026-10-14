#pragma once
#ifndef BOOLS_H
#define BOOLS_H

#include <string>
#include <cmath>
#include <mutex>
#include <imgui.h>

#include "Unity/Vector3.hpp"

extern int g_GlWidth;
extern int g_GlHeight;

inline bool Enable = false;
inline int  AimCheck = 0;
inline int  AimType = 0;
inline int  AimWhen = 1;
inline bool AimActivar = false, AimCrouch = false;
inline float AimDistance = 0.0f;
inline bool esp = false;
inline float Size = 1000;
inline float AimDis = 0.0f;
inline float MoveSpeed = 0.0f;
inline bool Tele = false;
inline bool check_move = false;
inline bool AimKill = false;
inline bool Headshot = false;
inline bool Autoswap = false;
inline bool AutoCharge = false;
inline bool FastSwitch = false;
inline bool ActiveFeature = true;
inline bool AimVisible2 = true;
inline bool AimVisible = true;

inline int countEnemy = 0;
inline int SwapDelay = 200;
inline float AimFov2 = 360.0f;
inline float DistanceTele = 0;

inline bool AimSilent = false;
inline bool SilentAim = false;

inline bool Aimbot = false;
inline float Fov_Aim = 360.0f;
inline float Aimdis = 500.0f;

inline bool aimskill = false;

inline bool isInsideFOV(int x, int y) {
    if (!Size) return true;
    int circle_x = g_GlWidth / 2;
    int circle_y = g_GlHeight / 2;
    int rad = Size;
    return (x - circle_x) * (x - circle_x) + (y - circle_y) * (y - circle_y) <= rad * rad;
}

inline void *enemyPlayer = NULL;
inline void *myPlayer = NULL;

inline ImColor Esp_Box       = ImColor(255,255,255,229);
inline ImColor Esp_Grad1     = ImColor(255,255,255,229);
inline ImColor Esp_Grad2     = ImColor(255,255,255,229);
inline ImColor Esp_3DBox     = ImColor(255,255,255,229);
inline ImColor Esp_Fled      = ImColor(255,255,255,150);
inline ImColor Esp_Text      = ImColor(255,255,255,229);
inline ImColor Esp_Dist      = ImColor(255,255,255,229);
inline ImColor Esp_Filed     = ImColor(255,255,255,150);
inline ImColor Esp_Skel      = ImColor(255,255,255,150);
inline ImColor Esp_He        = ImColor(255,255,255,150);
inline ImColor Hp            = ImColor(0, 255, 0);
inline ImColor White         = ImColor(255, 255, 255);

inline ImColor die = ImColor(255,0,0);
inline ImColor hp  = ImColor(0,255,0,255);

inline const char* dir[] = {"NONE","FIRE","SCOPE"};

inline int   is_SpeedTime = 0;
inline float visual_circle_size = 0.0f;
inline float visual_circle_stroke = 1.0f;
inline float visual_lined_size = 0.0f;
inline float visual_lined_stroke = 1.0f;
inline float visual_esp_line = 1.0f;
inline float visual_esp_box = 1.0f;
inline float visual_esp_boxth = 1.0f;
inline float visual_esp_box_filedth = 1.0f;
inline float visual_esp_box_filed = 1.0f;
inline float visual_esp_skel = 1.0f;
inline int   visual_esp_line_type = 0;

//Silent aim
inline std::mutex silentLock;
inline void* g_HitObjInfo = nullptr;
inline Vector3 g_TargetDir = {0, 0, 0};
inline bool g_HasData = false;

inline std::string int_to_string(int num) {
    return std::to_string(num);
}

inline bool isFov(Vector3 vec1, Vector3 vec2, int radius) {
    int x = (int)vec1.x;
    int y = (int)vec1.y;
    int x0 = (int)vec2.x;
    int y0 = (int)vec2.y;
    if ((pow(x - x0, 2) + pow(y - y0, 2)) <= pow(radius, 2)) return true;
    return false;
}

#endif 