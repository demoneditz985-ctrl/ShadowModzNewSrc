#pragma once
// ============================================================================
//  SHADOW MODZ - FLOATING MENU  ("ONYX")
// ----------------------------------------------------------------------------
//  * the lock screen comes first: until the access key is typed on the built in
//    keypad the feature tabs do not exist (see ShadowAuth.hpp),
//  * five tabs (AIM / ESP / CATCH / MISC / SYSTEM) hold every feature the mod
//    has - the X-macro lists below are the single source of truth for the
//    bindings, so a feature cannot silently disappear from the UI,
//  * draggable window (header), scrollable content, round launcher bubble that
//    snaps to the closest screen edge,
//  * every tap is resolved by ShadowUI's pointer layer (one press = one owner),
//    which is what fixed the "tap lands somewhere else" problem.
// ============================================================================

#include "imgui.h"
#include "imgui_internal.h"

#include "SHADOWMODZ MODS/ShadowTheme.hpp"
#include "SHADOWMODZ MODS/ShadowAuth.hpp"

#include <cmath>
#include <cstdio>
#include <cstring>

namespace ShadowMenu
{

// ---------------------------------------------------------------------------
//  branding
// ---------------------------------------------------------------------------
inline const char* kBrand        = "SHADOW MODZ";
inline const char* kTagline      = "PREMIUM MOD MENU";
inline const char* kVersion      = "v5.0";
inline const char* kTelegram     = "https://t.me/+BBimnHMiSvpiYTBl";
inline const char* kTelegramNice = "t.me/+BBimnHMiSvpiYTBl";

// ---------------------------------------------------------------------------
//  E V E R Y   F E A T U R E  (single source of truth for the bindings)
// ---------------------------------------------------------------------------
#define SHADOW_BOOL_FIELDS(X)                        \
    X(Enable,            Enable)                     \
    X(Aimbot,            Aimbot)                     \
    X(SilentAim,         SilentAim)                  \
    X(AimSilent,         AimSilent)                  \
    X(Headshot,          Headshot)                   \
    X(AimVisible2,       AimVisible2)                \
    X(AimVisibleThread,  aimVisibleEnabled)          \
    X(Antidev,           ANTIDEV)                    \
    X(AimKill1,          AimKill1)                   \
    X(AutoKill,          AutoKill)                   \
    X(AimLock,           AimLock)                    \
    X(AimLockWall,       AimLockThroughWall)         \
    X(CustomFireRate,    CustomFireRate)             \
    X(SpinBot,           SpinBot)                    \
    X(EspLine,           Config.ESP.Line)            \
    X(EspBox,            Config.ESP.Box)             \
    X(EspBox3d,          Config.ESP.Box3d)           \
    X(EspBoxFilled,      Config.ESP.BoxFilled)       \
    X(EspBoxHalf,        Config.ESP.BoxHalfFilled)   \
    X(EspHealth,         Config.ESP.Health)          \
    X(EspIdentity,       Config.ESP.MAFIA)           \
    X(EspDistance,       Config.ESP.Distance)        \
    X(EspBones,          EspSkeleton2)               \
    X(EspFovCircle,      DrawFov)                    \
    X(SniperAlert,       SniperAlert)                \
    X(EspGrenade,        ESPGrenade)                 \
    X(NightMode,         EnableNightMode)            \
    X(MassKill,          Mass.Sucks)                 \
    X(ShurikenKill,      ShurikenKill)               \
    X(AutoRevive,        autoReviveEnabled)          \
    X(MedkitMoving,      MedkitMoving)               \
    X(MedkitFasterRate,  FUCKMODI)                   \
    X(QuickReload,       QuickReloadLakiyaxMods)     \
    X(GliderHack,        gliderHackEnabled)          \
    X(GliderInfinite,    gliderInfiniteEnabled)      \
    X(Parachuting,       isParachuting)              \
    X(SpeedHack,         SpeedHack)                  \
    X(SpeedRun,          speedrun)                   \
    X(FastSwitch,        Fastswitch)                 \
    X(AutoRun,           AutoRun)                    \
    X(InfiniteRange,     InfiniteRange)              \
    X(NoRecoil,          norecoil)                   \
    X(SniperFast,        SniperFast)                 \
    X(PcSwapAnim,        PcSwapAnim)                 \
    X(SetIndia,          Setindia)                   \
    X(CameraHeight,      camxa)                      \
    X(GuestReset,        Reset)                      \
    X(FreeMove,          FreeMove)                   \
    X(StopFireAnim,      StopFireAnim)               \
    X(FastSensitivity,   FastSensivity)              \
    X(SpoofName,         SpoofNameEnabled)           \
    X(UserLevelSpoof,    LUNDUP)                     \
    X(Watermark,         RenderWatermark)            \
    X(BypassLogin400,    g_LoginBypass400)           \
    X(FirewallOn,        g_FirewallEnabled)          \
    X(FirewallVerbose,   g_FirewallVerbose)

#define SHADOW_FLOAT_FIELDS(X)                       \
    X(FovAim,            Fov_Aim)                    \
    X(AimDistance,       Aimdis)                     \
    X(AimSpeed,          AimSpeed)                   \
    X(AimKillFov,        AimKillFov)                 \
    X(AimKillDistance,   AimKillDistance)            \
    X(AutoKillDelay,     AutoKillDelayMs)            \
    X(AimLockFov,        AimLockFov)                 \
    X(AimLockSpeed,      AimLockSpeed)               \
    X(FireRate,          FireRateMs)                 \
    X(ShotsPerBurst,     ShotsPerBurst)              \
    X(SpinBotSpeed,      SpinBotSpeed)               \
    X(AiFov,             AiFov)                      \
    X(LineThickness,     LineThickness)              \
    X(BoxThickness,      BoxThickness)               \
    X(BoxRoundness,      BoxRoundness)               \
    X(MassKillRange,     Mass.Sucks1)                \
    X(MassKillOffset,    Mass.FrontOffset)           \
    X(MedkitRate,        FUCKMODIRate)               \
    X(SpeedMultiplier,   active)                     \
    X(Sensitivity,       Sensivity)

#define SHADOW_INT_FIELDS(X)                         \
    X(AimHeight,         g_AimHeightIndex)           \
    X(AimWhen,           AimWhen)                    \
    X(BoxStyle,          BoxStyle)                   \
    X(LineMode,          LineMode)                   \
    X(CarYOffset,        DONTCarValue)

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

// callbacks into the mod
struct Hooks
{
    void (*openURL)(const char* url) = nullptr;
    void (*applyAimHeight)(int index) = nullptr;
};
inline Hooks hooks;

// ---------------------------------------------------------------------------
//  state
// ---------------------------------------------------------------------------
inline bool  open       = false;
inline int   tab        = 0;
inline float menuX      = 0.0f;
inline float menuY      = 0.0f;
inline bool  positioned = false;
inline ImRect windowRect = ImRect(ImVec2(0, 0), ImVec2(0, 0));

inline float scrollY     = 0.0f;
inline float scrollAnim  = 0.0f;
inline float scrollMax   = 0.0f;

inline float launcherX    = 0.0f;
inline float launcherY    = 0.0f;
inline bool  launcherInit = false;

inline char  keyBuf[ShadowAuth::kKeyMax + 1] = { 0 };
inline int   keyLen  = 0;
inline bool  keyFail = false;
inline float keyFailT = 0.0f;
inline int   keyTries = 0;
inline bool  authLoaded = false;

inline char   toastText[96] = { 0 };
inline float  toastUntil    = 0.0f;
inline ImVec4 toastCol      = ImVec4(1, 1, 1, 1);

inline const int   kTabCount = 5;
inline const char* kTabs[kTabCount] = { "AIM", "ESP", "CATCH", "MISC", "SYSTEM" };
inline const ShadowUI::Glyph kTabGlyphs[kTabCount] =
{
    ShadowUI::GAim, ShadowUI::GEye, ShadowUI::GBomb, ShadowUI::GGear, ShadowUI::GSliders
};

inline void Toast(const char* msg, ImVec4 col = ImVec4(0.29f, 0.90f, 0.54f, 1.0f), float dur = 2.2f)
{
    snprintf(toastText, sizeof(toastText), "%s", msg ? msg : "");
    toastCol   = col;
    toastUntil = ImGui::GetTime() + dur;
}

// ---------------------------------------------------------------------------
//  key entry
// ---------------------------------------------------------------------------
inline void ScrollReset();
inline void HIDE_PANEL();
inline void KeyFailTimeReset();

inline void KeyPush(char c)
{
    if (keyLen >= ShadowAuth::kKeyMax) return;
    keyBuf[keyLen++] = c;
    keyBuf[keyLen]   = 0;
    keyFail = false;
}
inline void KeyPop()
{
    if (keyLen > 0) keyBuf[--keyLen] = 0;
    keyFail = false;
}
inline void KeyClear() { keyLen = 0; keyBuf[0] = 0; }

inline void TryUnlock()
{
    if (ShadowAuth::Verify(keyBuf))
    {
        ShadowAuth::unlocked = true;
        ShadowAuth::SaveUnlock();
        KeyClear();
        keyFail  = false;
        keyTries = 0;
        scrollY = 0.0f;
        tab     = 0;
        Toast("ACCESS GRANTED - WELCOME", ImVec4(0.29f, 0.90f, 0.54f, 1.0f), 3.0f);
    }
    else
    {
        ++keyTries;
        keyFail  = true;
        KeyFailTimeReset();
        KeyClear();
        Toast("WRONG KEY - GET IT ON TELEGRAM", ImVec4(0.98f, 0.29f, 0.40f, 1.0f), 2.6f);
    }
}


inline void KeyFailTimeReset() { keyFailT = ImGui::GetTime(); }
inline void ScrollReset()      { scrollY = 0.0f; scrollAnim = 0.0f; }

inline int EnabledCount(Refs& R)
{
    int n = 0;
#define SHADOW_COUNT_BOOL(name, target) if (R.name && *R.name) ++n;
    SHADOW_BOOL_FIELDS(SHADOW_COUNT_BOOL)
#undef SHADOW_COUNT_BOOL
    return n;
}

// ---------------------------------------------------------------------------
//  shared button helpers
// ---------------------------------------------------------------------------
inline ImRect BtnRowAlloc(float h) { return ShadowUI::Alloc(ShadowUI::S(h)); }

inline void SplitRow(const ImRect& row, ImRect& l, ImRect& r, float gap = 8.0f)
{
    float g = ShadowUI::S(gap);
    float w = (row.GetWidth() - g) * 0.5f;
    l = ImRect(row.Min, ImVec2(row.Min.x + w, row.Max.y));
    r = ImRect(ImVec2(row.Min.x + w + g, row.Min.y), row.Max);
}

inline void HIDE_PANEL();
inline void ScrollReset();

inline bool WideBtn(const char* id, const char* label, ShadowUI::Glyph g,
                    bool primary = false, bool danger = false)
{
    ImRect r = ShadowUI::Alloc(ShadowUI::S(42.0f));
    if (!ShadowUI::Visible(r)) return false;
    return ShadowUI::Btn(id, r, label, g, primary, danger);
}

// ---------------------------------------------------------------------------
//  launcher bubble
// ---------------------------------------------------------------------------
inline void Launcher(Refs& R, ImVec2 screen)
{
    ImDrawList* dl = ImGui::GetWindowDrawList();
    const float rad = ShadowUI::S(ShadowUI::theme.launcherRadius);
    const ImU32 uid = ShadowUI::P::Id("##smdz_launcher");

    if (!launcherInit)
    {
        launcherX = screen.x - ShadowUI::S(74.0f);
        launcherY = screen.y * 0.42f;
        launcherInit = true;
    }

    ImRect r(ImVec2(launcherX - rad, launcherY - rad), ImVec2(launcherX + rad, launcherY + rad));
    ShadowUI::P::Claim(uid, r);

    const bool dragging = ShadowUI::P::Owns(uid) && ShadowUI::P::st.down;
    if (dragging)
    {
        launcherX += ShadowUI::P::st.delta.x;
        launcherY += ShadowUI::P::st.delta.y;
        launcherX = ImClamp(launcherX, rad + ShadowUI::S(4.0f), screen.x - rad - ShadowUI::S(4.0f));
        launcherY = ImClamp(launcherY, rad + ShadowUI::S(6.0f), screen.y - rad - ShadowUI::S(6.0f));
        r = ImRect(ImVec2(launcherX - rad, launcherY - rad), ImVec2(launcherX + rad, launcherY + rad));
        ShadowUI::P::Claim(uid, r);
    }
    if (ShadowUI::P::Tap(uid, r))
    {
        open = true;
        ScrollReset();
        Toast(ShadowAuth::unlocked ? "MENU OPENED" : "ENTER YOUR ACCESS KEY");
    }
    if (ShadowUI::P::st.wentUp && ShadowUI::P::st.owner == uid && ShadowUI::P::st.moved)
    {
        // snapped to the closest horizontal edge after a drag
        float mid = screen.x * 0.5f;
        float target = (launcherX < mid) ? (rad + ShadowUI::S(10.0f)) : (screen.x - rad - ShadowUI::S(10.0f));
        launcherX += (target - launcherX) * ShadowUI::Clamp01(ImGui::GetIO().DeltaTime * 12.0f);
    }

    const float hv = ShadowUI::Anim(uid ^ 0x0Eu, (ShadowUI::P::Hot(uid, r) || dragging) ? 1.0f : 0.0f);
    const float pulse = 0.55f + 0.45f * sinf(ImGui::GetTime() * 2.2f);

    ShadowUI::GlowCircle(dl, ImVec2(launcherX, launcherY), rad, ShadowUI::pal.accent,
                         (0.7f + 0.5f * hv) * (0.6f + 0.4f * pulse), 4);
    dl->AddCircleFilled(ImVec2(launcherX, launcherY), rad,
                        ShadowUI::U32(ShadowUI::Mix(ShadowUI::pal.bgWindow, ShadowUI::pal.accent, 0.10f + 0.18f * hv)));
    dl->AddCircle(ImVec2(launcherX, launcherY), rad, ShadowUI::U32(ShadowUI::pal.accentHi, 0.55f + 0.35f * hv), 0, ShadowUI::S(1.6f));

    ShadowUI::LogoDraw(dl, ImVec2(launcherX, launcherY), rad * 1.28f, rad * 0.64f, false);

    // badge: how many features are on (or a lock while the key screen is up)
    const int n = EnabledCount(R);
    char badge[16];
    if (!ShadowAuth::unlocked) snprintf(badge, sizeof(badge), "KEY");
    else                       snprintf(badge, sizeof(badge), "%d", n);
    ImVec2 bs = ShadowUI::TextSize(badge, 0.66f);
    ImRect b(ImVec2(launcherX + rad * 0.42f, launcherY - rad * 0.95f),
             ImVec2(launcherX + rad * 0.42f + ImMax(bs.x + ShadowUI::S(12.0f), ShadowUI::S(24.0f)), launcherY - rad * 0.45f));
    dl->AddRectFilled(b.Min, b.Max, ShadowUI::U32(ShadowAuth::unlocked ? ShadowUI::pal.accentDeep : ShadowUI::pal.danger, 0.92f), ShadowUI::S(8.0f));
    dl->AddRect(b.Min, b.Max, ShadowUI::U32(ShadowUI::pal.accentHi, 0.6f), ShadowUI::S(8.0f), 0, ShadowUI::S(1.0f));
    ShadowUI::TextCenter(dl, b, badge, IM_COL32(255, 255, 255, 255), 0.66f);
}


// ---------------------------------------------------------------------------
//  L O C K   S C R E E N
// ---------------------------------------------------------------------------
inline void LockScreen(ImDrawList* dl, const ImRect& area)
{
    if (!dl) return;
    ShadowUI::BeginLayout(dl, area.Min.x, area.Min.y, area.GetWidth(), area.Min.y, area.Max.y, 0.0f);

    const float shake = (keyFail && (ImGui::GetTime() - keyFailT) < 0.40f)
                      ? sinf((ImGui::GetTime() - keyFailT) * 70.0f) * ShadowUI::S(7.0f) : 0.0f;

    ImRect inner(ImVec2(area.Min.x + ShadowUI::S(14.0f), area.Min.y + ShadowUI::S(6.0f)),
                 ImVec2(area.Max.x - ShadowUI::S(14.0f), area.Max.y - ShadowUI::S(4.0f)));

    // ---- heading ----
    ImRect head(inner.Min, ImVec2(inner.Max.x, inner.Min.y + ShadowUI::S(72.0f)));
    ShadowUI::LogoDraw(dl, ImVec2(head.GetCenter().x, head.Min.y + ShadowUI::S(28.0f)), ShadowUI::S(50.0f), ShadowUI::S(25.0f), true);
    ImRect cap(ImVec2(head.Min.x, head.Min.y + ShadowUI::S(54.0f)), head.Max);
    ShadowUI::TextCenter(dl, cap, "ACCESS KEY REQUIRED", ShadowUI::U32(ShadowUI::pal.text), 0.94f);

    // ---- key field ----
    ImRect field(ImVec2(inner.Min.x, head.Max.y + ShadowUI::S(2.0f)),
                 ImVec2(inner.Max.x, head.Max.y + ShadowUI::S(50.0f)));
    field.Min.x += shake; field.Max.x += shake;

    const float fr = ShadowUI::S(11.0f);
    ImVec4 fedge = keyFail ? ShadowUI::pal.danger : ShadowUI::pal.accent;
    ShadowUI::GlowRound(dl, field, fr, fedge, (keyFail ? 1.1f : 0.45f) * ShadowUI::theme.glowStrength, 3);
    dl->AddRectFilled(field.Min, field.Max, ShadowUI::U32(ShadowUI::pal.bgBar, 0.95f), fr);
    dl->AddRect(field.Min, field.Max, ShadowUI::U32(fedge, keyFail ? 0.95f : 0.50f), fr, 0, ShadowUI::S(1.3f));

    if (keyLen == 0)
    {
        ShadowUI::TextCenter(dl, field, "TYPE THE KEY BELOW", ShadowUI::U32(ShadowUI::pal.textDim), 0.86f);
    }
    else
    {
        // the field is masked: the key never sits in plain sight on a stream
        char masked[ShadowAuth::kKeyMax + 1];
        int  n = keyLen < ShadowAuth::kKeyMax ? keyLen : ShadowAuth::kKeyMax;
        for (int i = 0; i < n; ++i) masked[i] = '*';
        masked[n] = 0;

        ImVec2 tsz = ShadowUI::TextSize(masked, 1.05f);
        ShadowUI::TextAt(dl, masked, ImVec2(field.GetCenter().x - tsz.x * 0.5f,
                                            field.GetCenter().y - tsz.y * 0.5f),
                         ShadowUI::U32(ShadowUI::pal.accentHi), 1.05f);
        float cx = field.GetCenter().x + tsz.x * 0.5f + ShadowUI::S(5.0f);
        if (fmodf(ImGui::GetTime(), 1.0f) < 0.55f)
            dl->AddLine(ImVec2(cx, field.GetCenter().y - ShadowUI::S(13.0f)),
                        ImVec2(cx, field.GetCenter().y + ShadowUI::S(13.0f)),
                        ShadowUI::U32(ShadowUI::pal.accent, 0.9f), ShadowUI::S(1.8f));
    }

    // ---- keypad: A-Z + 0-9 in a 6x6 grid ----
    // the actions and the status line keep their own band at the bottom, the
    // grid takes whatever is left (it must never be overlapped by either)
    const float actH    = ShadowUI::S(44.0f);
    const float statusH = ShadowUI::S(24.0f);
    ImRect pad(ImVec2(inner.Min.x, field.Max.y + ShadowUI::S(8.0f)),
               ImVec2(inner.Max.x, inner.Max.y - actH - statusH - ShadowUI::S(12.0f)));
    const int cols = 6, rows = 6;
    const float gapX = ShadowUI::S(5.0f), gapY = ShadowUI::S(5.0f);
    const float cw = (pad.GetWidth()  - gapX * (cols - 1)) / (float)cols;
    const float ch = (pad.GetHeight() - gapY * (rows - 1)) / (float)rows;
    const char* keys = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    for (int i = 0; i < 36; ++i)
    {
        if (cw < ShadowUI::S(6.0f) || ch < ShadowUI::S(6.0f)) break;      // screen too small
        int cx = i % cols, cy = i / cols;
        ImRect cell(ImVec2(pad.Min.x + (cw + gapX) * cx, pad.Min.y + (ch + gapY) * cy),
                    ImVec2(pad.Min.x + (cw + gapX) * cx + cw, pad.Min.y + (ch + gapY) * cy + ch));
        char id[24], lbl[4];
        snprintf(id, sizeof(id), "##key%d", i);
        snprintf(lbl, sizeof(lbl), "%c", keys[i]);
        if (ShadowUI::Keycap(id, lbl, cell, false)) KeyPush(keys[i]);
    }

    // ---- actions ----
    ImRect act(ImVec2(inner.Min.x, inner.Max.y - actH - statusH - ShadowUI::S(4.0f)),
               ImVec2(inner.Max.x, inner.Max.y - statusH - ShadowUI::S(4.0f)));
    ImRect bDel, bOk, bTg;
    {
        float g = ShadowUI::S(6.0f);
        float bw = (act.GetWidth() - g * 2.0f) / 3.0f;
        bDel = ImRect(act.Min, ImVec2(act.Min.x + bw, act.Max.y));
        bOk  = ImRect(ImVec2(bDel.Max.x + g, act.Min.y), ImVec2(bDel.Max.x + g + bw, act.Max.y));
        bTg  = ImRect(ImVec2(bOk.Max.x + g, act.Min.y), act.Max);
    }

    if (ShadowUI::Keycap("##key_del", "", bDel, false, ShadowUI::GX)) KeyPop();
    if (ShadowUI::Keycap("##key_ok", "UNLOCK", bOk, true)) TryUnlock();
    if (ShadowUI::Keycap("##key_tg", "GET KEY", bTg, false))
    {
        if (hooks.openURL) hooks.openURL(kTelegram);
        Toast("OPENING TELEGRAM", ShadowUI::pal.accentHi, 2.0f);
    }

    // ---- status ----
    const char* msg = keyFail ? "WRONG KEY - ASK IN THE TELEGRAM CHANNEL"
                              : (keyTries > 0 ? "GET A KEY FROM THE TELEGRAM CHANNEL BELOW"
                                              : "THE MOD UNLOCKS ONCE THE KEY IS ACCEPTED");
    ImRect status(ImVec2(inner.Min.x, inner.Max.y - statusH), inner.Max);
    ShadowUI::TextCenter(dl, status, msg, ShadowUI::U32(keyFail ? ShadowUI::pal.danger : ShadowUI::pal.textDim, 0.95f), 0.72f);
}

// ---------------------------------------------------------------------------
//  H E A D E R   /   T A B S   /   F O O T E R
// ---------------------------------------------------------------------------
inline void Header(ImDrawList* dl, Refs& R, const ImRect& r)
{
    (void)R;
    const ImU32 dragId = ShadowUI::P::Id("##smdz_header");
    ShadowUI::P::Claim(dragId, r);
    if (ShadowUI::P::Owns(dragId) && ShadowUI::P::st.down && !ShadowUI::P::st.wentDown)
    {
        menuX += ShadowUI::P::st.delta.x;
        menuY += ShadowUI::P::st.delta.y;
    }

    float x = r.Min.x + ShadowUI::S(18.0f);
    ShadowUI::LogoDraw(dl, ImVec2(x + ShadowUI::S(15.0f), r.GetCenter().y), ShadowUI::S(30.0f), ShadowUI::S(15.0f), true);
    x += ShadowUI::S(42.0f);

    ShadowUI::TextVCenter(dl, r, x, kBrand, ShadowUI::U32(ShadowUI::pal.text), 1.0f);
    float wm = ShadowUI::TextSize(kBrand, 1.0f).x;
    x += wm + ShadowUI::S(12.0f);

    // telegram chip (opens the channel)
    {
        char chip[64];
        snprintf(chip, sizeof(chip), "JOIN");
        ImRect c(ImVec2(r.Max.x - ShadowUI::S(150.0f), r.GetCenter().y - ShadowUI::S(12.0f)),
                 ImVec2(r.Max.x - ShadowUI::S(88.0f), r.GetCenter().y + ShadowUI::S(12.0f)));
        if (ShadowUI::Btn("##hdr_join", c, chip, ShadowUI::GSend, false))
        {
            if (hooks.openURL) hooks.openURL(kTelegram);
            Toast("OPENING TELEGRAM", ShadowUI::pal.accentHi, 2.0f);
        }
    }

    // version chip
    {
        ImRect c(ImVec2(r.Max.x - ShadowUI::S(80.0f), r.GetCenter().y - ShadowUI::S(12.0f)),
                 ImVec2(r.Max.x - ShadowUI::S(14.0f), r.GetCenter().y + ShadowUI::S(12.0f)));
        dl->AddRectFilled(c.Min, c.Max, ShadowUI::U32(ShadowUI::pal.accent, 0.14f), ShadowUI::S(9.0f));
        dl->AddRect(c.Min, c.Max, ShadowUI::U32(ShadowUI::pal.accent, 0.42f), ShadowUI::S(9.0f), 0, ShadowUI::S(1.0f));
        ShadowUI::TextCenter(dl, c, kVersion, ShadowUI::U32(ShadowUI::pal.accentHi), 0.80f);
    }

    // drag dots
    for (int i = 0; i < 3; ++i)
        dl->AddCircleFilled(ImVec2(r.Min.x + ShadowUI::S(7.0f), r.GetCenter().y + (i - 1) * ShadowUI::S(4.0f)),
                            ShadowUI::S(1.3f), ShadowUI::U32(ShadowUI::pal.textDim, 0.85f));
}

inline void TabBar(ImDrawList* dl, const ImRect& r)
{
    const float gap = ShadowUI::S(6.0f);
    const float w = (r.GetWidth() - gap * (kTabCount - 1)) / (float)kTabCount;

    for (int i = 0; i < kTabCount; ++i)
    {
        ImRect cell(ImVec2(r.Min.x + (w + gap) * i, r.Min.y), ImVec2(r.Min.x + (w + gap) * i + w, r.Max.y));
        char id[24];
        snprintf(id, sizeof(id), "##tab%d", i);
        const ImU32 uid = ShadowUI::P::Id(id);

        ShadowUI::P::Claim(uid, cell);
        if (ShadowUI::P::Tap(uid, cell) && tab != i) { tab = i; scrollY = 0.0f; scrollAnim = 0.0f; }

        const bool sel = (tab == i);
        const float a  = ShadowUI::Anim(uid ^ 0x0ABCu, sel ? 1.0f : 0.0f);
        const float hv = ShadowUI::Anim(uid ^ 0x0DEFu, (ShadowUI::P::Hot(uid, cell) && !sel) ? 1.0f : 0.0f);
        ImRect body(cell.Min, ImVec2(cell.Max.x, cell.Max.y - ShadowUI::S(4.0f)));
        const float rr = ShadowUI::S(11.0f);

        if (a > 0.02f)
        {
            ShadowUI::GlowRound(dl, body, rr, ShadowUI::pal.accent, a * 0.85f * ShadowUI::theme.glowStrength, 3);
            dl->AddRectFilled(body.Min, body.Max, ShadowUI::U32(ShadowUI::pal.accentDeep, 0.28f + 0.32f * a), rr);
        }
        else
        {
            dl->AddRectFilled(body.Min, body.Max, ShadowUI::U32(ShadowUI::pal.bgBar, (0.70f + 0.25f * hv) * ShadowUI::theme.windowOpacity), rr);
        }
        dl->AddRect(body.Min, body.Max, ShadowUI::U32(sel ? ShadowUI::pal.accentHi : ShadowUI::pal.borderSoft, sel ? 0.85f : 0.65f), rr, 0, ShadowUI::S(1.2f));

        ImVec2 glyph(ImVec2(body.GetCenter().x, body.Min.y + body.GetHeight() * 0.32f));
        ShadowUI::GlyphDraw(dl, kTabGlyphs[i], glyph, ShadowUI::S(15.0f),
                            ShadowUI::U32(sel ? ShadowUI::pal.accentHi : ShadowUI::pal.textSoft, 0.65f + 0.35f * a), 1.0f);
        ImRect lbl(ImVec2(body.Min.x, body.Min.y + body.GetHeight() * 0.50f), body.Max);
        ShadowUI::TextCenter(dl, lbl, kTabs[i], ShadowUI::U32(sel ? ShadowUI::pal.text : ShadowUI::pal.textDim), 0.74f);

        if (a > 0.02f)
        {
            ImRect bar(ImVec2(body.Min.x + ShadowUI::S(10.0f), body.Max.y - ShadowUI::S(1.6f)),
                       ImVec2(body.Max.x - ShadowUI::S(10.0f), body.Max.y + ShadowUI::S(0.6f)));
            dl->AddRectFilled(bar.Min, bar.Max, ShadowUI::U32(ShadowUI::pal.accent, 0.35f + 0.65f * a), ShadowUI::S(1.5f));
        }
    }
}

// ---------------------------------------------------------------------------
//  T A B S
// ---------------------------------------------------------------------------
inline void TabAim(Refs& R)
{
    using namespace ShadowUI;

    SectionLabel("MASTER", GPower);
    Row("aim_enable", "MOD ENABLED", R.Enable, GPower, "ALL FEATURES");

    SectionLabel("AIMBOT", GAim);
    Row("aim_aimbot",    "Aimbot",            R.Aimbot,        GAim,     "auto aim");
    Row("aim_silent",    "Silent Aim",        R.SilentAim,     GBolt);
    Row("aim_aimsilent", "Aim Silent (Hook)", R.AimSilent,     GEye);
    Row("aim_headshot",  "Headshot",          R.Headshot,      GSkull,   "always head");
    Row("aim_visible2",  "Visible Check",     R.AimVisible2,   GEye);
    Row("aim_vthread",   "Visible Thread",    R.AimVisibleThread, GEye);
    Row("aim_antidev",   "Anti Dev Aim",      R.Antidev,       GShield);

    SectionLabel("AUTO TARGET", GMagnet);
    Row("aim_aimkill",   "Aim Kill",          R.AimKill1,      GBolt,    "instant");
    Row("aim_autokill",  "Auto Kill",         R.AutoKill,      GBolt);
    Row("aim_aimlock",   "Aim Lock",          R.AimLock,       GMagnet);
    Row("aim_lockwall",  "Lock Through Wall", R.AimLockWall,   GShield);
    Row("aim_firerate",  "Custom Fire Rate",  R.CustomFireRate, GFlame);
    Row("aim_spinbot",   "Spin Bot",          R.SpinBot,       GRocket);

    SectionLabel("TUNING", GSliders);
    Slider("aim_fov",    "Target FOV",     R.FovAim,         0.0f,   360.0f, "%.0f", GAim,   "deg");
    Slider("aim_dist",   "Max Distance",   R.AimDistance,    0.0f,   500.0f, "%.0f", GFlag,  "m");
    Slider("aim_speed",  "Aim Speed",      R.AimSpeed,       0.01f,  1.00f,  "%.2f", GGauge);
    Slider("aim_kfov",   "AimKill FOV",    R.AimKillFov,     0.0f,   800.0f, "%.0f", GAim);
    Slider("aim_kdist",  "AimKill Range",  R.AimKillDistance, 0.0f,  300.0f, "%.0f", GFlag,  "m");
    Slider("aim_kdelay", "AutoKill Delay", R.AutoKillDelay,  10.0f, 1000.0f, "%.0f", GGauge, "ms");
    Slider("aim_lfov",   "AimLock FOV",    R.AimLockFov,     0.0f,   800.0f, "%.0f", GMagnet);
    Slider("aim_lspeed", "AimLock Speed",  R.AimLockSpeed,   0.01f,  1.00f,  "%.2f", GGauge);
    Slider("aim_fr",     "Fire Rate",      R.FireRate,       20.0f, 1000.0f, "%.0f", GFlame, "ms");
    Slider("aim_burst",  "Shots / Burst",  R.ShotsPerBurst,  1.0f,    10.0f, "%.0f", GBolt);
    Slider("aim_spinsp", "SpinBot Speed",  R.SpinBotSpeed, 100.0f,  2000.0f, "%.0f", GRocket);
    Slider("aim_aifov",  "AI FOV",         R.AiFov,          0.0f,   360.0f, "%.0f", GAim, "deg");

    SectionLabel("BEHAVIOUR", GGear);
    static const char* heights[] = { "AUTO", "HEAD", "NECK", "CHEST", "BODY", "LEGS" };
    if (Choice("aim_height", "Hitbox Height", R.AimHeight, heights, IM_ARRAYSIZE(heights)))
        if (hooks.applyAimHeight && R.AimHeight) hooks.applyAimHeight(*R.AimHeight);

    static const char* whens[] = { "ALWAYS", "SCOPING", "FIRE" };
    Choice("aim_when", "Aim When", R.AimWhen, whens, IM_ARRAYSIZE(whens));
}

inline void TabEsp(Refs& R)
{
    using namespace ShadowUI;

    SectionLabel("PLAYERS", GPerson);
    Row("esp_line",    "ESP Line",       R.EspLine);
    Row("esp_box",     "ESP Box 2D",     R.EspBox,       GBox);
    Row("esp_box3d",   "ESP Box 3D",     R.EspBox3d,     GBox);
    Row("esp_fill",    "Box Filled",     R.EspBoxFilled, GBox);
    Row("esp_half",    "Box Half Filled", R.EspBoxHalf,  GBox);
    Row("esp_health",  "Health Bar",     R.EspHealth,    GHeart);
    Row("esp_ident",   "Name / Identity", R.EspIdentity, GUser);
    Row("esp_dist",    "Distance Text",  R.EspDistance,  GFlag);
    Row("esp_bones",   "Bone ESP",       R.EspBones,     GBone);

    SectionLabel("OVERLAY", GEye);
    Row("esp_fov",     "FOV Circle",   R.EspFovCircle, GAim);
    Row("esp_sniper",  "Sniper Alert", R.SniperAlert,  GBell);
    Row("esp_nade",    "ESP Grenade",  R.EspGrenade,   GBomb);
    Row("esp_night",   "Night Mode",   R.NightMode,    GEye);

    SectionLabel("STYLE", GPalette);
    Slider("esp_lth",  "Line Thickness", R.LineThickness, 0.5f,  5.0f, "%.1f", GSliders);
    Slider("esp_bth",  "Box Thickness",  R.BoxThickness,  0.5f,  5.0f, "%.1f", GSliders);
    Slider("esp_brad", "Box Roundness",  R.BoxRoundness,  0.0f, 15.0f, "%.1f", GSliders);
    static const char* boxStyles[] = { "SQUARE", "CORNER", "ROUND" };
    Choice("esp_bs", "Box Style", R.BoxStyle, boxStyles, IM_ARRAYSIZE(boxStyles));
    static const char* lineModes[] = { "TOP", "BOTTOM", "BOTH" };
    Choice("esp_lm", "Line Position", R.LineMode, lineModes, IM_ARRAYSIZE(lineModes));
}

inline void TabCatch(Refs& R)
{
    using namespace ShadowUI;

    SectionLabel("INSTANT KILL", GBomb);
    Row("cat_mass",   "Mass Kill",     R.MassKill,   GBomb, "use carefully");
    Slider("cat_range", "Mass Range",  R.MassKillRange,  1.0f, 60.0f, "%.0f", GFlag, "m");
    Slider("cat_off",   "Kill Offset", R.MassKillOffset, 0.0f, 10.0f, "%.1f", GSliders);
    Row("cat_shuriken", "Shuriken Kill", R.ShurikenKill, GStar);

    SectionLabel("SURVIVAL", GHeart);
    Row("cat_revive",  "Auto Revive",     R.AutoRevive,     GHeart);
    Row("cat_medmove", "Medkit While Run", R.MedkitMoving,  GCheck);
    Row("cat_medfast", "Instant Medkit",  R.MedkitFasterRate, GGauge);
    Slider("cat_medrate", "Medkit Rate",  R.MedkitRate, 1.0f, 20.0f, "%.1f", GSliders);
    Row("cat_reload",  "Quick Reload",    R.QuickReload,    GGauge);

    SectionLabel("GLIDER / PARA", GChute);
    Row("cat_glider",   "Glider Hack",       R.GliderHack,     GChute);
    Row("cat_gliderinf","Infinite Glider",   R.GliderInfinite, GChute);
    Row("cat_para",     "Instant Parachute", R.Parachuting,    GChute);
}

inline void TabMisc(Refs& R)
{
    using namespace ShadowUI;

    SectionLabel("MOVEMENT", GRocket);
    Row("mis_speed",  "Speed Hack",       R.SpeedHack,  GBolt, "runs faster");
    Slider("mis_mult", "Speed Multiplier", R.SpeedMultiplier, 1.0f, 20.0f, "%.1fx", GGauge);
    Row("mis_run",    "Speed Run",        R.SpeedRun,   GRocket);
    Row("mis_autorun","Auto Run",         R.AutoRun,    GRocket);
    Row("mis_inf",    "Infinite Range",   R.InfiniteRange, GFlag);
    Row("mis_free",   "Free Move",        R.FreeMove,   GRocket);
    Row("mis_cam",    "Camera Height",    R.CameraHeight, GUser);

    SectionLabel("WEAPON", GAim);
    Row("mis_recoil", "No Recoil",        R.NoRecoil,   GShield);
    Row("mis_sniper", "Sniper Fast",      R.SniperFast, GBolt);
    Row("mis_fastsens","Fast Sensitivity", R.FastSensitivity, GGauge);
    Slider("mis_sens", "Sensitivity",     R.Sensitivity, 0.0f, 100.0f, "%.0f", GSliders);
    Row("mis_stopfire","Stop Fire Anim",  R.StopFireAnim, GFlame);
    Row("mis_fastsw",  "Fast Weapon Switch", R.FastSwitch, GGear);
    Row("mis_pcswap",  "PC Swap Anim",    R.PcSwapAnim, GGear);

    SectionLabel("VEHICLE", GCar);
    SliderInt("mis_cary", "Car Y Offset", R.CarYOffset, -10, 10, "%d", GCar);
    Row("mis_india",  "Set India (server)", R.SetIndia, GFlag);

    SectionLabel("IDENTITY", GUser);
    Row("mis_spoof",  "Spoof Name",    R.SpoofName,      GUser, "local only");
    Row("mis_level",  "Level Spoof",   R.UserLevelSpoof, GUser);
    Row("mis_reset",  "Guest Reset",   R.GuestReset,     GTrash, "careful");
}

inline void TabSystem(Refs& R)
{
    using namespace ShadowUI;

    SectionLabel("HUD", GEye);
    Row("sys_wm",   "Watermark",        R.Watermark,       GCheck);
    Row("sys_fw",   "Firewall",         R.FirewallOn,      GShield);
    Row("sys_fwv",  "Firewall Verbose", R.FirewallVerbose, GInfo);
    Row("sys_bypass","Login Bypass 400", R.BypassLogin400, GLock);

    SectionLabel("THEME", GPalette);
    {
        // accent presets
        ImRect row = Alloc(S(46.0f));
        if (Visible(row))
        {
            TextAt(lt.dl, "Accent", ImVec2(row.Min.x + S(13.0f), row.Min.y + S(5.0f)), U32(pal.textSoft, 0.95f), 0.86f);
            const float gap = S(7.0f);
            const float size = S(30.0f);
            float total = kAccentCount * size + (kAccentCount - 1) * gap;
            float x = row.GetCenter().x - total * 0.5f;
            for (int i = 0; i < kAccentCount; ++i)
            {
                ImRect c(ImVec2(x, row.GetCenter().y - size * 0.5f + S(6.0f)),
                         ImVec2(x + size, row.GetCenter().y + size * 0.5f + S(6.0f)));
                char id[32]; snprintf(id, sizeof(id), "##accent%d", i);
                const ImU32 uid = P::Id(id);
                P::Claim(uid, c);
                if (P::Tap(uid, c)) { ApplyAccent(i); Toast("ACCENT CHANGED"); }
                const bool sel = (ShadowUI::theme.accentIndex == i);
                const float hv = Anim(uid ^ 0x77u, (P::Hot(uid, c) || sel) ? 1.0f : 0.0f);
                if (sel) GlowRound(lt.dl, c, size * 0.5f, kAccents[i].main, 0.9f * ShadowUI::theme.glowStrength, 3);
                lt.dl->AddCircleFilled(c.GetCenter(), size * 0.36f, U32(kAccents[i].main, 0.55f + 0.45f * hv));
                lt.dl->AddCircle(c.GetCenter(), size * 0.36f, U32(sel ? kAccents[i].hi : pal.borderSoft, 0.9f), 0, S(1.4f));
                x += size + gap;
            }
        }
    }
    Slider("sys_glow",  "Glow Strength", &ShadowUI::theme.glowStrength, 0.0f, 1.60f, "%.2fx", GStar);
    Slider("sys_scale", "UI Scale",      &ShadowUI::theme.userScale,    0.70f, 1.60f, "%.2fx", GGauge);
    Row("sys_launcher", "Show Launcher", &ShadowUI::theme.showLauncher, GRocket);

    SectionLabel("ACCESS", GKey);
    {
        ImRect row = Alloc(S(34.0f));
        if (Visible(row))
        {
            bool unlocked = ShadowAuth::unlocked;
            TextAt(lt.dl, "Access Key", ImVec2(row.Min.x + S(13.0f), row.GetCenter().y - TextSize("A", 0.90f).y * 0.5f),
                   U32(pal.textSoft, 0.95f), 0.90f);
            const char* st = unlocked ? (ShadowAuth::autoAccepted ? "REMEMBERED" : "GRANTED") : "LOCKED";
            ImRect chip(ImVec2(row.Max.x - S(122.0f), row.GetCenter().y - S(11.0f)),
                        ImVec2(row.Max.x - S(13.0f), row.GetCenter().y + S(11.0f)));
            lt.dl->AddRectFilled(chip.Min, chip.Max, U32(unlocked ? pal.ok : pal.danger, 0.18f), S(8.0f));
            lt.dl->AddRect(chip.Min, chip.Max, U32(unlocked ? pal.ok : pal.danger, 0.65f), S(8.0f), 0, S(1.1f));
            TextCenter(lt.dl, chip, st, U32(unlocked ? pal.ok : pal.danger), 0.78f);
        }
    }
    {
        ImRect row = Alloc(S(42.0f)), a, b;
        if (Visible(row))
        {
            SplitRow(row, a, b);
            if (Btn("sys_tg", a, "TELEGRAM", GSend, true))
            {
                if (hooks.openURL) hooks.openURL(kTelegram);
                Toast("OPENING TELEGRAM", pal.accentHi, 2.0f);
            }
            if (Btn("sys_relock", b, "LOCK MENU", GLock, false, true))
            {
                ShadowAuth::unlocked = false;
                ShadowAuth::autoAccepted = false;
                KeyClear();
                keyFail = false;
                scrollY = 0.0f;
                Toast("LOCKED - KEY NEEDED", pal.warn, 2.4f);
            }
        }
    }

    SectionLabel("MENU", GSliders);
    {
        ImRect row = Alloc(S(42.0f)), a, b;
        if (Visible(row))
        {
            SplitRow(row, a, b);
            if (Btn("sys_reset", a, "RESET PANEL", GRight)) { positioned = false; scrollY = 0.0f; Toast("PANEL RESET"); }
            if (Btn("sys_hide", b, "HIDE PANEL", GDown)) { HIDE_PANEL(); }
        }
    }

    SectionLabel("ABOUT", GInfo);
    {
        ImRect row = Alloc(S(52.0f));
        if (Visible(row))
        {
            char about[160];
            (void)R;
            snprintf(about, sizeof(about), "%s  %s   -   %s", kBrand, kVersion, kTelegramNice);
            TextAt(lt.dl, about, ImVec2(row.Min.x + S(13.0f), row.Min.y + S(6.0f)), U32(pal.textDim), 0.76f);
            snprintf(about, sizeof(about), "Every feature wired - draggable + scrollable panel");
            TextAt(lt.dl, about, ImVec2(row.Min.x + S(13.0f), row.Min.y + S(24.0f)), U32(pal.textDim, 0.8f), 0.72f);
        }
    }
}

inline void HIDE_PANEL() { open = false; Toast("MENU HIDDEN - TAP THE BUBBLE"); }

// ---------------------------------------------------------------------------
//  B O D Y
// ---------------------------------------------------------------------------
inline void Body(Refs& R, const ImVec2& screen)
{
    ImDrawList* dl = ImGui::GetWindowDrawList();

    // widgets read the draw list (and the clip) straight from the layout
    // cursor - it must point at something before anything is drawn, otherwise
    // the lock screen and the footer would render nothing at all.
    ShadowUI::BeginLayout(dl, 0.0f, 0.0f, screen.x, 0.0f, screen.y, 0.0f);

    // one time: was the key remembered from a previous session?
    if (!authLoaded)
    {
        authLoaded = true;
        if (ShadowAuth::LoadUnlock()) ShadowAuth::unlocked = true;
    }

    if (ShadowUI::theme.showLauncher && !open) Launcher(R, screen);

    if (!open)
    {
        if (toastText[0] && ImGui::GetTime() < toastUntil)
        {
            ImVec2 t = ShadowUI::TextSize(toastText, 0.9f);
            ImRect tr(ImVec2(screen.x * 0.5f - t.x * 0.5f - ShadowUI::S(18.0f), ShadowUI::S(20.0f)),
                      ImVec2(screen.x * 0.5f + t.x * 0.5f + ShadowUI::S(18.0f), ShadowUI::S(20.0f) + ShadowUI::S(38.0f)));
            ShadowUI::Panel(dl, tr, ShadowUI::S(12.0f), toastCol, 0.9f * ShadowUI::theme.glowStrength,
                            ShadowUI::pal.bgWindow, ShadowUI::Alpha(toastCol, 0.10f));
            ShadowUI::TextCenter(dl, tr, toastText, ShadowUI::U32(toastCol), 0.9f);
        }
        return;
    }

    // ---- responsive window ----
    const float maxW = ImMax(ShadowUI::S(150.0f), screen.x - ShadowUI::S(24.0f));
    const float maxH = ImMax(ShadowUI::S(150.0f), screen.y - ShadowUI::S(20.0f));
    float ww = ImClamp(screen.x * 0.60f, ImMin(ShadowUI::S(430.0f), maxW), maxW);
    float wh = ImClamp(screen.y * 0.80f, ImMin(ShadowUI::S(320.0f), maxH), maxH);
    if (ww > ShadowUI::S(560.0f)) ww = ImMin(ShadowUI::S(560.0f), maxW);
    if (wh > ShadowUI::S(640.0f)) wh = ImMin(ShadowUI::S(640.0f), maxH);
    ShadowUI::theme.menuWidth  = ww;
    ShadowUI::theme.menuHeight = wh;

    if (!positioned)
    {
        menuX = (screen.x - ww) * 0.5f;
        menuY = (screen.y - wh) * 0.5f;
        positioned = true;
    }
    else
    {
        menuX = ImClamp(menuX, ShadowUI::S(6.0f), screen.x - ww - ShadowUI::S(6.0f));
        menuY = ImClamp(menuY, ShadowUI::S(6.0f), screen.y - wh - ShadowUI::S(6.0f));
    }

    ImRect win(ImVec2(menuX, menuY), ImVec2(menuX + ww, menuY + wh));
    windowRect = win;

    // dimmed backdrop so the neon pops (the game stays visible)
    dl->AddRectFilled(ImVec2(0, 0), screen, IM_COL32(0, 0, 0, 70));

    // ---- window frame ----
    ShadowUI::GlowRound(dl, win, ShadowUI::S(ShadowUI::theme.radius), ShadowUI::pal.accent,
                        1.1f * ShadowUI::theme.glowStrength, 5);
    ShadowUI::Panel(dl, win, ShadowUI::S(ShadowUI::theme.radius), ShadowUI::pal.accent, 0.0f,
                    ShadowUI::pal.bgWindow, ShadowUI::pal.bgWindow2, ShadowUI::theme.windowOpacity);

    const float headerH = ShadowUI::S(54.0f);
    const float tabH    = ShadowUI::S(52.0f);
    const float footH   = ShadowUI::S(54.0f);

    ImRect header(win.Min, ImVec2(win.Max.x, win.Min.y + headerH));
    ImRect tabbar(ImVec2(win.Min.x + ShadowUI::S(12.0f), header.Max.y + ShadowUI::S(5.0f)),
                  ImVec2(win.Max.x - ShadowUI::S(12.0f), header.Max.y + ShadowUI::S(5.0f) + tabH));
    ImRect footer(ImVec2(win.Min.x, win.Max.y - footH), win.Max);
    ImRect body(ImVec2(win.Min.x, tabbar.Max.y + ShadowUI::S(6.0f)),
                ImVec2(win.Max.x, footer.Min.y - ShadowUI::S(4.0f)));
    ImRect content(ImVec2(body.Min.x + ShadowUI::S(12.0f), body.Min.y),
                   ImVec2(body.Max.x - ShadowUI::S(12.0f), body.Max.y - ShadowUI::S(2.0f)));

    ShadowUI::Separator(dl, ImRect(ImVec2(win.Min.x + ShadowUI::S(10.0f), header.Max.y - ShadowUI::S(0.5f)),
                                   ImVec2(win.Max.x - ShadowUI::S(10.0f), header.Max.y + ShadowUI::S(0.5f))),
                        ShadowUI::pal.accent, 1.0f);

    Header(dl, R, header);

    if (!ShadowAuth::unlocked)
    {
        LockScreen(dl, ImRect(ImVec2(body.Min.x, body.Min.y), ImVec2(body.Max.x, footer.Min.y)));
    }
    else
    {
        TabBar(dl, tabbar);

        // ---- scrolling ----
        const ImU32 scrollId = ShadowUI::P::Id("##smdz_scroll");
        ShadowUI::P::Note(scrollId, content);
        ShadowUI::P::SetPressArea(content);
        bool dragScroll = false;

        // a vertical stroke that landed on the list (or on empty space inside it)
        // becomes a scroll - but a stroke that started on a widget outside the
        // list (the header, a tab, the footer) keeps its own widget
        if (ShadowUI::P::st.down && ShadowUI::P::st.moved &&
            ShadowUI::P::st.owner != scrollId &&
            ShadowUI::P::st.pressArea.Contains(ShadowUI::P::st.downPos) &&
            ShadowUI::P::OwnerWasInsidePressArea())
        {
            float dx = ShadowUI::P::st.pos.x - ShadowUI::P::st.downPos.x;
            float dy = ShadowUI::P::st.pos.y - ShadowUI::P::st.downPos.y;
            if (fabsf(dy) > fabsf(dx)) ShadowUI::P::Steal(scrollId);
        }
        if (ShadowUI::P::Owns(scrollId) && ShadowUI::P::st.down)
        {
            dragScroll = true;
            scrollY -= ShadowUI::P::st.delta.y;
        }
        ImGuiIO& io = ImGui::GetIO();
        if (content.Contains(ShadowUI::P::st.pos) && !ShadowUI::P::st.down && io.MouseWheel != 0.0f)
            scrollY -= io.MouseWheel * ShadowUI::S(52.0f);

        scrollY = ImClamp(scrollY, 0.0f, ImMax(0.0f, scrollMax));

        // ---- content ----
        const float top = content.Min.y - scrollY;
        ShadowUI::BeginLayout(dl, content.Min.x, top, content.GetWidth(),
                              content.Min.y, content.Max.y, scrollY);

        dl->PushClipRect(content.Min, content.Max, true);
        switch (tab)
        {
            case 0: TabAim(R);    break;
            case 1: TabEsp(R);    break;
            case 2: TabCatch(R);  break;
            case 3: TabMisc(R);   break;
            default: TabSystem(R); break;
        }
        ShadowUI::ContentEnd();
        dl->PopClipRect();

        scrollMax = ImMax(0.0f, ShadowUI::lt.contentH - content.GetHeight());

        // scrollbar
        if (scrollMax > ShadowUI::S(2.0f))
        {
            float vis = content.GetHeight() / (content.GetHeight() + scrollMax);
            float barH = ImMax(ShadowUI::S(34.0f), content.GetHeight() * vis);
            float t = scrollY / scrollMax;
            ImRect bar(ImVec2(content.Max.x + ShadowUI::S(2.0f), content.Min.y + (content.GetHeight() - barH) * t),
                       ImVec2(content.Max.x + ShadowUI::S(6.0f), content.Min.y + (content.GetHeight() - barH) * t + barH));
            dl->AddRectFilled(bar.Min, bar.Max, ShadowUI::U32(ShadowUI::pal.accent, 0.55f), ShadowUI::S(2.0f));
        }

        // soft fade so rows do not end abruptly at the clip edge
        dl->AddRectFilledMultiColor(ImVec2(content.Min.x, content.Min.y),
                                    ImVec2(content.Max.x, content.Min.y + ShadowUI::S(10.0f)),
                                    ShadowUI::U32(ShadowUI::pal.bgWindow, 0.75f), ShadowUI::U32(ShadowUI::pal.bgWindow, 0.75f),
                                    ShadowUI::U32(ShadowUI::pal.bgWindow, 0.0f),  ShadowUI::U32(ShadowUI::pal.bgWindow, 0.0f));
        if (scrollY < scrollMax - ShadowUI::S(2.0f))
            dl->AddRectFilledMultiColor(ImVec2(content.Min.x, content.Max.y - ShadowUI::S(14.0f)),
                                        ImVec2(content.Max.x, content.Max.y),
                                        ShadowUI::U32(ShadowUI::pal.bgWindow, 0.0f),  ShadowUI::U32(ShadowUI::pal.bgWindow, 0.0f),
                                        ShadowUI::U32(ShadowUI::pal.bgWindow, 0.85f), ShadowUI::U32(ShadowUI::pal.bgWindow, 0.85f));
    }

    // ---- footer ----
    {
        ShadowUI::Separator(dl, ImRect(ImVec2(win.Min.x + ShadowUI::S(10.0f), footer.Min.y - ShadowUI::S(0.5f)),
                                       ImVec2(win.Max.x - ShadowUI::S(10.0f), footer.Min.y + ShadowUI::S(0.5f))),
                            ShadowUI::pal.accent, 0.8f);

        ImRect a, b, c;
        {
            const float g = ShadowUI::S(6.0f);
            const float pad = ShadowUI::S(12.0f);
            float w = (footer.GetWidth() - pad * 2.0f - g * 2.0f) / 3.0f;
            float y0 = footer.Min.y + ShadowUI::S(8.0f), y1 = footer.Max.y - ShadowUI::S(8.0f);
            a = ImRect(ImVec2(footer.Min.x + pad, y0), ImVec2(footer.Min.x + pad + w, y1));
            b = ImRect(ImVec2(a.Max.x + g, y0), ImVec2(a.Max.x + g + w, y1));
            c = ImRect(ImVec2(b.Max.x + g, y0), ImVec2(b.Max.x + g + w, y1));
        }

        if (ShadowUI::Btn("##foot_hide", a, "HIDE", ShadowUI::GDown))
            HIDE_PANEL();
        if (ShadowUI::Btn("##foot_exit", b, "EXIT MOD", ShadowUI::GPower, false, true))
        {
            if (R.Enable) *R.Enable = false;
            HIDE_PANEL();
            Toast("MOD DISABLED", ShadowUI::pal.danger, 2.6f);
        }
        if (ShadowUI::Btn("##foot_join", c, "JOIN", ShadowUI::GSend, true))
        {
            if (hooks.openURL) hooks.openURL(kTelegram);
            Toast("OPENING TELEGRAM", ShadowUI::pal.accentHi, 2.0f);
        }
    }

    // ---- toast under the window ----
    if (toastText[0] && ImGui::GetTime() < toastUntil)
    {
        ImVec2 t = ShadowUI::TextSize(toastText, 0.86f);
        ImRect tr(ImVec2(win.GetCenter().x - t.x * 0.5f - ShadowUI::S(18.0f), win.Max.y + ShadowUI::S(10.0f)),
                  ImVec2(win.GetCenter().x + t.x * 0.5f + ShadowUI::S(18.0f), win.Max.y + ShadowUI::S(10.0f) + ShadowUI::S(36.0f)));
        if (tr.Max.y < screen.y - ShadowUI::S(4.0f))
        {
            ShadowUI::Panel(dl, tr, ShadowUI::S(12.0f), toastCol, 0.9f * ShadowUI::theme.glowStrength,
                            ShadowUI::pal.bgWindow, ShadowUI::Alpha(toastCol, 0.10f));
            ShadowUI::TextCenter(dl, tr, toastText, ShadowUI::U32(toastCol), 0.86f);
        }
    }

    if (ShadowUI::theme.showStatusBar)
    {
        char info[128];
        snprintf(info, sizeof(info), "%s %s   %.0fx%.0f   %d ON", kBrand, kVersion, screen.x, screen.y, EnabledCount(R));
        ShadowUI::TextAt(dl, info, ImVec2(win.Min.x + ShadowUI::S(6.0f), win.Max.y + ShadowUI::S(4.0f)),
                         ShadowUI::U32(ShadowUI::pal.textDim), 0.72f);
    }
}

// ---------------------------------------------------------------------------
//  D R A W  -  entry point called from the EGL swap hook
// ---------------------------------------------------------------------------
inline void Draw(Refs& R, float screenW, float screenH)
{
    if (screenW <= 1.0f || screenH <= 1.0f) return;
    if (!ImGui::GetCurrentContext()) return;

    // every metric is derived from this one scale
    ShadowUI::theme.uiScale = ShadowUI::theme.userScale * ImClamp(ImMin(screenW / 720.0f, screenH / 1280.0f), 0.75f, 2.00f);

    ShadowUI::P::BeginFrame();

    const ImVec2 screen(screenW, screenH);

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(screen);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
    ImGui::Begin("##shadowmodz_canvas", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
                 ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                 ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration |
                 ImGuiWindowFlags_NoInputs);
    {
        Body(R, screen);
    }
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ShadowUI::P::EndFrame();
}

} // namespace ShadowMenu
