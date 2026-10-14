#pragma once
// ============================================================================
//  SHADOW MODZ - FLOATING MENU
//  ---------------------------------------------------------------------------
//  Complete rebuild of the in-game floating UI:
//    * draggable round launcher (brand logo) that snaps to the screen edge
//    * draggable, resizable black + purple glass window
//    * scrollable nav rail + scrollable content area with flick/touch scroll
//    * every original feature kept, regrouped into cards
//    * presets, accent picker, UI scaling, live firewall console, telegram
//
//  Telegram : https://t.me/+BBimnHMiSvpiYTBl
// ============================================================================

#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "ShadowTheme.hpp"

namespace ShadowMenu
{

using namespace ShadowUI;

// ---------------------------------------------------------------------------
//  branding
// ---------------------------------------------------------------------------
static const char* kBrand        = "SHADOW MODZ";
static const char* kBrandUpper   = "SHADOW MODZ";
static const char* kVersion      = "v4.0";
static const char* kEdition      = "PREMIUM EDITION";
static const char* kTelegram     = "https://t.me/+BBimnHMiSvpiYTBl";
static const char* kTelegramNice = "t.me/+BBimnHMiSvpiYTBl";

// ---------------------------------------------------------------------------
//  feature bindings - the menu only ever writes through these pointers so the
//  UI can be compiled / previewed without the whole game hook chain.
// ---------------------------------------------------------------------------
#define SHADOW_BOOL_FIELDS(X)                    \
    X(Enable,             Enable)                \
    X(Aimbot,             Aimbot)                \
    X(SilentAim,          SilentAim)             \
    X(AimSilent,          AimSilent)             \
    X(Headshot,           Headshot)              \
    X(AimVisible2,        AimVisible2)           \
    X(AimKill1,           AimKill1)              \
    X(AutoKill,           AutoKill)              \
    X(AimLock,            AimLock)               \
    X(AimLockWall,        AimLockThroughWall)    \
    X(CustomFireRate,     CustomFireRate)        \
    X(SpinBot,            SpinBot)               \
    X(AimVisibleThread,   aimVisibleEnabled)     \
    X(Antidev,            ANTIDEV)               \
    X(EspLine,            Config.ESP.Line)       \
    X(EspBox,             Config.ESP.Box)        \
    X(EspBox3d,           Config.ESP.Box3d)      \
    X(EspBoxFilled,       Config.ESP.BoxFilled)  \
    X(EspBoxHalf,         Config.ESP.BoxHalfFilled) \
    X(EspHealth,          Config.ESP.Health)     \
    X(EspIdentity,        Config.ESP.MAFIA)      \
    X(EspDistance,        Config.ESP.Distance)   \
    X(EspBones,           EspSkeleton2)          \
    X(EspFovCircle,       DrawFov)               \
    X(SniperAlert,        SniperAlert)           \
    X(EspGrenade,         ESPGrenade)            \
    X(NightMode,          EnableNightMode)       \
    X(SpeedHack,          SpeedHack)             \
    X(SpeedRun,           speedrun)              \
    X(FastSwitch,         Fastswitch)            \
    X(AutoRun,            AutoRun)               \
    X(InfiniteRange,      InfiniteRange)         \
    X(NoRecoil,           norecoil)              \
    X(SniperFast,         SniperFast)            \
    X(PcSwapAnim,         PcSwapAnim)            \
    X(SetIndia,           Setindia)              \
    X(CameraHeight,       camxa)                 \
    X(GuestReset,         Reset)                 \
    X(FreeMove,           FreeMove)              \
    X(StopFireAnim,       StopFireAnim)          \
    X(Parachuting,        isParachuting)         \
    X(MedkitMoving,       MedkitMoving)          \
    X(FastSensitivity,    FastSensivity)         \
    X(QuickReload,        QuickReloadLakiyaxMods)\
    X(ShurikenKill,       ShurikenKill)          \
    X(MedkitFasterRate,   FUCKMODI)              \
    X(UserLevelSpoof,     LUNDUP)                \
    X(AutoRevive,         autoReviveEnabled)     \
    X(GliderHack,         gliderHackEnabled)     \
    X(GliderInfinite,     gliderInfiniteEnabled) \
    X(SpoofName,          SpoofNameEnabled)      \
    X(MassKill,           Mass.Sucks)            \
    X(BypassLogin400,     g_LoginBypass400)      \
    X(FirewallOn,         g_FirewallEnabled)     \
    X(FirewallVerbose,    g_FirewallVerbose)     \
    X(Watermark,          RenderWatermark)

#define SHADOW_FLOAT_FIELDS(X)                        \
    X(FovAim,            Fov_Aim)                     \
    X(AimDistance,       Aimdis)                      \
    X(AimSpeed,          AimSpeed)                    \
    X(AimKillFov,        AimKillFov)                  \
    X(AimKillDistance,   AimKillDistance)             \
    X(AutoKillDelay,     AutoKillDelayMs)             \
    X(AimLockFov,        AimLockFov)                  \
    X(AimLockSpeed,      AimLockSpeed)                \
    X(FireRate,          FireRateMs)                  \
    X(ShotsPerBurst,     ShotsPerBurst)               \
    X(SpinBotSpeed,      SpinBotSpeed)                \
    X(LineThickness,     LineThickness)               \
    X(BoxThickness,      BoxThickness)                \
    X(BoxRoundness,      BoxRoundness)                \
    X(SpeedMultiplier,   active)                      \
    X(MassKillRange,     Mass.Sucks1)                 \
    X(MassKillOffset,    Mass.FrontOffset)            \
    X(Sensitivity,       Sensivity)                   \
    X(MedkitRate,        FUCKMODIRate)                \
    X(AiFov,             AiFov)

#define SHADOW_INT_FIELDS(X)                 \
    X(AimWhen,          AimWhen)             \
    X(LineMode,         LineMode)            \
    X(BoxStyle,         BoxStyle)            \
    X(CarYOffset,       DONTCarValue)        \
    X(AimHeight,        g_AimHeightIndex)

struct Refs
{

#define SHADOW_DECL_BOOL(name, target)   bool*  name = nullptr;
#define SHADOW_DECL_FLOAT(name, target)  float* name = nullptr;
#define SHADOW_DECL_INT(name, target)    int*   name = nullptr;

    SHADOW_BOOL_FIELDS(SHADOW_DECL_BOOL)
    SHADOW_FLOAT_FIELDS(SHADOW_DECL_FLOAT)
    SHADOW_INT_FIELDS(SHADOW_DECL_INT)

#undef SHADOW_DECL_BOOL
#undef SHADOW_DECL_FLOAT
#undef SHADOW_DECL_INT
};

// ---------------------------------------------------------------------------
//  hooks back into the mod (URL opening, aim height presets, ...)
// ---------------------------------------------------------------------------
struct Hooks
{
    void (*openURL)(const char*) = nullptr;
    void (*applyAimHeight)(int)  = nullptr;
};
inline Hooks hooks;

// ---------------------------------------------------------------------------
//  persistent menu state
// ---------------------------------------------------------------------------
inline bool  open        = true;
inline int   tab         = 0;
inline float launcherX   = 70.0f;
inline float launcherY   = 240.0f;
inline float menuX       = 30.0f;
inline float menuY       = 40.0f;
inline bool  positioned  = false;

inline bool  launcherDrag = false;
inline float launcherOffX = 0.0f;
inline float launcherOffY = 0.0f;
inline float launcherPressX = 0.0f;   // where the finger went down
inline float launcherPressY = 0.0f;
inline float launcherGrabT = 0.0f;
inline float snapTargetX = 0.0f;
inline float pressAnim   = 0.0f;

inline char  toastText[96] = { 0 };
inline float toastUntil    = 0.0f;

inline void Toast(const char* msg)
{
    snprintf(toastText, sizeof(toastText), "%s", msg ? msg : "");
    toastUntil = ImGui::GetTime() + 2.2f;
}

// ---------------------------------------------------------------------------
//  presets
// ---------------------------------------------------------------------------
struct PresetSlot
{
    bool used = false;
    std::vector<float> bools;
    std::vector<float> floats;
    std::vector<float> ints;
};
inline PresetSlot slots[3];
inline const char* kSlotNames[3] = { "SLOT A", "SLOT B", "SLOT C" };

inline void SavePreset(Refs& R, int slot)
{
    if (slot < 0 || slot > 2) return;
    PresetSlot& p = slots[slot];
    p.bools.clear(); p.floats.clear(); p.ints.clear();

#define SHADOW_SAVE_BOOL(name, target)  p.bools.push_back(*(R.name) ? 1.0f : 0.0f);
#define SHADOW_SAVE_FLOAT(name, target) p.floats.push_back(*(R.name));
#define SHADOW_SAVE_INT(name, target)   p.ints.push_back((float)*(R.name));
    SHADOW_BOOL_FIELDS(SHADOW_SAVE_BOOL)
    SHADOW_FLOAT_FIELDS(SHADOW_SAVE_FLOAT)
    SHADOW_INT_FIELDS(SHADOW_SAVE_INT)
#undef SHADOW_SAVE_BOOL
#undef SHADOW_SAVE_FLOAT
#undef SHADOW_SAVE_INT

    p.used = true;
    Toast("PRESET SAVED");
}

inline bool LoadPreset(Refs& R, int slot)
{
    if (slot < 0 || slot > 2 || !slots[slot].used) return false;
    PresetSlot& p = slots[slot];
    size_t bi = 0, fi = 0, ii = 0;

#define SHADOW_LOAD_BOOL(name, target)  if (bi < p.bools.size())  *(R.name) = p.bools[bi++]  > 0.5f;
#define SHADOW_LOAD_FLOAT(name, target) if (fi < p.floats.size()) *(R.name) = p.floats[fi++];
#define SHADOW_LOAD_INT(name, target)   if (ii < p.ints.size())   *(R.name) = (int)p.ints[ii++];
    SHADOW_BOOL_FIELDS(SHADOW_LOAD_BOOL)
    SHADOW_FLOAT_FIELDS(SHADOW_LOAD_FLOAT)
    SHADOW_INT_FIELDS(SHADOW_LOAD_INT)
#undef SHADOW_LOAD_BOOL
#undef SHADOW_LOAD_FLOAT
#undef SHADOW_LOAD_INT

    if (hooks.applyAimHeight) hooks.applyAimHeight(*(R.AimHeight));
    Toast("PRESET LOADED");
    return true;
}

// ---------------------------------------------------------------------------
//  helpers
// ---------------------------------------------------------------------------
inline int EnabledCount(Refs& R)
{
    int n = 0;
#define SHADOW_COUNT_BOOL(name, target) if (R.name && *(R.name)) ++n;
    SHADOW_BOOL_FIELDS(SHADOW_COUNT_BOOL)
#undef SHADOW_COUNT_BOOL
    return n;
}

inline bool TabActive(Refs& R, int t)
{
    switch (t)
    {
        case 0: return (R.Enable && *R.Enable) || (R.Aimbot && *R.Aimbot) || (R.AutoKill && *R.AutoKill) || (R.AimLock && *R.AimLock);
        case 1: return (R.EspBox && *R.EspBox) || (R.EspLine && *R.EspLine) || (R.EspBones && *R.EspBones) || (R.EspHealth && *R.EspHealth);
        case 2: return (R.SpeedHack && *R.SpeedHack) || (R.FreeMove && *R.FreeMove) || (R.SpinBot && *R.SpinBot) || (R.AutoRevive && *R.AutoRevive);
        case 3: return R.FirewallOn && *R.FirewallOn;
        case 4: return false;
        default: return false;
    }
}

inline void SmallText(ImDrawList* dl, ImVec2 p, ImVec4 col, const char* txt, float scale = 0.82f)
{
    if (!dl) return;
    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize() * scale, p, U32(col), txt);
}

// ---------------------------------------------------------------------------
//  launcher (round floating button carrying the brand logo)
//  hit-tested directly instead of through an ImGui window: a full screen
//  invisible window would swallow every click meant for the menu, this way the
//  launcher stays on top visually while clicks over the menu still reach it.
// ---------------------------------------------------------------------------
inline ImVec4 menuRect(-1.0f, -1.0f, -1.0f, -1.0f);   // last drawn menu bounds

inline bool PointInMenu(ImVec2 p)
{
    if (!open) return false;
    return p.x >= menuRect.x && p.x <= menuRect.z && p.y >= menuRect.y && p.y <= menuRect.w;
}

inline void LauncherInput(ImVec2 screen)
{
    if (!theme.showLauncher) return;

    const float R = S(theme.launcherRadius);
    launcherX = ImClamp(launcherX, R * 0.6f, ImMax(R * 0.6f, screen.x - R * 0.6f));
    launcherY = ImClamp(launcherY, R * 0.6f, ImMax(R * 0.6f, screen.y - R * 0.6f));

    ImGuiIO& io = ImGui::GetIO();
    const ImVec2 m = io.MousePos;

    // ignore anything that happens inside the open menu window
    const bool overMenu   = PointInMenu(m);
    const bool blockInput = overMenu || ImGui::IsAnyItemActive();

    const ImVec2 a(launcherX - R, launcherY - R);
    const ImVec2 b(launcherX + R, launcherY + R);
    const bool over = !blockInput && m.x >= a.x && m.x <= b.x && m.y >= a.y && m.y <= b.y;

    if (!blockInput && ImGui::IsMouseClicked(0) && over)
    {
        launcherDrag   = true;
        launcherGrabT  = ImGui::GetTime();
        launcherOffX   = launcherX - m.x;
        launcherOffY   = launcherY - m.y;
        launcherPressX = m.x;
        launcherPressY = m.y;
    }

    if (launcherDrag && ImGui::IsMouseDown(0))
    {
        launcherX = m.x + launcherOffX;
        launcherY = m.y + launcherOffY;
    }

    if (launcherDrag && ImGui::IsMouseReleased(0))
    {
        launcherDrag = false;
        // a tap only counts when the finger stayed put - the button follows the
        // pointer 1:1 while dragging, so measuring against the button centre
        // would wrongly report "tap" on every drag.
        const float dx = m.x - launcherPressX;
        const float dy = m.y - launcherPressY;
        const bool  tap = (sqrtf(dx * dx + dy * dy) < ImMax(S(10.0f), R * 0.42f)) &&
                          (ImGui::GetTime() - launcherGrabT) < 0.45f;
        if (tap)
            open = !open;
        else
            snapTargetX = (launcherX < screen.x * 0.5f) ? S(16.0f) + R * 0.4f
                                                        : screen.x - S(16.0f) - R * 0.4f;
    }

    // edge snap animation
    if (fabsf(snapTargetX - launcherX) > 0.5f)
    {
        launcherX += (snapTargetX - launcherX) * ImClamp(io.DeltaTime * 9.0f, 0.0f, 1.0f);
        launcherX = ImClamp(launcherX, R * 0.6f, ImMax(R * 0.6f, screen.x - R * 0.6f));
    }
    else
    {
        snapTargetX = launcherX;
    }

    pressAnim += ((launcherDrag && over) ? 1.0f : 0.0f) * ImClamp(io.DeltaTime * 12.0f, 0.0f, 1.0f);
    pressAnim = ImClamp(pressAnim, 0.0f, 1.0f);
}

inline void LauncherVisuals(ImDrawList* dl, ImVec2 screen)
{
    if (!theme.showLauncher || !dl) return;

    const float R = S(theme.launcherRadius) * (1.0f + pressAnim * 0.06f);
    const ImVec2 c(launcherX, launcherY);
    const float t = (float)ImGui::GetTime();

    // ambient glow
    GlowCircle(dl, c, R, pal.accent, (0.9f + 0.25f * sinf(t * 2.2f)) * theme.glowStrength, 4);

    // rotating dashed ring
    const int   seg   = 26;
    const float step  = (2.0f * IM_PI) / (float)seg;
    const float phase = t * 1.1f;
    for (int i = 0; i < seg; ++i)
    {
        if ((i % 3) == 0) continue;
        const float a0 = phase + (float)i * step;
        const float a1 = a0 + step * 0.55f;
        dl->PathArcTo(c, R + S(4.5f), a0, a1, 8);
        dl->PathStroke(U32(pal.accentHi, 0.75f), 0, S(1.8f));
    }

    // base disc
    dl->AddCircleFilled(c, R, U32(ImVec4(0.043f, 0.035f, 0.063f, 0.94f)), 48);
    dl->AddCircleFilled(c, R * 0.995f, U32(pal.accentDeep, 0.28f), 48);

    if (open)
    {
        // close glyph
        const float k = R * 0.34f;
        dl->AddLine(ImVec2(c.x - k, c.y - k), ImVec2(c.x + k, c.y + k), U32(pal.accentHi), S(2.6f));
        dl->AddLine(ImVec2(c.x - k, c.y + k), ImVec2(c.x + k, c.y - k), U32(pal.accentHi), S(2.6f));
    }
    else
    {
        LogoDraw(dl, c, R * 1.62f, R * 0.81f, false);
    }

    dl->AddCircle(c, R, U32(pal.accent, 0.95f), 48, S(2.0f));
    dl->AddCircle(c, R - S(4.0f), U32(pal.pink, 0.35f), 48, S(1.2f));

    // notification dot while the menu is closed and modules are active
    if (!open)
    {
        dl->AddCircleFilled(ImVec2(c.x + R * 0.72f, c.y - R * 0.72f), S(4.5f), U32(pal.pink), 16);
    }
}

// ---------------------------------------------------------------------------
//  window header / status bar
// ---------------------------------------------------------------------------
inline void DrawHeader(Refs& R, ImDrawList* dl, ImVec2 wp, ImVec2 ws, float headerH)
{
    const float padX = S(14.0f);

    // bar background
    dl->AddRectFilledMultiColor(ImVec2(wp.x, wp.y), ImVec2(wp.x + ws.x, wp.y + headerH),
                                U32(pal.accentDeep, 0.55f), U32(pal.bgCardHi, 0.85f),
                                U32(pal.bgWindow, 0.30f), U32(pal.bgWindow, 0.30f));
    Separator(dl, ImVec2(wp.x, wp.y + headerH), ImVec2(wp.x + ws.x, wp.y + headerH), pal.border);

    // logo
    const float ls = S(42.0f);
    LogoDraw(dl, ImVec2(wp.x + padX + ls * 0.5f, wp.y + headerH * 0.5f), ls, ls * 0.5f, false);

    // title / subtitle
    const float tx = wp.x + padX + ls + S(12.0f);
    const float titleSize = ImGui::GetFontSize() * 1.22f;
    dl->AddText(ImGui::GetFont(), titleSize, ImVec2(tx, wp.y + S(9.0f)), U32(pal.text), kBrandUpper);
    char sub[128];
    snprintf(sub, sizeof(sub), "%s  •  %s", kEdition, kTelegramNice);
    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 0.78f, ImVec2(tx, wp.y + S(32.0f)),
                U32(pal.accentHi), sub);

    // telegram pill (tap to open)
    ImGui::SetCursorScreenPos(ImVec2(wp.x + ws.x - S(146.0f), wp.y + S(16.0f)));
    if (Button("TELEGRAM", ICON_FA_PAPER_PLANE, ImVec2(S(96.0f), S(30.0f)), true))
    {
        if (hooks.openURL) hooks.openURL(kTelegram);
        Toast("OPENING TELEGRAM");
    }

    // minimise + close
    const float bs = S(26.0f);
    ImGui::SetCursorScreenPos(ImVec2(wp.x + ws.x - S(42.0f), wp.y + S(16.0f)));
    if (IconButton("##min", ICON_FA_MINUS, ImVec2(bs, S(30.0f)), pal.textSoft))
        open = false;

    (void)R;

    // drag handle (submitted last so the buttons above keep priority)
    static ImVec2 grab(0, 0);
    ImGui::SetCursorScreenPos(ImVec2(wp.x + S(4.0f), wp.y + S(4.0f)));
    ImGui::InvisibleButton("##shadowmodz_drag", ImVec2(ws.x - S(160.0f), headerH - S(8.0f)));
    if (ImGui::IsItemActive())
    {
        if (ImGui::IsItemActivated())
        {
            ImVec2 m = ImGui::GetIO().MousePos;
            grab = ImVec2(m.x - menuX, m.y - menuY);
        }
        ImVec2 m = ImGui::GetIO().MousePos;
        menuX = m.x - grab.x;
        menuY = m.y - grab.y;
    }
}

inline void DrawStatusBar(Refs& R, ImDrawList* dl, ImVec2 wp, ImVec2 ws, float statusH)
{
    const float y = wp.y + ws.y - statusH;
    dl->AddRectFilled(ImVec2(wp.x, y), ImVec2(wp.x + ws.x, wp.y + ws.y),
                      U32(ImVec4(0.031f, 0.024f, 0.047f, 0.92f * theme.windowOpacity)));
    Separator(dl, ImVec2(wp.x, y), ImVec2(wp.x + ws.x, y), pal.border);

    char fps[48];
    snprintf(fps, sizeof(fps), "%.0f FPS", ImGui::GetIO().Framerate);
    const float fs = ImGui::GetFontSize() * 0.80f;

    if (theme.showFps)
    {
        dl->AddText(ImGui::GetFont(), fs, ImVec2(wp.x + S(12.0f), y + S(7.0f)), U32(pal.ok), fps);
    }

    char mods[64];
    snprintf(mods, sizeof(mods), "%d MODULES ACTIVE", EnabledCount(R));
    dl->AddText(ImGui::GetFont(), fs, ImVec2(wp.x + S(84.0f), y + S(7.0f)), U32(pal.accentHi), mods);

    float tw = ImGui::CalcTextSize(kTelegramNice).x * 0.80f;
    dl->AddText(ImGui::GetFont(), fs, ImVec2(wp.x + ws.x * 0.5f - tw * 0.5f, y + S(7.0f)),
                U32(pal.textDim), kTelegramNice);

    dl->AddText(ImGui::GetFont(), fs, ImVec2(wp.x + ws.x - S(86.0f), y + S(7.0f)), U32(pal.textDim), kVersion);

    // resize grip
    ImGui::SetCursorScreenPos(ImVec2(wp.x + ws.x - S(26.0f), y + S(4.0f)));
    ImGui::InvisibleButton("##shadowmodz_resize", ImVec2(S(24.0f), S(20.0f)));
    SetLastItem();
    if (ImGui::IsItemActive())
    {
        ImVec2 d = ImGui::GetIO().MouseDelta;
        theme.menuWidth  = ImClamp(theme.menuWidth  + d.x, S(560.0f), ImMin(1180.0f, ImGui::GetIO().DisplaySize.x - S(16.0f)));
        theme.menuHeight = ImClamp(theme.menuHeight + d.y, S(360.0f), ImMin(860.0f,  ImGui::GetIO().DisplaySize.y - S(16.0f)));
    }
    ImU32 gc = ImGui::IsItemActive() ? U32(pal.accentHi) : U32(pal.textDim, 0.7f);
    for (int i = 1; i <= 3; ++i)
        dl->AddLine(ImVec2(wp.x + ws.x - S(6.0f) - (float)i * S(4.0f), wp.y + ws.y - S(6.0f)),
                    ImVec2(wp.x + ws.x - S(6.0f), wp.y + ws.y - S(6.0f) - (float)i * S(4.0f)), gc, S(1.2f));
}

// ---------------------------------------------------------------------------
//  navigation rail
// ---------------------------------------------------------------------------
struct NavItem { const char* label; const char* icon; int tab; };

inline void DrawNav(Refs& R, ImVec2 size)
{
    ImGui::BeginChild("##shadowmodz_nav", size, false);
    TouchScroll(true);

    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 wp = ImGui::GetWindowPos();

    static const NavItem items[] = {
        { "AIMBOT",   ICON_FA_CROSSHAIRS, 0 },
        { "VISUAL",   ICON_FA_EYE,        1 },
        { "MISC",     ICON_FA_FIRE,       2 },
        { "NETWORK",  ICON_FA_SHIELD_ALT,     3 },
        { "SETTINGS", ICON_FA_COG,       4 },
    };

    Spacer(6.0f);
    SectionTitle("MODULES", ICON_FA_BARS);
    Spacer(2.0f);

    for (int i = 0; i < IM_ARRAYSIZE(items); ++i)
    {
        const NavItem& it = items[i];
        ImGui::PushID(it.label);

        const float h = S(40.0f);
        const float w = ImGui::GetContentRegionAvail().x;
        ImVec2 p = ImGui::GetCursorScreenPos();

        const bool tapped = ImGui::InvisibleButton(it.label, ImVec2(w, h));
        const bool hovered = ImGui::IsItemHovered();
        SetLastItem();
        if (tapped) tab = it.tab;

        const bool sel = (tab == it.tab);
        float& anim = AnimRef(ImGui::GetID("##navanim"), sel ? 1.0f : 0.0f);
        anim += ((sel ? 1.0f : 0.0f) - anim) * AnimSpeed();

        if (anim > 0.02f)
        {
            GradientH(dl, p, ImVec2(p.x + w, p.y + h),
                      U32(pal.accentDeep, 0.55f * anim), U32(pal.accent, 0.16f * anim), S(9.0f));
            dl->AddRect(p, ImVec2(p.x + w, p.y + h), U32(pal.accent, 0.45f * anim), S(9.0f), 0, S(1.0f));
            dl->AddRectFilled(ImVec2(p.x + S(1.0f), p.y + S(6.0f)), ImVec2(p.x + S(4.0f), p.y + h - S(6.0f)),
                              U32(pal.accent, anim), S(1.5f));
        }
        else if (hovered)
        {
            dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), U32(pal.rowHover, 0.75f), S(9.0f));
        }

        ImVec4 icol = anim > 0.5f ? pal.accentHi : (hovered ? pal.textSoft : pal.textDim);
        dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(p.x + S(14.0f), p.y + S(11.0f)), U32(icol), it.icon);
        dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(p.x + S(42.0f), p.y + S(11.0f)),
                    U32(anim > 0.5f ? pal.text : pal.textSoft), it.label);

        if (TabActive(R, it.tab))
            dl->AddCircleFilled(ImVec2(p.x + w - S(14.0f), p.y + h * 0.5f), S(3.5f), U32(pal.ok), 12);

        if (sel)
            dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(p.x + w - S(20.0f), p.y + S(11.0f)),
                        U32(pal.accentHi), ICON_FA_CHEVRON_RIGHT);

        ImGui::Dummy(ImVec2(1.0f, S(4.0f)));
        ImGui::PopID();
    }

    Spacer(4.0f);
    SectionTitle("COMMUNITY", ICON_FA_PAPER_PLANE);
    Spacer(4.0f);

    // telegram card
    {
        const float w = ImGui::GetContentRegionAvail().x;
        ImVec2 p = ImGui::GetCursorScreenPos();
        const float h = S(78.0f);
        const bool tapped = ImGui::InvisibleButton("##tgcard", ImVec2(w, h));
        const bool hovered = ImGui::IsItemHovered();
        SetLastItem();
        if (tapped)
        {
            if (hooks.openURL) hooks.openURL(kTelegram);
            Toast("OPENING TELEGRAM");
        }
        dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), U32(pal.bgCardHi, 0.85f), S(10.0f));
        dl->AddRect(p, ImVec2(p.x + w, p.y + h), U32(pal.accent, hovered ? 0.85f : 0.45f), S(10.0f), 0, S(1.0f));
        dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(p.x + S(12.0f), p.y + S(11.0f)),
                    U32(pal.accentHi), ICON_FA_PAPER_PLANE);
        dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(p.x + S(38.0f), p.y + S(11.0f)),
                    U32(pal.text), "JOIN TELEGRAM");
        dl->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 0.74f, ImVec2(p.x + S(12.0f), p.y + S(37.0f)),
                    U32(pal.textDim), "updates / keys / support");
        dl->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 0.78f, ImVec2(p.x + S(12.0f), p.y + S(55.0f)),
                    U32(pal.accent), kTelegramNice);
        ImGui::Dummy(ImVec2(1.0f, S(6.0f)));
    }

    Spacer(4.0f);
    DividerText(kVersion);
    ImVec2 cp = ImGui::GetCursorScreenPos();
    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 0.78f, ImVec2(cp.x + S(4.0f), cp.y),
                U32(pal.textDim), "SHADOW MODZ TEAM");
    Spacer(18.0f);

    ImGui::EndChild();
}

// ---------------------------------------------------------------------------
//  tabs
// ---------------------------------------------------------------------------
inline void TabAimbot(Refs& R)
{
    CardBegin("aimbot_master", "AIM ASSIST CORE", ICON_FA_CROSSHAIRS, "MASTER");
    ToggleRow("Master Enable",       R.Enable,          "core");
    ToggleRow("Aimbot",              R.Aimbot,          "legit");
    ToggleRow("Silent Aim",          R.SilentAim);
    ToggleRow("Aim Silent (hook)",   R.AimSilent);
    ToggleRow("Headshot",            R.Headshot,        "always");
    ToggleRow("Aim Visible Check",   R.AimVisible2);
    ToggleRow("Aim Visible Thread",  R.AimVisibleThread);
    ToggleRow("Anti Dev Aim",        R.Antidev);
    CardEnd();
    Spacer(2.0f);

    CardBegin("aimbot_auto", "AUTO TARGET", ICON_FA_MAGIC, "DANGER", pal.warn);
    ToggleRow("AimKill",             R.AimKill1);
    ToggleRow("AutoKill",            R.AutoKill);
    ToggleRow("AimLock",             R.AimLock);
    ToggleRow("AimLock Through Wall",R.AimLockWall);
    ToggleRow("Custom Fire Rate",    R.CustomFireRate);
    ToggleRow("SpinBot",             R.SpinBot);
    CardEnd();
    Spacer(2.0f);

    CardBegin("aimbot_tune", "TARGETING", ICON_FA_SLIDERS_H);
    SliderRow("Target FOV",     R.FovAim,         0.0f,   360.0f, "%.0f deg", "radius");
    SliderRow("Max Distance",   R.AimDistance,    0.0f,   500.0f, "%.0f m",   "range");
    SliderRow("Aim Speed",      R.AimSpeed,       0.01f,  1.00f,  "%.2f",     "smooth");
    SliderRow("AimKill FOV",    R.AimKillFov,     0.0f,   800.0f, "%.0f");
    SliderRow("AimKill Dist",   R.AimKillDistance,0.0f,   300.0f, "%.0f m");
    SliderRow("AutoKill Delay", R.AutoKillDelay,  10.0f,  1000.0f,"%.0f ms");
    SliderRow("AimLock FOV",    R.AimLockFov,     0.0f,   800.0f, "%.0f");
    SliderRow("AimLock Speed",  R.AimLockSpeed,   0.01f,  1.00f,  "%.2f");
    SliderRow("Fire Rate",      R.FireRate,       20.0f,  1000.0f,"%.0f ms");
    SliderRow("Shots / Burst",  R.ShotsPerBurst,  1.0f,   10.0f,  "%.0f");
    SliderRow("SpinBot Speed",  R.SpinBotSpeed,   100.0f, 2000.0f,"%.0f");
    CardEnd();
    Spacer(2.0f);

    CardBegin("aimbot_pref", "AIM BEHAVIOUR", ICON_FA_USER_SECRET);
    static const char* heights[] = { "35%", "50%", "75%", "90%", "100%" };
    if (SelectRow("Aim Height", R.AimHeight, heights, IM_ARRAYSIZE(heights)))
        if (hooks.applyAimHeight) hooks.applyAimHeight(*(R.AimHeight));

    static const char* whens[] = { "Always", "When Firing", "When Scope" };
    SelectRow("Aim When", R.AimWhen, whens, IM_ARRAYSIZE(whens));
    SectionTitle("INFO", ICON_FA_IMAGE);
    InfoRow("Mode", *(R.AimWhen) == 0 ? "ALWAYS" : (*(R.AimWhen) == 1 ? "FIRING" : "SCOPE"), pal.accentHi);
    InfoRow("Target slot", "HEAD / NECK", pal.textSoft);
    CardEnd();
}

inline void TabVisual(Refs& R)
{
    CardBegin("visual_players", "PLAYER ESP", ICON_FA_EYE, "LIVE", pal.ok);
    ToggleRow("ESP Line",          R.EspLine);
    ToggleRow("ESP Box 2D",        R.EspBox);
    ToggleRow("ESP Box 3D",        R.EspBox3d);
    ToggleRow("ESP Box Filled",    R.EspBoxFilled);
    ToggleRow("ESP Box Half",      R.EspBoxHalf);
    ToggleRow("Health Bar",        R.EspHealth);
    ToggleRow("Identity Text",     R.EspIdentity);
    ToggleRow("Distance Text",     R.EspDistance);
    ToggleRow("Bone ESP",          R.EspBones);
    CardEnd();
    Spacer(2.0f);

    CardBegin("visual_overlay", "OVERLAYS", ICON_FA_GHOST);
    ToggleRow("Draw FOV Circle",   R.EspFovCircle);
    ToggleRow("Sniper Alert",      R.SniperAlert);
    ToggleRow("ESP Grenade",       R.EspGrenade);
    ToggleRow("Night Mode",        R.NightMode);
    CardEnd();
    Spacer(2.0f);

    CardBegin("visual_style", "ESP STYLE", ICON_FA_SLIDERS_H);
    SliderRow("Line Thickness", R.LineThickness, 0.5f, 5.0f, "%.1f");
    SliderRow("Box Thickness",  R.BoxThickness,  0.5f, 5.0f, "%.1f");
    SliderRow("Box Roundness",  R.BoxRoundness,  0.0f, 15.0f, "%.1f");
    static const char* lineModes[] = { "Bottom", "Middle", "Top", "Left", "Right" };
    SelectRow("Line Mode", R.LineMode, lineModes, IM_ARRAYSIZE(lineModes));
    static const char* boxStyles[] = {
        "Normal", "Corner", "3D", "Filled", "Dashed", "Gradient",
        "Shadow", "Glow", "Double", "X", "Outline", "HealthBar"
    };
    SelectRow("Box Style", R.BoxStyle, boxStyles, IM_ARRAYSIZE(boxStyles));
    CardEnd();
}

inline void TabMisc(Refs& R)
{
    CardBegin("misc_movement", "MOVEMENT", ICON_FA_BOLT);
    ToggleRow("Speed Hack",        R.SpeedHack);
    ToggleRow("Speed Run",         R.SpeedRun);
    ToggleRow("Fast Switch",       R.FastSwitch);
    ToggleRow("Auto Run",          R.AutoRun);
    ToggleRow("Free Move",         R.FreeMove);
    ToggleRow("Fast Sensitivity",  R.FastSensitivity);
    ToggleRow("Parachuting",       R.Parachuting);
    CardEnd();
    Spacer(2.0f);

    CardBegin("misc_combat", "COMBAT / EXPLOITS", ICON_FA_FIRE, "RISK", pal.warn);
    ToggleRow("No Recoil",         R.NoRecoil);
    ToggleRow("Infinite Range",    R.InfiniteRange);
    ToggleRow("Sniper Fast",       R.SniperFast);
    ToggleRow("PC Swap Anim",      R.PcSwapAnim);
    ToggleRow("Shuriken Kill",     R.ShurikenKill);
    ToggleRow("Quick Reload",      R.QuickReload);
    ToggleRow("Medkit Faster Rate",R.MedkitFasterRate);
    ToggleRow("Medkit Moving",     R.MedkitMoving);
    ToggleRow("Stop Fire Anim",    R.StopFireAnim);
    ToggleRow("Mass Kill",         R.MassKill);
    CardEnd();
    Spacer(2.0f);

    CardBegin("misc_account", "PLAYER / ACCOUNT", ICON_FA_USER_SECRET);
    ToggleRow("Camera Height",     R.CameraHeight);
    ToggleRow("Set India",         R.SetIndia);
    ToggleRow("Guest Reset",       R.GuestReset);
    ToggleRow("Spoof Name",        R.SpoofName);
    ToggleRow("User Level Spoof",  R.UserLevelSpoof);
    ToggleRow("Auto Revive",       R.AutoRevive);
    ToggleRow("Glider Hack",       R.GliderHack);
    ToggleRow("Glider Infinite",   R.GliderInfinite);
    ToggleRow("Bypass Login 400",  R.BypassLogin400);
    CardEnd();
    Spacer(2.0f);

    CardBegin("misc_values", "VALUES", ICON_FA_SLIDERS_H);
    SliderRow("Speed Multiplier", R.SpeedMultiplier, 0.0f,   5.0f, "%.2f");
    SliderRow("Mass Kill Range",  R.MassKillRange,   1.0f,   30.0f,"%.1f m");
    SliderRow("Mass Kill Offset", R.MassKillOffset, -1.0f,   3.0f, "%.2f", "front");
    IntSliderRow("Car Y Offset",  R.CarYOffset,    -20,     20,  "%d");
    SliderRow("Sensitivity",      R.Sensitivity,     0.1f,   10.0f,"%.2f");
    SliderRow("Medkit Rate",      R.MedkitRate,      0.1f,   5.0f, "%.2f");
    SliderRow("AI FOV",           R.AiFov,           0.0f,   180.0f,"%.0f");
    CardEnd();
}

inline void TabNetwork(Refs& R)
{
    CardBegin("net_control", "NETWORK CONTROL", ICON_FA_SHIELD_ALT, "FIREWALL", pal.ok);
    ToggleRow("Firewall",         R.FirewallOn,      "guard");
    ToggleRow("Verbose Logging",  R.FirewallVerbose, "debug");
    CardEnd();
    Spacer(2.0f);

    CardBegin("net_status", "LINK STATUS", ICON_FA_IMAGE);
    InfoRow("Hooks",     g_FwStats.hooksOk ? "ACTIVE" : "IDLE",
            g_FwStats.hooksOk ? pal.ok : pal.warn);
    InfoRow("Libraries", "COW / ANOGS / TLS", pal.textSoft);
    InfoRow("Whitelist", FW_ALLOWED_DOMAIN, pal.accentHi);
    InfoRow("Cached IP", strlen(g_fwAllowedIpCache) > 0 ? g_fwAllowedIpCache : "unresolved",
            strlen(g_fwAllowedIpCache) > 0 ? pal.ok : pal.warn);
    CardEnd();
    Spacer(2.0f);

    CardBegin("net_stats", "TRAFFIC COUNTER", ICON_FA_BARS);
    {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d", g_FwStats.totalChecks);
        MeterBar("Checks", 1.0f, pal.accent, buf);

        int allowed = g_FwStats.totalAllowed;
        int denied  = g_FwStats.totalBlocked;
        int total   = ImMax(1, allowed + denied);
        snprintf(buf, sizeof(buf), "%d", allowed);
        MeterBar("Allowed", (float)allowed / (float)total, pal.ok, buf);
        snprintf(buf, sizeof(buf), "%d", denied);
        MeterBar("Blocked", (float)denied / (float)total, pal.danger, buf);
    }
    InfoRow("Anogs",   g_FwStats.anogsBlocked  ? "BLOCKED" : "-", pal.danger);
    InfoRow("Domains", g_FwStats.domainBlocked ? "BLOCKED" : "-", pal.danger);
    InfoRow("Ports",   g_FwStats.portBlocked   ? "BLOCKED" : "-", pal.danger);
    InfoRow("Private", g_FwStats.privIpBlocked ? "BLOCKED" : "-", pal.danger);
    CardEnd();
    Spacer(2.0f);

    CardBegin("net_console", "LIVE CONSOLE", ICON_FA_BARS, "LIVE", pal.ok);
    {
        char head[96];
        snprintf(head, sizeof(head), "%d / %d entries", (int)g_fwLogs.size(), FW_MAX_LOGS);
        SmallText(ImGui::GetWindowDrawList(), ImGui::GetCursorScreenPos(), pal.textDim, head);
        Spacer(14.0f);
    }

    ImGui::PushStyleColor(ImGuiCol_ChildBg, U32(ImVec4(0.031f, 0.024f, 0.047f, 0.85f)));
    ImGui::BeginChild("##fwlog", ImVec2(0, S(190.0f)), true);
    TouchScroll(true);
    {
        std::lock_guard<std::mutex> lk(g_fwLogMutex);
        if (g_fwLogs.empty())
        {
            ImGui::TextColored(pal.textDim, "waiting for traffic...");
        }
        else
        {
            const float fs = ImGui::GetFontSize() * 0.80f;
            ImDrawList* ldl = ImGui::GetWindowDrawList();
            for (auto it = g_fwLogs.rbegin(); it != g_fwLogs.rend(); ++it)
            {
                const FW_LogEntry& e = *it;
                ImVec2 p = ImGui::GetCursorScreenPos();
                const float h = S(20.0f);
                ldl->AddRectFilled(p, ImVec2(p.x + ImGui::GetContentRegionAvail().x, p.y + h),
                                   U32(e.isBlocked ? pal.danger : pal.ok, 0.10f), S(4.0f));
                ldl->AddText(ImGui::GetFont(), fs, ImVec2(p.x + S(6.0f), p.y + S(3.0f)), U32(pal.textDim), e.time.c_str());
                ldl->AddText(ImGui::GetFont(), fs, ImVec2(p.x + S(74.0f), p.y + S(3.0f)),
                             U32(e.isBlocked ? pal.danger : pal.ok), e.isBlocked ? "BLOCK" : "ALLOW");
                char rest[256];
                snprintf(rest, sizeof(rest), "| %s | %s:%s | %s", e.libName.c_str(), e.target.c_str(),
                         e.port.c_str(), e.reason.c_str());
                ldl->AddText(ImGui::GetFont(), fs, ImVec2(p.x + S(122.0f), p.y + S(3.0f)), U32(pal.textSoft), rest);
                ImGui::Dummy(ImVec2(1.0f, h));
            }
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();

    Spacer(4.0f);
    if (Button("CLEAR LOGS", ICON_FA_MINUS, ImVec2(S(150.0f), S(32.0f)), false))
    {
        std::lock_guard<std::mutex> lk(g_fwLogMutex);
        g_fwLogs.clear();
        Toast("LOGS CLEARED");
    }
    CardEnd();
}

inline void TabSettings(Refs& R)
{
    CardBegin("set_ui", "INTERFACE", ICON_FA_SLIDERS_H, "THEME");
    SliderRow("UI Scale",        &theme.uiScale,       0.70f, 1.40f, "%.2fx");
    SliderRow("Window Opacity",  &theme.windowOpacity, 0.45f, 1.00f, "%.2f");
    SliderRow("Window Width",    &theme.menuWidth,     560.0f, 1180.0f, "%.0f px");
    SliderRow("Window Height",   &theme.menuHeight,    360.0f, 860.0f,  "%.0f px");
    SliderRow("Launcher Size",   &theme.launcherRadius,22.0f, 48.0f,  "%.0f px");
    SliderRow("Glow Strength",   &theme.glowStrength,  0.00f, 2.00f,  "%.2f");

    ToggleRow("FPS Counter",     &theme.showFps,        "overlay");
    ToggleRow("Status Bar",      &theme.showStatusBar,  "bottom");
    ToggleRow("Floating Button", &theme.showLauncher,   "launcher");
    ToggleRow("Compact Rows",    &theme.compactRows,    "density");
    CardEnd();
    Spacer(2.0f);

    CardBegin("set_accent", "ACCENT COLOUR", ICON_FA_MAGIC);
    {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        const float avail = ImGui::GetContentRegionAvail().x;
        const float gap = S(8.0f);
        const float bw = (avail - gap * 2.0f) / 3.0f;
        const float bh = S(34.0f);

        for (int i = 0; i < kAccentCount; ++i)
        {
            ImGui::PushID(i);
            ImVec2 p = ImGui::GetCursorScreenPos();
            const bool tapped = ImGui::InvisibleButton("##sw", ImVec2(bw, bh));
            const bool hovered = ImGui::IsItemHovered();
            const bool sel = (theme.accentIndex == i);
            if (tapped)
            {
                ApplyAccent(i);
                Toast("ACCENT APPLIED");
            }
            if (sel || hovered)
                GlowRect(dl, p, ImVec2(p.x + bw, p.y + bh), S(8.0f), kAccents[i].main, sel ? 1.0f : 0.5f, 2);
            GradientV(dl, p, ImVec2(p.x + bw, p.y + bh), U32(kAccents[i].hi), U32(kAccents[i].deep), S(8.0f));
            dl->AddRect(p, ImVec2(p.x + bw, p.y + bh), U32(kAccents[i].main, sel ? 1.0f : 0.5f), S(8.0f), 0, sel ? S(2.0f) : S(1.0f));
            ImVec2 ts = ImGui::CalcTextSize(kAccents[i].name);
            dl->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 0.82f,
                        ImVec2(p.x + (bw - ts.x * 0.82f) * 0.5f, p.y + (bh - ts.y * 0.82f) * 0.5f),
                        U32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), kAccents[i].name);
            ImGui::PopID();
            if ((i % 3) != 2 && i != kAccentCount - 1) ImGui::SameLine(0.0f, gap);
        }
    }
    CardEnd();
    Spacer(2.0f);

    CardBegin("set_layout", "LAYOUT", ICON_FA_SORT);
    if (Button("RESET WINDOW POSITION", ICON_FA_MAGIC, ImVec2(0, S(32.0f)), false))
    {
        const ImVec2 disp = ImGui::GetIO().DisplaySize;
        menuX = (disp.x - theme.menuWidth) * 0.5f;
        menuY = (disp.y - theme.menuHeight) * 0.5f;
        Toast("LAYOUT RESET");
    }
    Spacer(4.0f);
    if (Button("RESET LAUNCHER POSITION", ICON_FA_BARS, ImVec2(0, S(32.0f)), false))
    {
        launcherX = S(60.0f);
        launcherY = ImGui::GetIO().DisplaySize.y * 0.4f;
        snapTargetX = launcherX;
        Toast("LAUNCHER RESET");
    }
    Spacer(4.0f);
    ToggleRow("Watermark", R.Watermark, "overlay", ICON_FA_GHOST);
    CardEnd();
    Spacer(2.0f);

    CardBegin("set_presets", "PRESETS", ICON_FA_CROWN, "CONFIG");
    for (int i = 0; i < 3; ++i)
    {
        ImGui::PushID(i);
        SectionTitle(kSlotNames[i], ICON_FA_IMAGE);
        float avail = ImGui::GetContentRegionAvail().x;
        float bw = (avail - S(8.0f)) * 0.5f;

        if (Button("SAVE", ICON_FA_HEART, ImVec2(bw, S(30.0f)), true)) SavePreset(R, i);
        ImGui::SameLine(0.0f, S(8.0f));
        if (Button(slots[i].used ? "LOAD" : "EMPTY", ICON_FA_CHEVRON_RIGHT, ImVec2(bw, S(30.0f)), slots[i].used))
            LoadPreset(R, i);

        ImGui::PopID();
    }
    CardEnd();
    Spacer(2.0f);

    CardBegin("set_about", "ABOUT", ICON_FA_CROWN);
    {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        const float avail = ImGui::GetContentRegionAvail().x;
        ImVec2 p = ImGui::GetCursorScreenPos();
        const float h = S(104.0f);
        ImGui::Dummy(ImVec2(avail, h));
        dl->AddRectFilled(p, ImVec2(p.x + avail, p.y + h), U32(pal.bgCardHi, 0.75f), S(10.0f));
        dl->AddRect(p, ImVec2(p.x + avail, p.y + h), U32(pal.border), S(10.0f), 0, S(1.0f));
        LogoDraw(dl, ImVec2(p.x + S(58.0f), p.y + h * 0.5f), S(74.0f), S(37.0f));
        dl->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 1.15f, ImVec2(p.x + S(104.0f), p.y + S(18.0f)),
                    U32(pal.text), kBrandUpper);
        dl->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 0.80f, ImVec2(p.x + S(104.0f), p.y + S(44.0f)),
                    U32(pal.accentHi), kEdition);
        dl->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 0.78f, ImVec2(p.x + S(104.0f), p.y + S(64.0f)),
                    U32(pal.textDim), kTelegramNice);
        dl->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 0.78f, ImVec2(p.x + S(104.0f), p.y + S(82.0f)),
                    U32(pal.textDim), "FIREWALL / ESP / AIM ENGINE");
        ImGui::Dummy(ImVec2(1.0f, S(6.0f)));
    }
    if (Button("JOIN TELEGRAM CHANNEL", ICON_FA_PAPER_PLANE, ImVec2(0, S(34.0f)), true))
    {
        if (hooks.openURL) hooks.openURL(kTelegram);
        Toast("OPENING TELEGRAM");
    }
    Spacer(4.0f);
    InfoRow("Build", kVersion, pal.accentHi);
    InfoRow("Engine", "IMGUI 1.89.2", pal.textSoft);
    CardEnd();
    Spacer(2.0f);
    DividerText("SHADOW MODZ 2026");
}

// ---------------------------------------------------------------------------
//  window
// ---------------------------------------------------------------------------
inline void DrawWindow(Refs& R, ImVec2 screen)
{
    const float headerH = S(64.0f);
    const float navW    = S(186.0f);
    const float statusH = theme.showStatusBar ? S(26.0f) : 0.0f;

    if (!positioned)
    {
        menuX = (screen.x - theme.menuWidth) * 0.5f;
        menuY = (screen.y - theme.menuHeight) * 0.5f;
        positioned = true;
        snapTargetX = launcherX;
    }

    theme.menuWidth  = ImClamp(theme.menuWidth,  S(560.0f), ImMax(S(560.0f), screen.x - S(12.0f)));
    theme.menuHeight = ImClamp(theme.menuHeight, S(360.0f), ImMax(S(360.0f), screen.y - S(12.0f)));
    menuX = ImClamp(menuX, -S(20.0f), ImMax(0.0f, screen.x - theme.menuWidth + S(20.0f)));
    menuY = ImClamp(menuY, 0.0f, ImMax(0.0f, screen.y - S(80.0f)));

    ImGui::SetNextWindowSize(ImVec2(theme.menuWidth, theme.menuHeight), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(menuX, menuY), ImGuiCond_Always);
    ImGui::Begin("##shadowmodz_window", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
                 ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);

    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 wp = ImGui::GetWindowPos();
    ImVec2 ws = ImGui::GetWindowSize();
    menuRect = ImVec4(wp.x, wp.y, wp.x + ws.x, wp.y + ws.y);

    // glass frame
    dl->AddRectFilled(wp, ImVec2(wp.x + ws.x, wp.y + ws.y), U32(pal.bgWindow, pal.bgWindow.w * theme.windowOpacity), S(14.0f));
    DotGrid(dl, ImVec2(wp.x + 4.0f, wp.y + headerH + 6.0f), ImVec2(wp.x + ws.x - 4.0f, wp.y + ws.y - statusH - 4.0f),
            Alpha(pal.accent, 0.05f), S(26.0f));
    GlowRect(dl, wp, ImVec2(wp.x + ws.x, wp.y + ws.y), S(14.0f), pal.accent, 1.1f * theme.glowStrength, 3);
    dl->AddRect(wp, ImVec2(wp.x + ws.x, wp.y + ws.y), U32(pal.accent, 0.55f), S(14.0f), 0, S(1.4f));
    dl->AddRect(ImVec2(wp.x + S(2.0f), wp.y + S(2.0f)), ImVec2(wp.x + ws.x - S(2.0f), wp.y + ws.y - S(2.0f)),
                U32(pal.pink, 0.16f), S(12.0f), 0, S(1.0f));

    DrawHeader(R, dl, wp, ws, headerH);

    // nav rail
    ImGui::SetCursorScreenPos(ImVec2(wp.x + S(8.0f), wp.y + headerH + S(8.0f)));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, U32(pal.bgPanel, pal.bgPanel.w * theme.windowOpacity));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, S(10.0f));
    DrawNav(R, ImVec2(navW, ws.y - headerH - statusH - S(16.0f)));
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    // content
    ImGui::SetCursorScreenPos(ImVec2(wp.x + navW + S(14.0f), wp.y + headerH + S(8.0f)));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, U32(ImVec4(0.031f, 0.024f, 0.047f, 0.65f * theme.windowOpacity)));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, S(10.0f));
    ImGui::BeginChild("##shadowmodz_content", ImVec2(ws.x - navW - S(22.0f), ws.y - headerH - statusH - S(16.0f)), false);
    TouchScroll(true);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(S(8.0f), S(5.0f)));
    Spacer(4.0f);

    switch (tab)
    {
        case 0: TabAimbot(R);   break;
        case 1: TabVisual(R);   break;
        case 2: TabMisc(R);     break;
        case 3: TabNetwork(R);  break;
        case 4: TabSettings(R); break;
        default: break;
    }

    Spacer(10.0f);
    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    if (theme.showStatusBar)
        DrawStatusBar(R, dl, wp, ws, statusH);

    ImGui::End();
}

// ---------------------------------------------------------------------------
//  toast
// ---------------------------------------------------------------------------
inline void DrawToast(ImVec2 screen)
{
    if (ImGui::GetTime() > toastUntil || toastText[0] == 0) return;

    ImDrawList* dl = ImGui::GetForegroundDrawList();
    if (!dl) return;

    float alpha = ImClamp((toastUntil - (float)ImGui::GetTime()) / 0.6f, 0.0f, 1.0f);
    ImVec2 ts = ImGui::CalcTextSize(toastText);
    ImVec2 a(screen.x * 0.5f - ts.x * 0.5f - S(18.0f), screen.y - S(76.0f));
    ImVec2 b(a.x + ts.x + S(36.0f), a.y + ts.y + S(16.0f));

    dl->AddRectFilled(a, b, U32(pal.bgCardHi, 0.94f * alpha), S(10.0f));
    dl->AddRect(a, b, U32(pal.accent, 0.85f * alpha), S(10.0f), 0, S(1.2f));
    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
                ImVec2(a.x + S(18.0f), a.y + S(8.0f)), U32(pal.text, alpha), toastText);
}

// ---------------------------------------------------------------------------
//  entry point - call once per frame from the EGL swap hook
// ---------------------------------------------------------------------------
inline void Draw(Refs& R, float screenW, float screenH)
{
    ShadowUI::ApplyStyle();
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 1.0f;   // widgets scale themselves through S()

    const ImVec2 screen(screenW, screenH);

    if (open)
        DrawWindow(R, screen);
    else
        menuRect = ImVec4(-1.0f, -1.0f, -1.0f, -1.0f);

    LauncherInput(screen);
    LauncherVisuals(ImGui::GetForegroundDrawList(), screen);
    DrawToast(screen);
}

} // namespace ShadowMenu
