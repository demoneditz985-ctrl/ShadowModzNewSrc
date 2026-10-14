

#include <android/log.h>
#include <unistd.h>
#include <thread>
#include <limits>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <dlfcn.h>
#include <sys/mman.h>
#include <xdl.h>
#include <KittyUtils.h>
#include <KittyMemory.h>
#include <KittyMemory/MemoryPatch.h>
#include <Il2Cpp.h>
#include <SubstrateHook.h>
#include <CydiaSubstrate.h>

#include "SHADOWMODZ MODS/Gui.hpp"
#include "SHADOWMODZ MODS/main.h"
#include "fonts/FontAwesome6_solid.h"

#include "SHADOWMODZ MODS/firewall.h"

void hack_thread();

bool  EnableSmartAimbot   = false;
bool  SpeedHack           = false;
bool  RenderWatermark     = true;
bool  menu1 = false, menu2 = false, menu3 = false, menu4 = false;
bool  showWatermark       = true;
bool  rPritam             = false;
bool  TPBR                = false;
bool  Pritamaim           = false;
bool  PritamAimbot        = false;
bool  Reset               = false;
bool  Guest               = false;
bool  camxa               = false;
bool  SniperFast          = false;
bool  PcSwapAnim          = false;
bool  Setindia            = false;
bool  Fastswitch          = false;
bool  AutoRun             = false;
bool  InfiniteRange       = false;
bool  speedrun            = false;
bool  isSpeed             = false;
bool  Firewall            = true;
bool  health              = false;
bool  AimNewF             = true;
bool  ESPLine             = false;
bool  ESPBox              = false;
bool  EspBox3d            = false;
bool  SniperAlert         = false;
bool  SelectAim           = true;
bool  AimBody             = false;
bool  showAlert           = false;
bool  DrawFov             = false;
bool  ShubhHuRe           = false;
bool  Garena_ki_mkc       = false;
bool  ESPGrenade          = false;
bool  AimEnable           = false;
bool  AimWhen1 = false, AimWhen2 = false, AimWhen3 = false;
bool  camcao              = false;
bool  box                 = false;
bool  showbox1            = false;
bool  showline1           = false;
bool  showline2           = false;
bool  showline            = false;
bool  norecoil            = false;
bool  isParachuting       = false;
bool  MedkitMoving        = false;
bool  FreeMove            = false;
bool  StopFireAnim        = false;
bool  FastSensivity       = false;
bool  ShurikenKill        = false;
bool  AutoKill            = false;
bool  AimKill1            = false;
bool  AimKillIgnoreVisible = false;
bool  AimKillHeadOnly     = true;
bool  AimLock             = false;
bool  AimLockThroughWall  = false;
bool  CustomFireRate      = false;
bool  SpinBot             = false;
bool  autoReviveEnabled   = false;
bool  gliderHackEnabled   = false;
bool  gliderInfiniteEnabled = false;
bool  aimVisibleEnabled   = false;
bool  QuickReloadLakiyaxMods = false;
bool  LUNDUP              = true;
bool  FUCKMODI            = false;
bool  EnableNightMode     = false;
bool  EspSkeleton2        = false;
bool  ANTIDEV             = false;
bool  SpoofNameEnabled    = true;

float AutoKillDelayMs     = 100.0f;
float AimKillFov          = 500.0f;
float AimKillDistance     = 100.0f;
float AimLockFov          = 200.0f;
float AimLockSpeed        = 0.15f;
float FireRateMs          = 100.0f;
float ShotsPerBurst       = 1.0f;
float SpinBotSpeed        = 720.0f;
float aimVisibleFov       = 200.0f;
float aimVisibleMaxDist   = 200.0f;
float FUCKMODIRate        = 0.90f;
float Sensivity           = 1.0f;
float AiFov               = 15.0f;
float AimSpeed            = 0.1f;
float NECK_OFFSET         = -0.15f;
float LineThickness       = 2.0f;
float BoxThickness        = 2.0f;
float BoxRoundness        = 0.0f;
float RGBSpeed            = 1.0f;

int   SetDamage           = 1;
int   LineMode            = 0;
int   BoxStyle            = 0;
int   FovStyle            = 0;
int   HealthBoxStyle      = 0;
int   DONTCarValue        = 0;
int   damageCooldown      = 0;
int   delayCounter        = 0;
int   delayLimit          = 2;
int   delayLimit2         = 0;
int   DamageDelay         = 2;
int   aimPosition         = 0;
int   Fov_Aim_Old         = 360;
float Aimdis_Old          = 500.0f;
bool  aimVisibleIgnoreKnocked = true;

void* enemyTarget         = nullptr;
void* LockedEnemy         = nullptr;
float LockedEnemyTime     = 0.0f;
float lastFireTime        = 0.0f;
float lastAutoKillTime    = 0.0f;

bool  Startgs             = true;
void* mainCamera          = nullptr;
ImVec4 ThemeColer         = ImVec4(0.655f, 0.333f, 0.969f, 1.00f); // SHADOW MODZ purple

MassKill Mass;

bool AimHead35 = true, AimHead50 = false, AimHead75 = false, AimHead90 = false, AimHead100 = false;

bool g_LoginBypass400 = false;

// ---------------------------------------------------------------------------
//  SHADOW MODZ menu state  (the floating UI lives in
//  "SHADOWMODZ MODS/ShadowTheme.hpp" + "ShadowMenu.hpp")
// ---------------------------------------------------------------------------
ShadowMenu::Refs g_MenuRefs;
static bool  g_MenuBound = false;
static int   g_AimHeightIndex = 4;   // 0:35% 1:50% 2:75% 3:90% 4:100%

// keeps the aim height booleans in sync with the touch friendly selector
void ApplyAimHeightPreset(int index)
{
    g_AimHeightIndex = index;
    AimHead35  = (index == 0);
    AimHead50  = (index == 1);
    AimHead75  = (index == 2);
    AimHead90  = (index == 3);
    AimHead100 = (index == 4);
}

void OpenURL(Il2CppString* url);   // defined below

void ShadowOpenURL(const char* url)
{
    if (!url || !*url) return;
    OpenURL(Il2CppString::Create(url));
}

// wires the floating UI to the live feature globals (uses the X-macro list
// declared in ShadowMenu.hpp so the two can never drift apart)
void ShadowBindMenu()
{
#define SHADOW_BIND_BOOL(name, target)   g_MenuRefs.name = &target;
#define SHADOW_BIND_FLOAT(name, target)  g_MenuRefs.name = &target;
#define SHADOW_BIND_INT(name, target)    g_MenuRefs.name = &target;
    SHADOW_BOOL_FIELDS(SHADOW_BIND_BOOL)
    SHADOW_FLOAT_FIELDS(SHADOW_BIND_FLOAT)
    SHADOW_INT_FIELDS(SHADOW_BIND_INT)
#undef SHADOW_BIND_BOOL
#undef SHADOW_BIND_FLOAT
#undef SHADOW_BIND_INT

    g_MenuRefs.AimHeight = &g_AimHeightIndex;
}


// ---------------------------------------------------------------------------
//  SHADOW MODZ palette - deep black with neon purple accents
// ---------------------------------------------------------------------------
namespace CyberTheme {
    ImVec4 BgDeep       = ImVec4(0.027f, 0.020f, 0.043f, 1.00f);
    ImVec4 BgPanel      = ImVec4(0.063f, 0.051f, 0.094f, 1.00f);
    ImVec4 BgChild      = ImVec4(0.078f, 0.063f, 0.118f, 1.00f);
    ImVec4 NeonCyan     = ImVec4(0.655f, 0.333f, 0.969f, 1.00f); // purple accent
    ImVec4 NeonMagenta  = ImVec4(0.910f, 0.475f, 0.976f, 1.00f);
    ImVec4 MatrixGreen  = ImVec4(0.290f, 0.870f, 0.529f, 1.00f);
    ImVec4 Amber        = ImVec4(0.984f, 0.749f, 0.141f, 1.00f);
    ImVec4 TextPri      = ImVec4(0.929f, 0.910f, 0.992f, 1.00f);
    ImVec4 TextDim      = ImVec4(0.510f, 0.478f, 0.604f, 1.00f);
    ImVec4 Border       = ImVec4(0.655f, 0.333f, 0.969f, 0.47f);
    ImVec4 Danger       = ImVec4(0.957f, 0.247f, 0.369f, 1.00f);
    ImVec4 Success      = ImVec4(0.290f, 0.870f, 0.529f, 1.00f);
}

// the UI design system owns every style/imGui colour - see ShadowTheme.hpp
void ApplyCyberTheme() {
    ShadowUI::ApplyStyle();
}


static uintptr_t lib_base(const char* name) {
    if (!name) return 0;
    FILE* f = fopen("/proc/self/maps", "r");
    if (!f) return 0;
    char line[512];
    uintptr_t base = 0;
    while (fgets(line, sizeof(line), f)) {
        uintptr_t start, end;
        char perms[8];
        char path[256];
        if (sscanf(line, "%lx-%lx %7s %*s %*s %*s %255s",
                   &start, &end, perms, path) != 4)
            continue;
        if (strstr(path, name)) { base = start; break; }
    }
    fclose(f);
    return base;
}

static bool mem_write(uintptr_t addr, const uint8_t* data, size_t len) {
    if (!addr || !data || !len) return false;
    long pg = sysconf(_SC_PAGESIZE);
    uintptr_t page = addr & ~(uintptr_t)(pg - 1);
    size_t span = (addr + len - page + pg - 1) & ~(size_t)(pg - 1);
    if (mprotect((void*)page, span, PROT_READ | PROT_WRITE | PROT_EXEC) != 0)
        return false;
    memcpy((void*)addr, data, len);
    __builtin___clear_cache((char*)addr, (char*)(addr + len));
    mprotect((void*)page, span, PROT_READ | PROT_EXEC);
    return true;
}

static bool patch_hex(uintptr_t base, uintptr_t off, const char* hex) {
    if (!base || !hex) return false;
    uint8_t buf[64]; size_t n = 0;
    auto nib = [](char c)->int{
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return -1;
    };
    for (const char* p = hex; *p && n < sizeof(buf);) {
        while (*p == ' ') ++p;
        if (!*p) break;
        int hi = nib(p[0]), lo = nib(p[1]);
        if (hi < 0 || lo < 0) return false;
        buf[n++] = (uint8_t)((hi << 4) | lo);
        p += 2;
    }
    return mem_write(base + off, buf, n);
}

#define PATCH_LIB(lib, off, hex) \
    do { \
        uintptr_t _b = lib_base(lib); \
        if (_b) patch_hex(_b, (uintptr_t)(off), hex); \
    } while (0)

void OpenURL(Il2CppString* url) {
    if (!url) return;
    typedef void (*Application_OpenURL_ftn)(Il2CppString*);
    static Application_OpenURL_ftn open_url = nullptr;
    if (!open_url) {
        void* methodOffset = Il2CppGetMethodOffset(
            OBFUSCATE("UnityEngine.CoreModule.dll"),
            OBFUSCATE("UnityEngine"),
            OBFUSCATE("Application"),
            OBFUSCATE("OpenURL"),
            1
        );
        if (methodOffset) open_url = (Application_OpenURL_ftn)methodOffset;
    }
    if (open_url) open_url(url);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jvm = vm;
    std::thread(hack_thread).detach();
    return JNI_VERSION_1_6;
}



uintptr_t il2cpp_base = 0;
void *getRealAddr(ulong offset) {
    return reinterpret_cast<void*>(il2cpp_base + offset);
}

inline EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
inline EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH,  &g_GlWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &g_GlHeight);

    if (!g_IsSetup) {
        prevWidth  = g_GlWidth;
        prevHeight = g_GlHeight;
        SetupImgui();
        g_IsSetup = true;
    }

    
    static bool isTouching = false;
    static float touchStartTime = 0.0f;
    ImGuiIO& io = ImGui::GetIO();

    if (Class_Input__get_touchCount && Class_Input__GetTouch) {
        int tc = ((int (*)())(Class_Input__get_touchCount))();
        if (tc > 0) {
            UnityEngine_Touch_Fields touch = ((UnityEngine_Touch_Fields(*)(int))(Class_Input__GetTouch))(0);
            float revY = io.DisplaySize.y - touch.m_Position.fields.y;
            io.MousePos = ImVec2(touch.m_Position.fields.x, revY);
            if (!isTouching) touchStartTime = ImGui::GetTime();
            io.MouseDown[0] = true;
            isTouching = true;
        } else {
            if (isTouching && (ImGui::GetTime() - touchStartTime < 0.06f))
                io.MouseDown[0] = true;
            else {
                UpdateSpeedhack();
                io.MouseDown[0] = false;
                isTouching = false;
            }
        }
    }

    
    mainCamera = Camera_main();

    if (!g_MenuBound) {
        // wire the UI to the live feature globals
        ShadowBindMenu();
        ShadowMenu::hooks.openURL         = ShadowOpenURL;
        ShadowMenu::hooks.applyAimHeight  = ApplyAimHeightPreset;
        ShadowUI::CreateLogoTexture();          // embedded SHADOW MODZ logo
        ApplyAimHeightPreset(g_AimHeightIndex);
        g_MenuBound = true;
        LOGD("SHADOW MODZ: menu bound");
    }

    ApplyCyberTheme();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(g_GlWidth, g_GlHeight);
    ImGui::NewFrame();
    
    if (Enable) {
        BlackSkyUpdate();
        SpoofAllNames();
        UpdateSpinBot();
        AutoKillLogic();
        AntidevAimbotVoid();

        void* match = Curent_Match();
        if (match) {
            void* lp = GetLocalPlayer(match);
            if (lp) {
                UpdateShurikenKill(lp);
                if (LockedEnemy) CustomFireRateLogic(lp, LockedEnemy);
                AimLockLogic(lp, match);
            }
        }
    }

    
    DrawESP(g_GlWidth, g_GlHeight);
    DrawAIMBOT(g_GlWidth, g_GlHeight);

    
    // ------------------------------------------------------------------
    //  SHADOW MODZ floating menu (black + purple, draggable, scrollable)
    // ------------------------------------------------------------------
    //  the launcher on/off state is bound too, it lives in ShadowUI::theme
    ShadowMenu::Draw(g_MenuRefs, (float)g_GlWidth, (float)g_GlHeight);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return old_eglSwapBuffers(dpy, surface);
}

inline void StartGUI() {
    void* ptr = DobbySymbolResolver("/system/lib/libEGL.so", "eglSwapBuffers");
    if (ptr != nullptr) {
        DobbyHook((void*)ptr, (void*)hook_eglSwapBuffers, (void**)&old_eglSwapBuffers);
        LOGD("SHADOW MODZ GUI: hook OK");
    }
}

void hack_thread() {
    sleep(5);
    pid_t pid = getpid();
    LOGD("SHADOW MODZ hack_thread started pid=%d", pid);

    do {
        il2cpp_base = get_module_base(pid, "libil2cpp.so");
        sleep(1);
    } while (il2cpp_base == 0);

    LOGD("SHADOW MODZ il2cpp base: 0x%lx", il2cpp_base);
    Il2CppAttach();

    
    init_firewall();

    
    void* m1 = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"),
        OBFUSCATE("COW.GamePlay"), OBFUSCATE("NewPlayerAnimationSystemComponent"),
        OBFUSCATE("PlayChangeWeaponAnim"), 2);
    if (m1) DobbyHook(m1, (void*)hook_PlayChangeWeaponAnim, (void**)&orig_PlayChangeWeaponAnim);

    void* m2 = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"),
        OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("ClientUsingVersion"), 0);
    if (m2) DobbyHook(m2, (void*)hook_ClientVersion, (void**)&orig_ClientVersion);

    void* m3 = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"),
        OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_InSwapWeaponCD"), 0);
    if (m3) DobbyHook(m3, (void*)_get_InSwapWeaponCD, (void**)&get_InSwapWeaponCD);

    void* m4 = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"),
        OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsFastRunning"), 0);
    if (m4) DobbyHook(m4, (void*)hook_get_IsFastRunning, (void**)&orig_get_IsFastRunning);

    void* m5 = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"),
        OBFUSCATE("COW.GamePlay"), OBFUSCATE("UGCLevelPointLight"), OBFUSCATE("get_Range"), 0);
    if (m5) DobbyHook(m5, (void*)hook_get_Range, (void**)&orig_get_Range);

    
    void* mG1 = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"),
        OBFUSCATE("COW"), OBFUSCATE("GrenadeLine"), OBFUSCATE("Update"), 0);
    if (mG1) DobbyHook(mG1, (void*)_UpdateGranada, (void**)&UpdateGranada);

    void* mG2 = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"),
        OBFUSCATE("COW"), OBFUSCATE("GrenadeLine"), OBFUSCATE("OnShowGrenadeLineChanged"), 0);
    if (mG2) DobbyHook(mG2, (void*)_UpdateGranada, (void**)&UpdateGranada);

    
    void* mTD = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"),
        OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("TakeDamage"), 9);
    if (mTD) DobbyHook(mTD, (void*)hook_PlayerNetwork_TakeDamage,
                       (void**)&orig_PlayerNetwork_TakeDamage);

    
    void* mSil = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"),
        OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("TakeDamage"), 9);
    if (mSil) DobbyHook(mSil, (void*)BLAGCMCGEJG1, (void**)&old_BLAGCMCGEJG1);

    
    void* mMr = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"),
        OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("SetAimRotation"), 2);
    if (mMr) DobbyHook(mMr, (void*)hook_mrAimSilent, (void**)&orig_mrSilentAim);

    
    void* mFM = (void*)offset_IsFreeMove;
    if (mFM) DobbyHook(mFM, (void*)hook_IsFreeMove, (void**)&old_IsFreeMove);

    
    void* mSF = (void*)offset_PlayStopFire;
    if (mSF) DobbyHook(mSF, (void*)hook_PlayStopFire, (void**)&old_PlayStopFire);

    
    void* mPar = (void*)offset_get_IsParachuting;
    if (mPar) DobbyHook(mPar, (void*)hook_get_IsParachuting, (void**)&orig_get_IsParachuting);

    
    void* mMov = (void*)offset_IsMoving;
    if (mMov) DobbyHook(mMov, (void*)my_IsMoving_Hook_1, (void**)&orig_IsMoving_1);

    
    void* mSens = (void*)offset_GetSensitivity;
    if (mSens) DobbyHook(mSens, (void*)hook_GetSensitivity, (void**)&orig_GetSensitivity);

    
    void* mRel = (void*)offset_GetReloadScale;
    if (mRel) DobbyHook(mRel, (void*)_GetReloadScale, (void**)&orig_GetReloadScale);

    
    void* mMed = (void*)offset_get_FSModeUseMedikitFasterRate;
    if (mMed) DobbyHook(mMed, (void*)my_get_FSModeUseMedikitFasterRate,
                        (void**)&orig_get_FSModeUseMedikitFasterRate);

    
    void* mVeh = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"),
        OBFUSCATE("message"), OBFUSCATE("ProtoUtil"), OBFUSCATE("PackVector3Pos"), 1);
    if (mVeh) DobbyHook(mVeh, (void*)hook_PackVector3, (void**)&orig_PackVector3);

    
    void* mEmu = (void*)offset_IsEmulator;
    if (mEmu) DobbyHook(mEmu, (void*)_PCcheck, (void**)&PCcheck);

   
    void* mUL = (void*)offset_get_UserLevel;
    if (mUL) DobbyHook(mUL, (void*)_get_UserLevel, (void**)&get_UserLevel);

    
    MemoryPatch::createWithHex((uintptr_t)offset_AntiAddiction_IsAntiAddictActivated,
                               OBFUSCATE("C0 03 5F D6")).Modify();
    MemoryPatch::createWithHex((uintptr_t)offset_AntiAddiction_GetLogMaxAdultsLimit,
                               OBFUSCATE("C0 03 5F D6")).Modify();
    MemoryPatch::createWithHex((uintptr_t)offset_AntiAddiction_GetCurrentPlayedTimeMinutes,
                               OBFUSCATE("C0 03 5F D6")).Modify();
    MemoryPatch::createWithHex((uintptr_t)offset_AntiAddiction_GetRemainingPlayTimeMinutes,
                               OBFUSCATE("C0 03 5F D6")).Modify();

// this bypass is not tested and this would give ban so make your own bypass and add it okk !!!!! 

    PATCH_LIB("libanogs.so", "0x28ED34", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x28FD90", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x28EC28", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x295718", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x295654", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x2C7100", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x2C723C", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x2C7030", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x466EE8", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x46607C", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x222798", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x222A34", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x222AC4", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x222BF4", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x222C64", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x22D5D8", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x259150", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x257F30", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4B62E4", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4B5D18", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4B5E28", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4B6034", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4B97A4", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4A9A0C", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4A9C88", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4A9CCC", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4A9E70", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4AB264", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4AB3FC", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4AB3A4", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4AB55C", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4AB60C", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4AB684", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x47C01C", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x47C0B8", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x47C0FC", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x47C140", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1B802C", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x251164", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x2514AC", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x468964", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x468A9C", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x284710", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4A9600", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4A9484", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1B8C98", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1B5AE0", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1BD39C", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1BAE8C", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1B89A8", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1BC980", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1BC9E4", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1BCA14", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1BCDEC", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1BC0C0", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1B796C", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1B25BC", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1B3280", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1B6CCC", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1BB1DC", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1B5148", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1B40A0", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1EC610", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1FF8C0", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1FFCE4", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x284658", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x2C723C", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x2591F8", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x259258", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x28FD90", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4D9238", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4D9A00", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4DA1D4", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4DDD40", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4DE640", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4DEF40", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4DF840", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4D9120", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4D9C3C", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4DA76C", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x256FBC", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x257024", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x2C6C7C", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x284420", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x2845C0", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x2CD8F0", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x2CD948", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x2CD980", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x2CE0CC", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x2CF998", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x234284", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x20388C", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4FADD0", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4FAE80", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4FAE40", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C37AC", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C3AD8", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C54B4", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C7864", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C78D0", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C7F5C", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C5A20", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C793C", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C8264", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C61E4", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C8884", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C6E40", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C6530", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C7330", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C8C0C", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C4114", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C4518", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C4BA4", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C4FC8", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1C8FBC", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x497580", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x497494", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x497544", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x494FFC", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4942C4", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4942D8", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4ABCE0", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4E0140", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4FCD8C", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x45CBEC", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x465DC4", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1BDA8C", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1BD9FC", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1B9C58", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4A94BC", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4A9544", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4A95A0", "00 00 80 52 C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4ABB14", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4ABB70", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4ABBF4", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4AB9F8", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4982B0", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4977F4", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4976F0", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x497714", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x49755C", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x497A64", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x49836C", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x494450", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x4944E8", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x49456C", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x494588", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x20A774", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1CFDE8", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x1BE1E0", "C0 03 5F D6");
    PATCH_LIB("libanogs.so", "0x221744", "C0 03 5F D6");
    
    std::thread(StartAutoRevive).detach();
    std::thread(StartGlider).detach();
    std::thread(StartAimbotVisible).detach();

    StartGUI();
    LOGD("SHADOW MODZ: all hooks installed, GUI started");
}
