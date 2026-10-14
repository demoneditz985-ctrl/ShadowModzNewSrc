#pragma once
// ============================================================================
//  SHADOW MODZ - UI DESIGN SYSTEM
//  ---------------------------------------------------------------------------
//  Dark black + purple theme for the floating in-game menu.
//  Everything in this file is pure ImGui (no Android / Il2Cpp dependency) so
//  the whole design system can be previewed and unit checked on a desktop.
//
//  Telegram : https://t.me/+BBimnHMiSvpiYTBl
// ============================================================================

#include <imgui.h>
#include <imgui_internal.h>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

// FontAwesome icon codepoints (provided by the jni include directory)
#include "Icon/Icon.h"

namespace ShadowUI
{

// ---------------------------------------------------------------------------
//  Palette  -  BLACK + PURPLE
// ---------------------------------------------------------------------------
struct Palette
{
    ImVec4 accent      = ImVec4(0.655f, 0.333f, 0.969f, 1.000f); // #A755F7
    ImVec4 accentDeep  = ImVec4(0.365f, 0.161f, 0.780f, 1.000f); // #5D29C7
    ImVec4 accentHi    = ImVec4(0.796f, 0.596f, 0.996f, 1.000f); // #CB98FE
    ImVec4 pink        = ImVec4(0.910f, 0.475f, 0.976f, 1.000f); // #E879F9

    ImVec4 bgRoot      = ImVec4(0.027f, 0.020f, 0.043f, 1.000f); // #07050B
    ImVec4 bgWindow    = ImVec4(0.043f, 0.035f, 0.067f, 0.960f); // #0B0911
    ImVec4 bgPanel     = ImVec4(0.063f, 0.051f, 0.094f, 0.880f); // #100D18
    ImVec4 bgCard      = ImVec4(0.078f, 0.063f, 0.118f, 0.900f);
    ImVec4 bgCardHi    = ImVec4(0.110f, 0.086f, 0.169f, 0.950f);
    ImVec4 bgRow       = ImVec4(0.094f, 0.075f, 0.145f, 0.700f);
    ImVec4 rowHover    = ImVec4(0.157f, 0.118f, 0.259f, 0.850f);

    ImVec4 text        = ImVec4(0.929f, 0.910f, 0.992f, 1.000f);
    ImVec4 textSoft    = ImVec4(0.760f, 0.733f, 0.847f, 1.000f);
    ImVec4 textDim     = ImVec4(0.510f, 0.478f, 0.604f, 1.000f);
    ImVec4 border      = ImVec4(0.404f, 0.250f, 0.663f, 0.470f);
    ImVec4 borderSoft  = ImVec4(0.290f, 0.180f, 0.470f, 0.330f);

    ImVec4 ok          = ImVec4(0.290f, 0.870f, 0.529f, 1.000f);
    ImVec4 warn        = ImVec4(0.984f, 0.749f, 0.141f, 1.000f);
    ImVec4 danger      = ImVec4(0.957f, 0.247f, 0.369f, 1.000f);
};

inline Palette pal;

// purple accent presets (Settings -> ACCENT)
struct AccentPreset { const char* name; ImVec4 main; ImVec4 deep; ImVec4 hi; };
inline const AccentPreset kAccents[] =
{
    { "NEBULA",  ImVec4(0.655f, 0.333f, 0.969f, 1.0f), ImVec4(0.365f, 0.161f, 0.780f, 1.0f), ImVec4(0.796f, 0.596f, 0.996f, 1.0f) },
    { "VIOLET",  ImVec4(0.545f, 0.361f, 0.965f, 1.0f), ImVec4(0.310f, 0.180f, 0.760f, 1.0f), ImVec4(0.737f, 0.612f, 0.996f, 1.0f) },
    { "ORCHID",  ImVec4(0.796f, 0.353f, 0.937f, 1.0f), ImVec4(0.545f, 0.145f, 0.729f, 1.0f), ImVec4(0.898f, 0.639f, 0.980f, 1.0f) },
    { "AMETHYST",ImVec4(0.596f, 0.278f, 0.812f, 1.0f), ImVec4(0.376f, 0.145f, 0.573f, 1.0f), ImVec4(0.780f, 0.545f, 0.925f, 1.0f) },
    { "ROYAL",   ImVec4(0.451f, 0.278f, 0.949f, 1.0f), ImVec4(0.239f, 0.129f, 0.702f, 1.0f), ImVec4(0.647f, 0.545f, 0.984f, 1.0f) },
    { "MAGENTA", ImVec4(0.878f, 0.290f, 0.780f, 1.0f), ImVec4(0.612f, 0.129f, 0.588f, 1.0f), ImVec4(0.949f, 0.588f, 0.898f, 1.0f) },
};
inline const int kAccentCount = IM_ARRAYSIZE(kAccents);

struct Theme
{
    float uiScale         = 1.00f;   // scales every widget + font
    float windowOpacity   = 1.00f;
    float menuWidth       = 726.0f;
    float menuHeight      = 512.0f;
    float launcherRadius  = 33.0f;
    bool  showFps         = true;
    bool  showStatusBar   = true;
    bool  showLauncher    = true;
    bool  compactRows     = false;
    int   accentIndex     = 0;
    float glowStrength    = 1.00f;
};

inline Theme theme;

// ---------------------------------------------------------------------------
//  small helpers
// ---------------------------------------------------------------------------
inline ImVec4 Alpha(const ImVec4& c, float a) { ImVec4 r = c; r.w = a; return r; }
inline ImU32  U32(const ImVec4& c)            { return ImGui::ColorConvertFloat4ToU32(c); }
inline ImU32  U32(const ImVec4& c, float a)   { ImVec4 r = c; r.w = a; return ImGui::ColorConvertFloat4ToU32(r); }
inline ImU32  U32Lerp(ImU32 a, ImU32 b, float t)
{
    return ImGui::ColorConvertFloat4ToU32(
        ImLerp(ImGui::ColorConvertU32ToFloat4(a), ImGui::ColorConvertU32ToFloat4(b), ImClamp(t, 0.0f, 1.0f)));
}
inline float  S(float v) { return v * theme.uiScale; }
inline float  AnimSpeed() { return ImClamp(ImGui::GetIO().DeltaTime * 14.0f, 0.0f, 1.0f); }
inline float& AnimRef(ImGuiID id, float init)
{
    return *ImGui::GetStateStorage()->GetFloatRef(id, init);
}

inline void ApplyAccent(int index)
{
    if (index < 0 || index >= kAccentCount) index = 0;
    theme.accentIndex = index;
    pal.accent     = kAccents[index].main;
    pal.accentDeep = kAccents[index].deep;
    pal.accentHi   = kAccents[index].hi;
    pal.border     = Alpha(pal.accent, 0.47f);
    pal.borderSoft = Alpha(pal.accent, 0.28f);
}

// ---------------------------------------------------------------------------
//  ImGui style (called every frame, absolute values only - never cumulative)
// ---------------------------------------------------------------------------
inline void ApplyStyle()
{
    ImGuiStyle& s = ImGui::GetStyle();
    const float k = theme.uiScale;

    s.WindowPadding     = ImVec2(0.0f, 0.0f);
    s.WindowRounding    = 14.0f * k;
    s.WindowBorderSize  = 0.0f;
    s.ChildRounding     = 10.0f * k;
    s.ChildBorderSize   = 1.0f;
    s.PopupRounding     = 10.0f * k;
    s.PopupBorderSize   = 1.0f;
    s.FramePadding      = ImVec2(10.0f * k, 6.0f * k);
    s.FrameRounding     = 8.0f * k;
    s.ItemSpacing       = ImVec2(8.0f * k, 7.0f * k);
    s.ItemInnerSpacing  = ImVec2(7.0f * k, 5.0f * k);
    s.CellPadding       = ImVec2(6.0f * k, 4.0f * k);
    s.ScrollbarSize     = 9.0f * k;
    s.ScrollbarRounding = 9.0f * k;
    s.GrabMinSize       = 18.0f * k;
    s.GrabRounding      = 8.0f * k;
    s.TabRounding       = 8.0f * k;

    ImVec4* c = s.Colors;
    c[ImGuiCol_WindowBg]            = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // drawn by the menu frame
    c[ImGuiCol_ChildBg]             = Alpha(pal.bgPanel, pal.bgPanel.w * theme.windowOpacity);
    c[ImGuiCol_PopupBg]             = Alpha(pal.bgCardHi, 0.98f * theme.windowOpacity);
    c[ImGuiCol_Border]              = pal.borderSoft;
    c[ImGuiCol_BorderShadow]        = ImVec4(0, 0, 0, 0);
    c[ImGuiCol_Text]                = pal.text;
    c[ImGuiCol_TextDisabled]        = pal.textDim;
    c[ImGuiCol_FrameBg]             = pal.bgRow;
    c[ImGuiCol_FrameBgHovered]      = pal.rowHover;
    c[ImGuiCol_FrameBgActive]       = Alpha(pal.accentDeep, 0.75f);
    c[ImGuiCol_Button]              = Alpha(pal.accentDeep, 0.90f);
    c[ImGuiCol_ButtonHovered]       = pal.accent;
    c[ImGuiCol_ButtonActive]        = pal.accentHi;
    c[ImGuiCol_CheckMark]           = pal.accent;
    c[ImGuiCol_SliderGrab]          = pal.accent;
    c[ImGuiCol_SliderGrabActive]    = pal.accentHi;
    c[ImGuiCol_Header]              = Alpha(pal.accentDeep, 0.55f);
    c[ImGuiCol_HeaderHovered]       = Alpha(pal.accent, 0.55f);
    c[ImGuiCol_HeaderActive]        = Alpha(pal.accent, 0.80f);
    c[ImGuiCol_Separator]           = pal.borderSoft;
    c[ImGuiCol_SeparatorHovered]    = Alpha(pal.accent, 0.70f);
    c[ImGuiCol_SeparatorActive]     = pal.accent;
    c[ImGuiCol_ScrollbarBg]         = ImVec4(0.024f, 0.020f, 0.039f, 0.35f * theme.windowOpacity);
    c[ImGuiCol_ScrollbarGrab]       = Alpha(pal.accentDeep, 0.85f);
    c[ImGuiCol_ScrollbarGrabHovered]= pal.accent;
    c[ImGuiCol_ScrollbarGrabActive] = pal.accentHi;
    c[ImGuiCol_Tab]                 = Alpha(pal.bgCard, 0.85f);
    c[ImGuiCol_TabHovered]          = Alpha(pal.accent, 0.55f);
    c[ImGuiCol_TabActive]           = Alpha(pal.accentDeep, 0.95f);
    c[ImGuiCol_NavHighlight]        = pal.accent;
    c[ImGuiCol_TextSelectedBg]      = Alpha(pal.accent, 0.45f);
}

// ---------------------------------------------------------------------------
//  drawing primitives
// ---------------------------------------------------------------------------
inline void GlowRect(ImDrawList* dl, ImVec2 a, ImVec2 b, float rounding, ImVec4 col, float strength, int layers = 3)
{
    if (!dl) return;
    for (int i = layers; i >= 1; --i)
    {
        float grow = (float)i * S(3.0f);
        dl->AddRectFilled(ImVec2(a.x - grow, a.y - grow), ImVec2(b.x + grow, b.y + grow),
                          U32(col, col.w * 0.11f * strength / (float)i), rounding + grow);
    }
}

inline void GlowCircle(ImDrawList* dl, ImVec2 c, float r, ImVec4 col, float strength, int layers = 4)
{
    if (!dl) return;
    for (int i = layers; i >= 1; --i)
    {
        float rr = r + (float)i * S(3.0f);
        dl->AddCircleFilled(c, rr, U32(col, col.w * 0.10f * strength / (float)i), 48);
    }
}

inline void GradientV(ImDrawList* dl, ImVec2 a, ImVec2 b, ImU32 top, ImU32 bottom, float rounding)
{
    if (!dl) return;
    dl->AddRectFilled(a, b, top, rounding);
    if (rounding > 0.0f)
        dl->AddRectFilledMultiColor(ImVec2(a.x, a.y + rounding * 0.5f), b, top, top, bottom, bottom);
    else
        dl->AddRectFilledMultiColor(a, b, top, top, bottom, bottom);
}

inline void GradientH(ImDrawList* dl, ImVec2 a, ImVec2 b, ImU32 left, ImU32 right, float rounding)
{
    if (!dl) return;
    dl->AddRectFilledMultiColor(a, b, left, right, right, left);
    if (rounding > 0.0f)
        dl->AddRect(a, b, left, rounding, 0, 0.0f);
}

inline void ShadowPanel(ImDrawList* dl, ImVec2 a, ImVec2 b, float rounding, ImVec4 fill, ImVec4 edge, float glow)
{
    if (!dl) return;
    dl->AddRectFilled(ImVec2(a.x + 1.0f, a.y + 2.0f), ImVec2(b.x + 1.0f, b.y + 2.0f), IM_COL32(0, 0, 0, 90), rounding);
    dl->AddRectFilled(a, b, U32(fill), rounding);
    if (glow > 0.0f)
        GlowRect(dl, a, b, rounding, edge, glow, 2);
    dl->AddRect(a, b, U32(edge), rounding, 0, S(1.0f));
}

inline void Separator(ImDrawList* dl, ImVec2 a, ImVec2 b, ImVec4 col)
{
    if (!dl) return;
    dl->AddLine(a, b, U32(col), S(1.0f));
}

// subtle dotted grid used behind the menu body
inline void DotGrid(ImDrawList* dl, ImVec2 a, ImVec2 b, ImVec4 col, float step)
{
    if (!dl) return;
    ImU32 c = U32(col);
    for (float y = a.y; y < b.y; y += step)
        for (float x = a.x; x < b.x; x += step)
            dl->AddRectFilled(ImVec2(x, y), ImVec2(x + S(1.0f), y + S(1.0f)), c);
}

// ---------------------------------------------------------------------------
//  brand logo (OpenGL texture uploaded from the embedded JPEG)
//  ShadowLogo_Texture() lives in shadowmodz_logo.cpp; it returns nullptr until
//  the GL context exists, so every caller must be able to fall back.
// ---------------------------------------------------------------------------
inline void* LogoTexture   = nullptr;
inline float LogoAspect    = 1.0f;

// OpenGL upload of the embedded branding logo (shadowmodz_logo.cpp).
// Returns false until a GL context exists / if decoding fails - the UI then
// draws the vector "SM" mark instead.
bool CreateLogoTexture();
void ReleaseLogoTexture();

inline void LogoDraw(ImDrawList* dl, ImVec2 center, float size, float rounding, bool glow = true)
{
    if (!dl) return;
    const float r = size * 0.5f;
    ImVec2 a(center.x - r, center.y - r);
    ImVec2 b(center.x + r, center.y + r);

    if (glow)
        GlowCircle(dl, center, r, pal.accent, 1.0f * theme.glowStrength, 3);

    if (LogoTexture)
    {
        dl->AddImageRounded((ImTextureID)LogoTexture, a, b, ImVec2(0, 0), ImVec2(1, 1),
                            IM_COL32(255, 255, 255, 255), rounding);
    }
    else
    {
        // fallback mark (also used on the desktop preview build)
        GradientV(dl, a, b, U32(pal.accent), U32(pal.accentDeep), rounding);
        const char* mark = "SM";
        ImVec2 ts = ImGui::CalcTextSize(mark);
        dl->AddText(ImVec2(center.x - ts.x * 0.5f, center.y - ts.y * 0.5f),
                    IM_COL32(255, 255, 255, 255), mark);
    }
    dl->AddCircle(center, r - S(1.0f), U32(pal.accentHi, 0.85f), 0, S(1.6f));
    dl->AddCircle(center, r + S(2.0f), U32(pal.accent, 0.35f), 0, S(2.0f));
}

// ---------------------------------------------------------------------------
//  touch scrolling (drag / flick) for any child window
// ---------------------------------------------------------------------------
struct ScrollTouch
{
    float         velocity = 0.0f;
    ImGuiWindow*  target   = nullptr;
    int           cooldown = 0;
};

inline ScrollTouch& ScrollState()
{
    static ScrollTouch s;
    return s;
}

// call right after BeginChild()
inline void TouchScroll(bool allow = true)
{
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindow* win = ImGui::GetCurrentWindow();
    if (!win) return;

    ScrollTouch& st = ScrollState();
    if (st.cooldown > 0) --st.cooldown;

    // hit test against the child's own clip rect: keeps working while another
    // window owns the active id (e.g. mid gesture) and never steals the menu
    const ImVec4 cr = win->ClipRect.ToVec4();
    const bool inside = io.MousePos.x >= cr.x && io.MousePos.x <= cr.z &&
                        io.MousePos.y >= cr.y && io.MousePos.y <= cr.w;

    // a finger resting on a row keeps that row "active", so we simply take the
    // gesture over as soon as it becomes a vertical drag (rows cancel their own
    // press at the same threshold, so nothing fires twice)
    const bool dragging = ImGui::IsMouseDragging(0);
    const bool free     = !ImGui::IsAnyItemActive() || dragging;

    if (allow && inside && free && io.MouseDown[0])
    {
        const float dy = io.MouseDelta.y;
        const float dx = io.MouseDelta.x;
        if (fabsf(dy) > 0.0f && fabsf(dy) >= fabsf(dx))
        {
            ImGui::SetScrollY(ImGui::GetScrollY() - dy);
            st.velocity = -dy;
            st.target   = win;
            st.cooldown = 0;
            if (dragging)
                ImGui::ClearActiveID();   // stop the held row from firing
        }
    }

    if (st.target == win && !io.MouseDown[0])
    {
        if (fabsf(st.velocity) > S(0.6f))
        {
            ImGui::SetScrollY(ImGui::GetScrollY() + st.velocity);
            st.velocity *= 0.86f;
            if (fabsf(st.velocity) < S(0.6f)) st.velocity = 0.0f;
        }
        else
        {
            st.velocity = 0.0f;
        }
    }
}

// ---------------------------------------------------------------------------
//  text helpers
// ---------------------------------------------------------------------------
inline void TextShadow(ImDrawList* dl, ImVec2 p, ImVec4 col, const char* txt, float shadow = 1.0f)
{
    if (!dl) return;
    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(p.x + S(shadow), p.y + S(shadow)),
                IM_COL32(0, 0, 0, 160), txt);
    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), p, U32(col), txt);
}

inline void Label(ImDrawList* dl, ImVec2 p, ImVec4 col, const char* txt, const ImVec4* clip = nullptr)
{
    if (!dl) return;
    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), p, U32(col), txt, nullptr, 0.0f, clip);
}

inline void Chip(ImVec2 pos, const char* txt, ImVec4 col, bool filled = true, const char* icon = nullptr)
{
    ImDrawList* dl = ImGui::GetWindowDrawList();
    char buf[128];
    if (icon && icon[0]) snprintf(buf, sizeof(buf), "%s %s", icon, txt);
    else                 snprintf(buf, sizeof(buf), "%s", txt);

    ImVec2 ts = ImGui::CalcTextSize(buf);
    ImVec2 a(pos.x, pos.y);
    ImVec2 b(pos.x + ts.x + S(16.0f), pos.y + ts.y + S(6.0f));
    if (filled) dl->AddRectFilled(a, b, U32(col, 0.22f), S(6.0f));
    dl->AddRect(a, b, U32(col, 0.75f), S(6.0f), 0, S(1.0f));
    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(a.x + S(8.0f), a.y + S(3.0f)), U32(col), buf);
    ImGui::SetCursorScreenPos(ImVec2(b.x, a.y));
}

// ---------------------------------------------------------------------------
//  card container  (background drawn behind the content via draw channels)
// ---------------------------------------------------------------------------
struct CardCtx
{
    bool   open = false;
    ImVec2 startPos;
    float  width = 0.0f;
};

inline CardCtx& Card()
{
    static CardCtx c;
    return c;
}

inline bool CardBegin(const char* id, const char* title, const char* icon = nullptr,
                      const char* badge = nullptr, ImVec4 badgeCol = ImVec4(0, 0, 0, 0))
{
    CardCtx& ctx = Card();
    ImGui::PushID(id);

    ImDrawList* dl = ImGui::GetWindowDrawList();
    dl->ChannelsSplit(2);
    // NOTE: in this ImGui build the base channel (0) is emitted first and the
    // upper channels are appended after it, so the *background* must live in
    // channel 0 and the card content in channel 1.
    dl->ChannelsSetCurrent(1);          // channel 1 = content (on top)

    ImGui::BeginGroup();

    const float padX = S(12.0f);
    const float rowH = S(34.0f);
    ImVec2 p = ImGui::GetCursorScreenPos();
    ctx.startPos = ImVec2(p.x - padX, p.y - S(8.0f));
    ctx.width    = ImGui::GetContentRegionAvail().x + padX * 2.0f;

    // header row
    ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x, rowH));
    {
        ImVec2 hp(p.x, p.y);
        // accent bar
        dl->AddRectFilled(ImVec2(hp.x, hp.y + S(6.0f)), ImVec2(hp.x + S(3.0f), hp.y + rowH - S(6.0f)),
                          U32(pal.accent), S(1.5f));
        if (icon && icon[0])
            dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(hp.x + S(11.0f), hp.y + S(9.0f)),
                        U32(pal.accentHi), icon);
        float tx = hp.x + (icon && icon[0] ? S(30.0f) : S(11.0f));
        dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(tx, hp.y + S(9.0f)), U32(pal.text), title);

        if (badge && badge[0])
        {
            ImVec4 bc = badgeCol.w > 0.0f ? badgeCol : pal.accent;
            float tw = ImGui::CalcTextSize(badge).x;
            ImVec2 ba(p.x + ImGui::GetContentRegionAvail().x - tw - S(14.0f), hp.y + S(10.0f));
            dl->AddRectFilled(ba, ImVec2(ba.x + tw + S(12.0f), ba.y + S(15.0f)), U32(bc, 0.20f), S(4.0f));
            dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(ba.x + S(6.0f), ba.y - S(2.0f)), U32(bc), badge);
        }
        Separator(dl, ImVec2(ctx.startPos.x + padX, hp.y + rowH - S(4.0f)),
                      ImVec2(ctx.startPos.x + ctx.width - padX, hp.y + rowH - S(4.0f)), pal.borderSoft);
    }

    ImGui::Indent(padX);
    ctx.open = true;
    return true;
}

inline void CardEnd()
{
    CardCtx& ctx = Card();
    if (!ctx.open) return;

    ImGui::Unindent(S(12.0f));
    ImGui::EndGroup();

    ImVec2 mn = ImGui::GetItemRectMin();
    ImVec2 mx = ImGui::GetItemRectMax();

    ImDrawList* dl = ImGui::GetWindowDrawList();
    dl->ChannelsSetCurrent(0);          // channel 0 = background (behind content)

    ImVec2 a(mn.x - S(12.0f), mn.y - S(8.0f));
    ImVec2 b(ImMax(mx.x + S(12.0f), a.x + S(60.0f)), mx.y + S(10.0f));
    float rounding = S(11.0f);

    dl->AddRectFilled(ImVec2(a.x + S(1.0f), a.y + S(2.0f)), ImVec2(b.x + S(1.0f), b.y + S(2.0f)), IM_COL32(0, 0, 0, 70), rounding);
    GradientV(dl, a, b, U32(pal.bgCardHi, pal.bgCardHi.w * theme.windowOpacity),
                       U32(pal.bgCard, pal.bgCard.w * theme.windowOpacity), rounding);
    dl->AddRect(a, b, U32(pal.borderSoft), rounding, 0, S(1.0f));
    // top highlight line
    dl->AddRectFilledMultiColor(ImVec2(a.x + rounding, a.y), ImVec2(b.x - rounding, a.y + S(1.4f)),
                                U32(pal.accent, 0.0f), U32(pal.accent, 0.55f), U32(pal.accent, 0.55f), U32(pal.accent, 0.0f));

    dl->ChannelsMerge();
    ctx.open = false;
    ImGui::PopID();
    ImGui::Dummy(ImVec2(1.0f, S(9.0f)));
}

inline void SectionTitle(const char* txt, const char* icon = nullptr)
{
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    float h = S(20.0f);
    float avail = ImGui::GetContentRegionAvail().x;

    ImGui::Dummy(ImVec2(avail, h));

    float tx = p.x + S(2.0f);
    if (icon && icon[0])
    {
        dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(tx, p.y + S(1.0f)), U32(pal.accent), icon);
        tx += S(20.0f);
    }
    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(tx, p.y + S(1.0f)), U32(pal.textSoft), txt);
    float lx = tx + ImGui::CalcTextSize(txt).x + S(10.0f);
    if (lx < p.x + avail)
        Separator(dl, ImVec2(lx, p.y + h * 0.5f), ImVec2(p.x + avail, p.y + h * 0.5f), pal.borderSoft);
}

inline void InfoRow(const char* label, const char* value, ImVec4 valueCol, bool mono = false)
{
    ImDrawList* dl = ImGui::GetWindowDrawList();
    const float h = S(theme.compactRows ? 22.0f : 26.0f);
    float avail = ImGui::GetContentRegionAvail().x;
    ImVec2 p = ImGui::GetCursorScreenPos();

    dl->AddRectFilled(p, ImVec2(p.x + avail, p.y + h), U32(pal.bgRow, 0.55f), S(6.0f));
    float ty = p.y + (h - ImGui::GetFontSize()) * 0.5f;
    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(p.x + S(10.0f), ty),
                U32(mono ? pal.textSoft : pal.textDim), label);
    float tw = ImGui::CalcTextSize(value).x;
    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(p.x + avail - tw - S(10.0f), ty), U32(valueCol), value);

    ImGui::Dummy(ImVec2(avail, h + S(2.0f)));
}

// ---------------------------------------------------------------------------
//  widgets
// ---------------------------------------------------------------------------
// bounds of the last interactive row/track - lets callers (and tests) know
// exactly where a widget can be touched
inline ImVec2 LastItemMin(0.0f, 0.0f);
inline ImVec2 LastItemMax(0.0f, 0.0f);
inline void   SetLastItem() { LastItemMin = ImGui::GetItemRectMin(); LastItemMax = ImGui::GetItemRectMax(); }
inline bool ToggleRow(const char* label, bool* v, const char* hint = nullptr, const char* icon = nullptr)
{
    if (!v) return false;
    ImGui::PushID(label);

    ImDrawList* dl = ImGui::GetWindowDrawList();
    const float h = S(theme.compactRows ? 28.0f : 33.0f);
    const float avail = ImGui::GetContentRegionAvail().x;
    ImVec2 p = ImGui::GetCursorScreenPos();

    ImGui::SetCursorScreenPos(p);
    // InvisibleButton reports on release and cancels itself when the finger
    // moved (drag) - so scrolling the list never flips a switch by accident
    const bool tapped = ImGui::InvisibleButton("##row", ImVec2(avail, h));
    const bool hovered = ImGui::IsItemHovered();
    SetLastItem();
    if (tapped) *v = !*v;

    float& anim = AnimRef(ImGui::GetID("##anim"), *v ? 1.0f : 0.0f);
    anim += ((*v ? 1.0f : 0.0f) - anim) * AnimSpeed();

    ImU32 bg = hovered ? U32(pal.rowHover) : U32(pal.bgRow, 0.55f);
    dl->AddRectFilled(p, ImVec2(p.x + avail, p.y + h), bg, S(7.0f));
    if (anim > 0.02f)
        dl->AddRectFilled(p, ImVec2(p.x + avail, p.y + h), U32(pal.accentDeep, 0.30f * anim), S(7.0f));
    dl->AddRect(p, ImVec2(p.x + avail, p.y + h), U32(pal.borderSoft, hovered ? 0.9f : 0.5f), S(7.0f), 0, S(1.0f));

    // left accent bar when active
    if (anim > 0.02f)
        dl->AddRectFilled(ImVec2(p.x + S(1.0f), p.y + S(4.0f)), ImVec2(p.x + S(3.0f), p.y + h - S(4.0f)),
                          U32(pal.accent, anim), S(1.0f));

    const float swW = S(42.0f), swH = S(21.0f);
    ImVec2 sw0(p.x + avail - swW - S(9.0f), p.y + (h - swH) * 0.5f);
    ImVec2 sw1(sw0.x + swW, sw0.y + swH);

    ImU32 off = U32(ImVec4(0.106f, 0.090f, 0.157f, 1.0f));
    ImU32 on  = U32(pal.accent);
    dl->AddRectFilled(sw0, sw1, U32Lerp(off, on, anim), swH * 0.5f);
    if (anim > 0.02f)
        dl->AddRect(sw0, sw1, U32(pal.accentHi, 0.40f * anim), swH * 0.5f, 0, S(1.2f));

    float kx = ImLerp(sw0.x + swH * 0.5f, sw1.x - swH * 0.5f, anim);
    ImVec2 kc(kx, (sw0.y + sw1.y) * 0.5f);
    if (anim > 0.02f)
        dl->AddCircleFilled(kc, swH * 0.62f, U32(pal.accent, 0.35f * anim), 24);
    dl->AddCircleFilled(kc, swH * 0.36f, U32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), 24);

    // label
    float tx = p.x + S(12.0f);
    float ty = p.y + (h - ImGui::GetFontSize()) * 0.5f;
    if (icon && icon[0])
    {
        dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(tx, ty),
                    U32(anim > 0.5f ? pal.accentHi : pal.textDim), icon);
        tx += S(20.0f);
    }
    ImVec4 lblCol = anim > 0.5f ? pal.text : ImVec4(0.804f, 0.780f, 0.871f, 1.0f);
    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(tx, ty), U32(lblCol), label);

    if (hint && hint[0])
    {
        float hw = ImGui::CalcTextSize(hint).x;
        float hx = sw0.x - hw - S(10.0f);
        if (hx > tx + ImGui::CalcTextSize(label).x + S(8.0f))
            dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
                        ImVec2(hx, p.y + (h - S(15.0f)) * 0.5f), U32(pal.textDim), hint);
    }

    ImGui::Dummy(ImVec2(1.0f, S(3.0f)));
    ImGui::PopID();
    return *v;
}

inline bool SliderRow(const char* label, float* value, float mn, float mx,
                      const char* fmt = "%.2f", const char* hint = nullptr, ImVec4* accentOverride = nullptr)
{
    if (!value) return false;
    ImGui::PushID(label);

    ImDrawList* dl = ImGui::GetWindowDrawList();
    const float rowH  = S(24.0f);
    const float trackH = S(18.0f);
    const float avail = ImGui::GetContentRegionAvail().x;
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImVec4 accent = accentOverride ? *accentOverride : pal.accent;

    // value text + label
    char valBuf[64];
    snprintf(valBuf, sizeof(valBuf), fmt, *value);
    float ty = p.y + (rowH - ImGui::GetFontSize()) * 0.5f;
    Label(dl, ImVec2(p.x + S(2.0f), ty), pal.textSoft, label);
    float vw = ImGui::CalcTextSize(valBuf).x;
    ImVec2 pill0(p.x + avail - vw - S(16.0f), p.y + S(2.0f));
    ImVec2 pill1(p.x + avail, p.y + rowH - S(2.0f));
    dl->AddRectFilled(pill0, pill1, U32(accent, 0.22f), S(6.0f));
    dl->AddRect(pill0, pill1, U32(accent, 0.60f), S(6.0f), 0, S(1.0f));
    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(pill0.x + S(6.0f), pill0.y + S(2.0f)), U32(accent), valBuf);

    if (hint && hint[0])
    {
        float hw = ImGui::CalcTextSize(hint).x;
        float hx = pill0.x - hw - S(10.0f);
        if (hx > p.x + ImGui::CalcTextSize(label).x + S(10.0f))
            dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(hx, ty), U32(pal.textDim), hint);
    }

    ImGui::Dummy(ImVec2(avail, rowH));

    // track
    ImVec2 tpos = ImGui::GetCursorScreenPos();
    ImGui::InvisibleButton("##track", ImVec2(avail, trackH));
    SetLastItem();
    const bool active = ImGui::IsItemActive();
    const bool hovered = ImGui::IsItemHovered();
    bool changed = false;

    if (active)
    {
        float t = (ImGui::GetIO().MousePos.x - tpos.x) / ImMax(avail, 1.0f);
        t = ImClamp(t, 0.0f, 1.0f);
        float nv = mn + (mx - mn) * t;
        if (nv != *value) { *value = nv; changed = true; }
    }

    float fraction = (mx - mn) != 0.0f ? (*value - mn) / (mx - mn) : 0.0f;
    fraction = ImClamp(fraction, 0.0f, 1.0f);
    float knobX = tpos.x + avail * fraction;
    float cy    = tpos.y + trackH * 0.5f;
    float barY0 = cy - S(4.0f);
    float barY1 = cy + S(4.0f);

    dl->AddRectFilled(ImVec2(tpos.x, barY0), ImVec2(tpos.x + avail, barY1), U32(ImVec4(0.075f, 0.063f, 0.110f, 1.0f)), S(4.0f));
    GradientH(dl, ImVec2(tpos.x, barY0), ImVec2(knobX, barY1), U32(accent, 0.95f), U32(accent, 0.65f), S(4.0f));
    // ticks
    for (int i = 1; i < 10; ++i)
    {
        float tx = tpos.x + avail * ((float)i / 10.0f);
        dl->AddRectFilled(ImVec2(tx, barY0 + S(2.0f)), ImVec2(tx + S(1.0f), barY1 - S(2.0f)), IM_COL32(0, 0, 0, 60));
    }
    if (active || hovered)
        dl->AddCircleFilled(ImVec2(knobX, cy), S(11.0f), U32(accent, 0.28f), 24);
    dl->AddCircleFilled(ImVec2(knobX, cy), S(7.0f), U32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), 24);
    dl->AddCircle(ImVec2(knobX, cy), S(7.0f), U32(accent), 24, S(1.4f));

    ImGui::Dummy(ImVec2(1.0f, S(4.0f)));
    ImGui::PopID();
    return changed;
}

inline bool IntSliderRow(const char* label, int* value, int mn, int mx, const char* fmt = "%d", const char* hint = nullptr)
{
    if (!value) return false;
    float f = (float)*value;
    bool changed = SliderRow(label, &f, (float)mn, (float)mx, fmt, hint);
    int nv = (int)(f + (f >= 0.0f ? 0.5f : -0.5f));
    if (nv != *value) { *value = nv; changed = true; }
    return changed;
}

// tap to cycle - much friendlier than a combo box on a touch screen
inline bool SelectRow(const char* label, int* index, const char* const* items, int count,
                      const char* icon = nullptr)
{
    if (!index || !items || count <= 0) return false;
    ImGui::PushID(label);

    ImDrawList* dl = ImGui::GetWindowDrawList();
    const float h = S(theme.compactRows ? 28.0f : 33.0f);
    const float avail = ImGui::GetContentRegionAvail().x;
    ImVec2 p = ImGui::GetCursorScreenPos();

    const bool tapped = ImGui::InvisibleButton("##sel", ImVec2(avail, h));
    const bool hovered = ImGui::IsItemHovered();
    SetLastItem();
    bool changed = false;
    if (tapped)
    {
        *index = (*index + 1) % count;
        changed = true;
    }

    dl->AddRectFilled(p, ImVec2(p.x + avail, p.y + h), hovered ? U32(pal.rowHover) : U32(pal.bgRow, 0.55f), S(7.0f));
    dl->AddRect(p, ImVec2(p.x + avail, p.y + h), U32(pal.borderSoft), S(7.0f), 0, S(1.0f));

    float tx = p.x + S(12.0f);
    float ty = p.y + (h - ImGui::GetFontSize()) * 0.5f;
    if (icon && icon[0])
    {
        dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(tx, ty), U32(pal.textDim), icon);
        tx += S(20.0f);
    }
    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(tx, ty), U32(pal.textSoft), label);

    const char* cur = items[*index];
    float cw = ImGui::CalcTextSize(cur).x;
    ImVec2 c0(p.x + avail - cw - S(30.0f), p.y + S(5.0f));
    ImVec2 c1(p.x + avail - S(9.0f), p.y + h - S(5.0f));
    dl->AddRectFilled(c0, c1, U32(pal.accent, 0.20f), S(6.0f));
    dl->AddRect(c0, c1, U32(pal.accent, 0.55f), S(6.0f), 0, S(1.0f));
    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(c0.x + S(6.0f), c0.y + S(2.0f)), U32(pal.accentHi), cur);
    // cycle glyph
    Label(dl, ImVec2(p.x + avail - S(21.0f), ty), pal.textDim, ">");

    // option dots
    if (count > 1 && count <= 8)
    {
        float dx = p.x + S(12.0f);
        float dy = p.y + h - S(6.0f);
        for (int i = 0; i < count; ++i)
        {
            bool on = (i == *index);
            dl->AddRectFilled(ImVec2(dx + i * S(9.0f), dy), ImVec2(dx + i * S(9.0f) + S(6.0f), dy + S(2.0f)),
                              on ? U32(pal.accent) : U32(pal.textDim, 0.35f), S(1.0f));
        }
    }

    ImGui::Dummy(ImVec2(1.0f, S(3.0f)));
    ImGui::PopID();
    return changed;
}

inline bool Button(const char* label, const char* icon = nullptr, ImVec2 size = ImVec2(0, 0),
                   bool primary = true, const char* hint = nullptr)
{
    ImGui::PushID(label);
    ImDrawList* dl = ImGui::GetWindowDrawList();

    const float h = size.y > 0.0f ? size.y : S(34.0f);
    const float w = size.x > 0.0f ? size.x : ImGui::GetContentRegionAvail().x;
    ImVec2 p = ImGui::GetCursorScreenPos();

    bool pressed = ImGui::InvisibleButton("##btn", ImVec2(w, h));
    SetLastItem();
    const bool hovered = ImGui::IsItemHovered();
    const bool active  = ImGui::IsItemActive();

    float& anim = AnimRef(ImGui::GetID("##press"), 0.0f);
    anim += ((active ? 1.0f : 0.0f) - anim) * AnimSpeed();

    ImVec4 base = primary ? pal.accentDeep : pal.bgCardHi;
    ImVec4 top  = primary ? pal.accent : pal.rowHover;
    if (hovered) top = primary ? pal.accentHi : pal.accent;
    if (active)  top = ImVec4(top.x * 0.85f, top.y * 0.85f, top.z * 0.9f, top.w);

    if (hovered || active)
        GlowRect(dl, p, ImVec2(p.x + w, p.y + h), S(9.0f), base, 1.0f * theme.glowStrength, 2);

    ImVec2 a(p.x, p.y + anim * S(1.5f));
    ImVec2 b(p.x + w, p.y + h + anim * S(1.5f));
    GradientV(dl, a, b, U32(top), U32(base), S(9.0f));
    dl->AddRect(a, b, U32(pal.accentHi, hovered ? 0.9f : 0.5f), S(9.0f), 0, S(1.0f));

    char buf[160];
    if (icon && icon[0]) snprintf(buf, sizeof(buf), "%s  %s", icon, label);
    else                 snprintf(buf, sizeof(buf), "%s", label);

    ImVec2 ts = ImGui::CalcTextSize(buf);
    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
                ImVec2(a.x + (w - ts.x) * 0.5f, a.y + (h - ts.y) * 0.5f),
                U32(ImVec4(1.0f, 1.0f, 1.0f, primary ? 1.0f : 0.94f)), buf);

    if (hint && hint[0])
    {
        float hw = ImGui::CalcTextSize(hint).x;
        if (hw < w * 0.6f)
            dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
                        ImVec2(a.x + w - hw - S(10.0f), a.y + (h - ImGui::GetFontSize()) * 0.5f),
                        U32(pal.textDim), hint);
    }

    ImGui::PopID();
    return pressed;
}

inline bool IconButton(const char* id, const char* icon, ImVec2 size, ImVec4 col, bool danger = false)
{
    ImGui::PushID(id);
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    bool pressed = ImGui::InvisibleButton("##ib", size);
    SetLastItem();
    const bool hovered = ImGui::IsItemHovered();
    const bool active  = ImGui::IsItemActive();

    ImVec4 base = danger ? pal.danger : col;
    if (hovered)
        dl->AddRectFilled(p, ImVec2(p.x + size.x, p.y + size.y), U32(base, active ? 0.45f : 0.30f), S(7.0f));
    dl->AddRect(p, ImVec2(p.x + size.x, p.y + size.y), U32(base, hovered ? 0.95f : 0.45f), S(7.0f), 0, S(1.0f));
    ImVec2 ts = ImGui::CalcTextSize(icon);
    dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
                ImVec2(p.x + (size.x - ts.x) * 0.5f, p.y + (size.y - ts.y) * 0.5f), U32(base), icon);
    ImGui::PopID();
    return pressed;
}

// thin progress / meter bar
inline void MeterBar(const char* label, float fraction, ImVec4 col, const char* value = nullptr)
{
    ImDrawList* dl = ImGui::GetWindowDrawList();
    const float avail = ImGui::GetContentRegionAvail().x;
    const float h = S(18.0f);
    ImVec2 p = ImGui::GetCursorScreenPos();
    fraction = ImClamp(fraction, 0.0f, 1.0f);

    ImGui::Dummy(ImVec2(avail, h));
    Label(dl, ImVec2(p.x + S(2.0f), p.y + S(1.0f)), pal.textDim, label);
    ImVec2 b0(p.x, p.y + h - S(8.0f));
    ImVec2 b1(p.x + avail, p.y + h - S(4.0f));
    dl->AddRectFilled(b0, b1, U32(ImVec4(0.075f, 0.063f, 0.110f, 1.0f)), S(2.0f));
    GradientH(dl, b0, ImVec2(b0.x + avail * fraction, b1.y), U32(pal.accent, 0.95f), U32(col, 0.95f), S(2.0f));
    if (value && value[0])
    {
        float w = ImGui::CalcTextSize(value).x;
        Label(dl, ImVec2(p.x + avail - w, p.y + S(1.0f)), col, value);
    }
    ImGui::Dummy(ImVec2(1.0f, S(4.0f)));
}

// horizontal divider with label in the middle
inline void DividerText(const char* txt)
{
    ImDrawList* dl = ImGui::GetWindowDrawList();
    const float avail = ImGui::GetContentRegionAvail().x;
    const float h = S(18.0f);
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImGui::Dummy(ImVec2(avail, h));
    float tw = ImGui::CalcTextSize(txt).x;
    float cx = p.x + (avail - tw) * 0.5f;
    Separator(dl, ImVec2(p.x, p.y + h * 0.5f), ImVec2(cx - S(8.0f), p.y + h * 0.5f), pal.borderSoft);
    Separator(dl, ImVec2(cx + tw + S(8.0f), p.y + h * 0.5f), ImVec2(p.x + avail, p.y + h * 0.5f), pal.borderSoft);
    Label(dl, ImVec2(cx, p.y + S(1.0f)), pal.textDim, txt);
}

inline void Spacer(float y) { ImGui::Dummy(ImVec2(1.0f, S(y))); }

} // namespace ShadowUI
