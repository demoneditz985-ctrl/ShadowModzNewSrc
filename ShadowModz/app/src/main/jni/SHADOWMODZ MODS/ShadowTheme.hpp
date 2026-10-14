#pragma once
// ============================================================================
//  SHADOW MODZ - UI KIT  ("ONYX" design system)
// ----------------------------------------------------------------------------
//  Pure black glass panels with a neon glow, drawn entirely by hand:
//
//    * no ImGui items are used for the menu  -> no ImGui ActiveId / window
//      state, so a touch can never "go somewhere else" or strand a widget,
//    * one pointer layer owns every press: a widget only reacts to a press it
//      claimed on the same stroke, and only when the finger lifts on top of
//      it without having travelled (that is the tap/drag fix),
//    * one layout cursor stacks the widgets, and anything outside the scroll
//      clip is neither drawn nor hit-tested,
//    * every label is plain text, icons are drawn as vectors, so nothing
//      depends on a glyph being present in the game's merged font atlas.
// ============================================================================

#include "imgui.h"
#include "imgui_internal.h"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <map>

namespace ShadowUI
{

// ===========================================================================
//  P A L E T T E
// ===========================================================================
struct Palette
{
    ImVec4 bgRoot     = ImVec4(0.012f, 0.012f, 0.024f, 1.000f);  // #030306
    ImVec4 bgWindow   = ImVec4(0.035f, 0.031f, 0.055f, 0.985f);  // #09080E
    ImVec4 bgWindow2  = ImVec4(0.020f, 0.018f, 0.035f, 0.985f);
    ImVec4 bgCard     = ImVec4(0.055f, 0.051f, 0.086f, 0.900f);  // #0E0D16
    ImVec4 bgRow      = ImVec4(0.063f, 0.059f, 0.102f, 0.920f);
    ImVec4 bgRowHot   = ImVec4(0.106f, 0.098f, 0.161f, 0.950f);
    ImVec4 bgRowOn    = ImVec4(0.180f, 0.110f, 0.320f, 0.960f);
    ImVec4 bgBar      = ImVec4(0.090f, 0.086f, 0.141f, 0.950f);
    ImVec4 trackOff   = ImVec4(0.161f, 0.153f, 0.220f, 1.000f);

    ImVec4 text       = ImVec4(0.937f, 0.933f, 0.976f, 1.000f);
    ImVec4 textSoft   = ImVec4(0.706f, 0.694f, 0.800f, 1.000f);
    ImVec4 textDim    = ImVec4(0.427f, 0.416f, 0.510f, 1.000f);

    ImVec4 border     = ImVec4(0.259f, 0.239f, 0.396f, 0.850f);
    ImVec4 borderSoft = ImVec4(0.141f, 0.133f, 0.216f, 0.900f);

    ImVec4 ok         = ImVec4(0.239f, 0.902f, 0.549f, 1.000f);
    ImVec4 warn       = ImVec4(0.984f, 0.749f, 0.141f, 1.000f);
    ImVec4 danger     = ImVec4(0.976f, 0.286f, 0.404f, 1.000f);
    ImVec4 shadow     = ImVec4(0.000f, 0.000f, 0.000f, 1.000f);

    // accent is filled in by ApplyAccent()
    ImVec4 accent     = ImVec4(0.658f, 0.333f, 0.968f, 1.000f);
    ImVec4 accentDeep = ImVec4(0.360f, 0.160f, 0.780f, 1.000f);
    ImVec4 accentHi   = ImVec4(0.815f, 0.600f, 0.996f, 1.000f);
};
inline Palette pal;

struct AccentPreset { const char* name; ImVec4 main, deep, hi; };
inline const AccentPreset kAccents[] =
{
    { "NEBULA",  ImVec4(0.658f, 0.333f, 0.968f, 1.0f), ImVec4(0.360f, 0.160f, 0.780f, 1.0f), ImVec4(0.815f, 0.600f, 0.996f, 1.0f) },
    { "VIOLET",  ImVec4(0.545f, 0.361f, 0.965f, 1.0f), ImVec4(0.310f, 0.180f, 0.760f, 1.0f), ImVec4(0.737f, 0.612f, 0.996f, 1.0f) },
    { "ORCHID",  ImVec4(0.796f, 0.353f, 0.937f, 1.0f), ImVec4(0.545f, 0.145f, 0.729f, 1.0f), ImVec4(0.898f, 0.639f, 0.980f, 1.0f) },
    { "ICE",     ImVec4(0.231f, 0.749f, 0.996f, 1.0f), ImVec4(0.086f, 0.427f, 0.729f, 1.0f), ImVec4(0.588f, 0.886f, 1.000f, 1.0f) },
    { "ACID",    ImVec4(0.529f, 0.937f, 0.267f, 1.0f), ImVec4(0.243f, 0.573f, 0.086f, 1.0f), ImVec4(0.737f, 0.976f, 0.541f, 1.0f) },
    { "CRIMSON", ImVec4(0.980f, 0.290f, 0.404f, 1.0f), ImVec4(0.686f, 0.106f, 0.220f, 1.0f), ImVec4(1.000f, 0.584f, 0.647f, 1.0f) },
};
inline const int kAccentCount = IM_ARRAYSIZE(kAccents);

struct Theme
{
    float uiScale       = 1.00f;   // effective scale (auto * userScale)
    float userScale     = 1.00f;   // SYSTEM tab -> UI Scale
    float fontSize      = 26.0f;   // base text size the atlas is rasterised from
                                   // (FSize() multiplies it by uiScale, and SetupImgui()
                                   //  builds the atlas at the same 26 * auto scale, so text
                                   //  is drawn at exactly one screen pixel per font pixel)
    float rowHeight     = 48.0f;
    float rowGap        = 8.0f;
    float pad           = 14.0f;
    float radius        = 14.0f;
    float glowStrength  = 1.00f;
    float windowOpacity = 1.00f;
    float menuWidth     = 470.0f;  // recomputed from the screen in Draw()
    float menuHeight    = 560.0f;
    float launcherRadius = 30.0f;
    bool  showLauncher   = true;
    bool  showStatusBar  = false;
    int   accentIndex    = 0;
    int   glowLayers     = 4;
};
inline Theme theme;

// ===========================================================================
//  S M A L L   H E L P E R S
// ===========================================================================
inline ImVec4 Alpha(const ImVec4& c, float a) { ImVec4 r = c; r.w = a; return r; }
inline ImU32  U32(const ImVec4& c)          { return ImGui::ColorConvertFloat4ToU32(c); }
inline ImU32  U32(const ImVec4& c, float a) { ImVec4 r = c; r.w = a; return ImGui::ColorConvertFloat4ToU32(r); }
inline ImVec4 Mix(const ImVec4& a, const ImVec4& b, float t)
{
    return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t,
                  a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
}
inline float S(float v) { return v * theme.uiScale; }
inline float Clamp01(float v) { return v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v); }

// per widget animation state (0..1), keyed by the widget hash
inline std::map<ImU32, float>& Anims() { static std::map<ImU32, float> m; return m; }
inline float Anim(ImU32 id, float target)
{
    float& v = Anims()[id];
    float step = Clamp01(ImGui::GetIO().DeltaTime * 14.0f);
    v += (target - v) * step;
    if (v < 0.001f) v = 0.0f;
    return v;
}

inline ImFont* Font() { return ImGui::GetFont(); }
inline float  FSize(float scale = 1.0f) { return theme.fontSize * theme.uiScale * scale; }
// the inverse of FSize(): the scale factor that makes text exactly px tall
inline float  ScaleForPx(float px) { const float base = theme.fontSize * theme.uiScale; return (base > 1.0f) ? px / base : 1.0f; }
inline ImVec2 TextSize(const char* s, float scale = 1.0f)
{
    if (!s || !s[0]) return ImVec2(0, 0);
    ImFont* f = Font();
    if (!f) return ImVec2(0, 0);
    return f->CalcTextSizeA(FSize(scale), FLT_MAX, 0.0f, s);
}

// ===========================================================================
//  D R A W   P R I M I T I V E S
// ===========================================================================
inline void TextAt(ImDrawList* dl, const char* s, ImVec2 p, ImU32 col, float scale = 1.0f)
{
    ImFont* f = Font();
    if (!f || !s || !s[0]) return;
    dl->AddText(f, FSize(scale), p, col, s);
}

// soft neon halo behind text
inline void TextGlow(ImDrawList* dl, const char* s, ImVec2 p, ImU32 col, float scale = 1.0f, float glow = 1.0f)
{
    ImFont* f = Font();
    if (!f || !s || !s[0]) return;
    float sz = FSize(scale);
    if (glow > 0.01f && theme.glowLayers > 0)
    {
        ImU32 halo = (col & 0x00FFFFFFu) | ((ImU32)(40.0f * glow * theme.glowStrength) << 24);
        for (int dx = -1; dx <= 1; ++dx)
            for (int dy = -1; dy <= 1; ++dy)
                if (dx || dy)
                    dl->AddText(f, sz, ImVec2(p.x + dx * S(1.0f), p.y + dy * S(1.0f)), halo, s);
    }
    dl->AddText(f, sz, p, col, s);
}

inline void TextCenter(ImDrawList* dl, const ImRect& r, const char* s, ImU32 col, float scale = 1.0f)
{
    ImVec2 t = TextSize(s, scale);
    TextAt(dl, s, ImVec2(r.GetCenter().x - t.x * 0.5f, r.GetCenter().y - t.y * 0.5f), col, scale);
}

inline void TextVCenter(ImDrawList* dl, const ImRect& r, float x, const char* s, ImU32 col, float scale = 1.0f)
{
    ImVec2 t = TextSize(s, scale);
    TextAt(dl, s, ImVec2(x, r.GetCenter().y - t.y * 0.5f), col, scale);
}

// letterspaced text (used for the wordmark + section captions)
inline void TextSpaced(ImDrawList* dl, const char* s, ImVec2 p, ImU32 col, float scale, float spacing)
{
    ImFont* f = Font();
    if (!f || !s) return;
    float x = p.x;
    for (const char* c = s; *c; ++c)
    {
        char one[2] = { *c, 0 };
        dl->AddText(f, FSize(scale), ImVec2(x, p.y), col, one);
        x += TextSize(one, scale).x + spacing;
    }
}

inline float TextSpacedWidth(const char* s, float scale, float spacing)
{
    if (!s) return 0.0f;
    float x = 0.0f;
    for (const char* c = s; *c; ++c)
    {
        char one[2] = { *c, 0 };
        x += TextSize(one, scale).x + spacing;
    }
    return x > 0.0f ? x - spacing : 0.0f;
}

// outer glow made of expanding rounded outlines
inline void GlowRound(ImDrawList* dl, const ImRect& r, float rounding, ImVec4 col, float strength, int layers = 0)
{
    if (strength <= 0.01f || theme.glowStrength <= 0.01f) return;
    if (layers <= 0) layers = theme.glowLayers;
    for (int i = layers; i >= 1; --i)
    {
        float e = S(2.4f) * (float)i * strength * 0.7f;
        dl->AddRect(ImVec2(r.Min.x - e, r.Min.y - e), ImVec2(r.Max.x + e, r.Max.y + e),
                    U32(col, 0.050f * strength), rounding + e, 0, S(1.0f));
    }
}

inline void GlowCircle(ImDrawList* dl, ImVec2 c, float radius, ImVec4 col, float strength, int layers = 4)
{
    if (strength <= 0.01f) return;
    for (int i = layers; i >= 1; --i)
    {
        float rr = radius + S(2.6f) * (float)i * strength * 0.7f;
        dl->AddCircle(c, rr, U32(col, 0.055f * strength), 0, S(1.4f));
    }
}

// black glass panel with a neon edge
inline void Panel(ImDrawList* dl, const ImRect& r, float rounding, ImVec4 edge, float glow,
                  ImVec4 fill, ImVec4 fillTop, float opacity = 1.0f)
{
    if (glow > 0.01f) GlowRound(dl, r, rounding, edge, glow);
    dl->AddRectFilled(r.Min, r.Max, U32(fill, fill.w * opacity), rounding);

    // faint accent sheen across the top half
    ImRect top(r.Min, ImVec2(r.Max.x, r.Min.y + (r.Max.y - r.Min.y) * 0.42f));
    dl->PushClipRect(top.Min, top.Max, true);
    dl->AddRectFilled(r.Min, r.Max, U32(fillTop, fillTop.w * opacity), rounding);
    dl->PopClipRect();

    dl->AddRect(r.Min, r.Max, U32(edge, 0.55f), rounding, 0, S(1.3f));
}

inline void Separator(ImDrawList* dl, const ImRect& r, ImVec4 col, float fade = 1.0f)
{
    dl->AddRectFilledMultiColor(r.Min, r.Max,
                                U32(col, 0.0f), U32(col, 0.55f * fade),
                                U32(col, 0.55f * fade), U32(col, 0.0f));
}

inline void DotGrid(ImDrawList* dl, const ImRect& r, ImVec4 col, float step)
{
    float s = S(step);
    for (float y = r.Min.y + s; y < r.Max.y; y += s)
        for (float x = r.Min.x + s; x < r.Max.x; x += s)
            dl->AddCircleFilled(ImVec2(x, y), S(1.0f), U32(col, 0.16f));
}

// ===========================================================================
//  V E C T O R   G L Y P H S
//  ---------------------------------------------------------------------------
//  Drawn instead of font icons: the game's merged atlas does not contain every
//  FontAwesome glyph, so a missing icon used to render as an empty box.
// ===========================================================================
enum Glyph
{
    GNone = 0, GAim, GCrosshair, GBox, GPerson, GHeart, GEye, GBolt, GShield, GStar,
    GGear, GKey, GLock, GUser, GSliders, GPalette, GBell, GCar, GChute, GMagnet,
    GBomb, GSkull, GGauge, GWrench, GInfo, GFlag, GFlame, GCheck, GDown, GUp,
    GRight, GPower, GPlus, GMinus, GSend, GX, GRocket, GBone, GGlobe, GTrash
};

inline void GlyphDraw(ImDrawList* dl, Glyph g, ImVec2 c, float s, ImU32 col, float th = 1.0f)
{
    const float t = ImMax(S(1.6f) * th, 1.0f);
    const float r = s * 0.5f;
    switch (g)
    {
    case GAim:                                       // crosshair
        dl->AddCircle(c, r * 0.78f, col, 0, t);
        dl->AddLine(ImVec2(c.x - r * 1.1f, c.y), ImVec2(c.x - r * 0.30f, c.y), col, t);
        dl->AddLine(ImVec2(c.x + r * 0.30f, c.y), ImVec2(c.x + r * 1.1f, c.y), col, t);
        dl->AddLine(ImVec2(c.x, c.y - r * 1.1f), ImVec2(c.x, c.y - r * 0.30f), col, t);
        dl->AddLine(ImVec2(c.x, c.y + r * 0.30f), ImVec2(c.x, c.y + r * 1.1f), col, t);
        dl->AddCircleFilled(c, r * 0.16f, col);
        break;
    case GCrosshair:
        dl->AddCircle(c, r * 0.72f, col, 0, t);
        dl->AddLine(ImVec2(c.x, c.y - r), ImVec2(c.x, c.y + r), col, t);
        dl->AddLine(ImVec2(c.x - r, c.y), ImVec2(c.x + r, c.y), col, t);
        break;
    case GBox:
        dl->AddRect(ImVec2(c.x - r * 0.85f, c.y - r * 0.65f), ImVec2(c.x + r * 0.85f, c.y + r * 0.65f), col, S(2.0f), 0, t);
        dl->AddLine(ImVec2(c.x - r * 0.85f, c.y), ImVec2(c.x - r * 0.25f, c.y), col, t);
        break;
    case GPerson:
        dl->AddCircle(ImVec2(c.x, c.y - r * 0.45f), r * 0.36f, col, 0, t);
        dl->AddLine(ImVec2(c.x, c.y - r * 0.05f), ImVec2(c.x, c.y + r * 0.55f), col, t);
        dl->AddLine(ImVec2(c.x - r * 0.55f, c.y + r * 0.15f), ImVec2(c.x + r * 0.55f, c.y + r * 0.15f), col, t);
        dl->AddLine(ImVec2(c.x, c.y + r * 0.55f), ImVec2(c.x - r * 0.45f, c.y + r * 1.0f), col, t);
        dl->AddLine(ImVec2(c.x, c.y + r * 0.55f), ImVec2(c.x + r * 0.45f, c.y + r * 1.0f), col, t);
        break;
    case GHeart:
        dl->AddCircleFilled(ImVec2(c.x - r * 0.40f, c.y - r * 0.30f), r * 0.42f, col);
        dl->AddCircleFilled(ImVec2(c.x + r * 0.40f, c.y - r * 0.30f), r * 0.42f, col);
        dl->AddTriangleFilled(ImVec2(c.x - r * 0.80f, c.y - r * 0.10f),
                              ImVec2(c.x + r * 0.80f, c.y - r * 0.10f),
                              ImVec2(c.x, c.y + r * 0.95f), col);
        break;
    case GEye:
        dl->AddBezierCubic(ImVec2(c.x - r, c.y), ImVec2(c.x - r * 0.35f, c.y - r * 0.95f),
                           ImVec2(c.x + r * 0.35f, c.y - r * 0.95f), ImVec2(c.x + r, c.y), col, t);
        dl->AddBezierCubic(ImVec2(c.x + r, c.y), ImVec2(c.x + r * 0.35f, c.y + r * 0.95f),
                           ImVec2(c.x - r * 0.35f, c.y + r * 0.95f), ImVec2(c.x - r, c.y), col, t);
        dl->AddCircleFilled(c, r * 0.28f, col);
        break;
    case GBolt:
        dl->AddTriangleFilled(ImVec2(c.x + r * 0.35f, c.y - r), ImVec2(c.x - r * 0.50f, c.y + r * 0.12f),
                              ImVec2(c.x, c.y + r * 0.05f), col);
        dl->AddTriangleFilled(ImVec2(c.x - r * 0.35f, c.y + r), ImVec2(c.x + r * 0.50f, c.y - r * 0.12f),
                              ImVec2(c.x, c.y - r * 0.05f), col);
        break;
    case GShield:
        dl->AddBezierCubic(ImVec2(c.x - r * 0.85f, c.y - r * 0.60f), ImVec2(c.x - r * 0.30f, c.y - r * 0.95f),
                           ImVec2(c.x + r * 0.30f, c.y - r * 0.95f), ImVec2(c.x + r * 0.85f, c.y - r * 0.60f), col, t);
        dl->AddLine(ImVec2(c.x - r * 0.85f, c.y - r * 0.60f), ImVec2(c.x - r * 0.85f, c.y + r * 0.05f), col, t);
        dl->AddLine(ImVec2(c.x + r * 0.85f, c.y - r * 0.60f), ImVec2(c.x + r * 0.85f, c.y + r * 0.05f), col, t);
        dl->AddTriangleFilled(ImVec2(c.x - r * 0.85f, c.y), ImVec2(c.x + r * 0.85f, c.y), ImVec2(c.x, c.y + r * 1.0f), col);
        break;
    case GStar:
        for (int i = 0; i < 5; ++i)
        {
            float a = -1.5708f + i * 1.2566f;
            ImVec2 p0(c.x + cosf(a) * r, c.y + sinf(a) * r);
            float a2 = a + 2.5133f;
            ImVec2 p1(c.x + cosf(a2) * r, c.y + sinf(a2) * r);
            float a3 = a + 1.2566f;
            ImVec2 p2(c.x + cosf(a3) * r * 0.45f, c.y + sinf(a3) * r * 0.45f);
            dl->AddTriangleFilled(p0, p1, p2, col);
        }
        break;
    case GGear:
        dl->AddCircle(c, r * 0.55f, col, 0, t);
        dl->AddCircle(c, r * 0.20f, col, 0, t);
        for (int i = 0; i < 8; ++i)
        {
            float a = i * 0.7854f;
            dl->AddLine(ImVec2(c.x + cosf(a) * r * 0.55f, c.y + sinf(a) * r * 0.55f),
                        ImVec2(c.x + cosf(a) * r, c.y + sinf(a) * r), col, t * 1.6f);
        }
        break;
    case GKey:
        dl->AddCircle(ImVec2(c.x - r * 0.45f, c.y - r * 0.35f), r * 0.45f, col, 0, t);
        dl->AddLine(ImVec2(c.x - r * 0.15f, c.y - r * 0.05f), ImVec2(c.x + r * 0.90f, c.y + r * 0.70f), col, t);
        dl->AddLine(ImVec2(c.x + r * 0.45f, c.y + r * 0.25f), ImVec2(c.x + r * 0.70f, c.y + r * 0.45f), col, t);
        break;
    case GLock:
        dl->AddRect(ImVec2(c.x - r * 0.70f, c.y - r * 0.15f), ImVec2(c.x + r * 0.70f, c.y + r * 0.85f), col, S(2.0f), 0, t);
        dl->AddBezierCubic(ImVec2(c.x - r * 0.42f, c.y - r * 0.15f), ImVec2(c.x - r * 0.42f, c.y - r * 1.0f),
                           ImVec2(c.x + r * 0.42f, c.y - r * 1.0f), ImVec2(c.x + r * 0.42f, c.y - r * 0.15f), col, t);
        dl->AddCircleFilled(ImVec2(c.x, c.y + r * 0.30f), r * 0.16f, col);
        break;
    case GUser:
        dl->AddCircle(ImVec2(c.x, c.y - r * 0.45f), r * 0.38f, col, 0, t);
        dl->AddBezierCubic(ImVec2(c.x - r * 0.85f, c.y + r * 0.90f), ImVec2(c.x - r * 0.55f, c.y + r * 0.10f),
                           ImVec2(c.x + r * 0.55f, c.y + r * 0.10f), ImVec2(c.x + r * 0.85f, c.y + r * 0.90f), col, t);
        break;
    case GSliders:
        dl->AddLine(ImVec2(c.x - r * 0.90f, c.y - r * 0.50f), ImVec2(c.x + r * 0.90f, c.y - r * 0.50f), col, t);
        dl->AddLine(ImVec2(c.x - r * 0.90f, c.y + 0.0f),     ImVec2(c.x + r * 0.90f, c.y + 0.0f), col, t);
        dl->AddLine(ImVec2(c.x - r * 0.90f, c.y + r * 0.50f), ImVec2(c.x + r * 0.90f, c.y + r * 0.50f), col, t);
        dl->AddCircleFilled(ImVec2(c.x - r * 0.35f, c.y - r * 0.50f), r * 0.20f, col);
        dl->AddCircleFilled(ImVec2(c.x + r * 0.42f, c.y),             r * 0.20f, col);
        dl->AddCircleFilled(ImVec2(c.x - r * 0.10f, c.y + r * 0.50f), r * 0.20f, col);
        break;
    case GPalette:
        dl->AddCircle(c, r * 0.85f, col, 0, t);
        dl->AddCircleFilled(ImVec2(c.x - r * 0.35f, c.y - r * 0.30f), r * 0.16f, col);
        dl->AddCircleFilled(ImVec2(c.x + r * 0.30f, c.y - r * 0.38f), r * 0.16f, col);
        dl->AddCircleFilled(ImVec2(c.x + r * 0.40f, c.y + r * 0.20f), r * 0.16f, col);
        break;
    case GBell:
        dl->AddBezierCubic(ImVec2(c.x - r * 0.72f, c.y + r * 0.45f), ImVec2(c.x - r * 0.72f, c.y - r * 0.95f),
                           ImVec2(c.x + r * 0.72f, c.y - r * 0.95f), ImVec2(c.x + r * 0.72f, c.y + r * 0.45f), col, t);
        dl->AddLine(ImVec2(c.x - r * 0.85f, c.y + r * 0.45f), ImVec2(c.x + r * 0.85f, c.y + r * 0.45f), col, t);
        dl->AddCircleFilled(ImVec2(c.x, c.y + r * 0.72f), r * 0.16f, col);
        break;
    case GCar:
        dl->AddRect(ImVec2(c.x - r * 0.95f, c.y - r * 0.25f), ImVec2(c.x + r * 0.95f, c.y + r * 0.35f), col, S(2.0f), 0, t);
        dl->AddTriangleFilled(ImVec2(c.x - r * 0.55f, c.y - r * 0.25f), ImVec2(c.x + r * 0.55f, c.y - r * 0.25f),
                              ImVec2(c.x + r * 0.30f, c.y - r * 0.85f), col);
        dl->AddCircleFilled(ImVec2(c.x - r * 0.55f, c.y + r * 0.45f), r * 0.26f, col);
        dl->AddCircleFilled(ImVec2(c.x + r * 0.55f, c.y + r * 0.45f), r * 0.26f, col);
        break;
    case GChute:
        dl->AddBezierCubic(ImVec2(c.x - r, c.y - r * 0.15f), ImVec2(c.x - r * 0.55f, c.y - r * 1.05f),
                           ImVec2(c.x + r * 0.55f, c.y - r * 1.05f), ImVec2(c.x + r, c.y - r * 0.15f), col, t);
        dl->AddLine(ImVec2(c.x - r, c.y - r * 0.15f), ImVec2(c.x, c.y + r * 0.95f), col, t);
        dl->AddLine(ImVec2(c.x + r, c.y - r * 0.15f), ImVec2(c.x, c.y + r * 0.95f), col, t);
        dl->AddLine(ImVec2(c.x, c.y - r * 0.55f), ImVec2(c.x, c.y + r * 0.95f), col, t);
        break;
    case GMagnet:
        dl->AddCircle(c, r * 0.80f, col, 0, t * 2.2f);
        dl->AddRectFilled(ImVec2(c.x - r * 0.95f, c.y - r * 0.05f), ImVec2(c.x - r * 0.45f, c.y + r * 0.35f), col);
        dl->AddRectFilled(ImVec2(c.x + r * 0.45f, c.y - r * 0.05f), ImVec2(c.x + r * 0.95f, c.y + r * 0.35f), col);
        break;
    case GBomb:
        dl->AddCircleFilled(ImVec2(c.x - r * 0.15f, c.y + r * 0.25f), r * 0.72f, col);
        dl->AddLine(ImVec2(c.x + r * 0.35f, c.y - r * 0.30f), ImVec2(c.x + r * 0.75f, c.y - r * 0.80f), col, t * 1.6f);
        dl->AddCircleFilled(ImVec2(c.x + r * 0.85f, c.y - r * 0.90f), r * 0.18f, col);
        break;
    case GSkull:
        dl->AddCircleFilled(ImVec2(c.x, c.y - r * 0.20f), r * 0.72f, col);
        dl->AddRectFilled(ImVec2(c.x - r * 0.34f, c.y + r * 0.40f), ImVec2(c.x + r * 0.34f, c.y + r * 0.80f), col);
        break;
    case GGauge:
        dl->AddBezierCubic(ImVec2(c.x - r * 0.90f, c.y + r * 0.55f), ImVec2(c.x - r * 0.90f, c.y - r * 1.0f),
                           ImVec2(c.x + r * 0.90f, c.y - r * 1.0f), ImVec2(c.x + r * 0.90f, c.y + r * 0.55f), col, t);
        dl->AddLine(ImVec2(c.x, c.y + r * 0.55f), ImVec2(c.x + r * 0.55f, c.y - r * 0.35f), col, t * 1.6f);
        dl->AddCircleFilled(ImVec2(c.x, c.y + r * 0.55f), r * 0.16f, col);
        break;
    case GWrench:
        dl->AddCircleFilled(ImVec2(c.x - r * 0.55f, c.y - r * 0.50f), r * 0.40f, col);
        dl->AddLine(ImVec2(c.x - r * 0.30f, c.y - r * 0.28f), ImVec2(c.x + r * 0.70f, c.y + r * 0.72f), col, t * 2.0f);
        dl->AddCircleFilled(ImVec2(c.x + r * 0.78f, c.y + r * 0.80f), r * 0.20f, col);
        break;
    case GInfo:
        dl->AddCircle(c, r * 0.90f, col, 0, t);
        dl->AddLine(ImVec2(c.x, c.y - r * 0.10f), ImVec2(c.x, c.y + r * 0.55f), col, t);
        dl->AddCircleFilled(ImVec2(c.x, c.y - r * 0.52f), r * 0.13f, col);
        break;
    case GFlag:
        dl->AddLine(ImVec2(c.x - r * 0.60f, c.y - r * 0.90f), ImVec2(c.x - r * 0.60f, c.y + r * 0.95f), col, t * 1.6f);
        dl->AddTriangleFilled(ImVec2(c.x - r * 0.60f, c.y - r * 0.85f), ImVec2(c.x + r * 0.80f, c.y - r * 0.45f),
                              ImVec2(c.x - r * 0.60f, c.y + 0.0f), col);
        break;
    case GFlame:
        dl->AddBezierCubic(ImVec2(c.x, c.y - r), ImVec2(c.x + r * 0.95f, c.y),
                           ImVec2(c.x + r * 0.40f, c.y + r), ImVec2(c.x, c.y + r * 0.95f), col, t * 1.4f);
        dl->AddBezierCubic(ImVec2(c.x, c.y - r), ImVec2(c.x - r * 0.95f, c.y),
                           ImVec2(c.x - r * 0.40f, c.y + r), ImVec2(c.x, c.y + r * 0.95f), col, t * 1.4f);
        break;
    case GCheck:
        dl->AddLine(ImVec2(c.x - r * 0.75f, c.y + r * 0.05f), ImVec2(c.x - r * 0.15f, c.y + r * 0.65f), col, t * 1.8f);
        dl->AddLine(ImVec2(c.x - r * 0.15f, c.y + r * 0.65f), ImVec2(c.x + r * 0.80f, c.y - r * 0.60f), col, t * 1.8f);
        break;
    case GDown:
        dl->AddLine(ImVec2(c.x - r * 0.70f, c.y - r * 0.30f), ImVec2(c.x, c.y + r * 0.40f), col, t * 1.6f);
        dl->AddLine(ImVec2(c.x + r * 0.70f, c.y - r * 0.30f), ImVec2(c.x, c.y + r * 0.40f), col, t * 1.6f);
        break;
    case GUp:
        dl->AddLine(ImVec2(c.x - r * 0.70f, c.y + r * 0.30f), ImVec2(c.x, c.y - r * 0.40f), col, t * 1.6f);
        dl->AddLine(ImVec2(c.x + r * 0.70f, c.y + r * 0.30f), ImVec2(c.x, c.y - r * 0.40f), col, t * 1.6f);
        break;
    case GRight:
        dl->AddLine(ImVec2(c.x - r * 0.30f, c.y - r * 0.70f), ImVec2(c.x + r * 0.40f, c.y), col, t * 1.6f);
        dl->AddLine(ImVec2(c.x - r * 0.30f, c.y + r * 0.70f), ImVec2(c.x + r * 0.40f, c.y), col, t * 1.6f);
        break;
    case GPower:
        dl->AddBezierCubic(ImVec2(c.x - r * 0.90f, c.y - r * 0.30f), ImVec2(c.x - r * 0.95f, c.y + r * 1.05f),
                           ImVec2(c.x + r * 0.95f, c.y + r * 1.05f), ImVec2(c.x + r * 0.90f, c.y - r * 0.30f), col, t);
        dl->AddLine(ImVec2(c.x, c.y - r), ImVec2(c.x, c.y + r * 0.15f), col, t * 1.6f);
        break;
    case GPlus:
        dl->AddLine(ImVec2(c.x - r * 0.75f, c.y), ImVec2(c.x + r * 0.75f, c.y), col, t * 1.8f);
        dl->AddLine(ImVec2(c.x, c.y - r * 0.75f), ImVec2(c.x, c.y + r * 0.75f), col, t * 1.8f);
        break;
    case GMinus:
        dl->AddLine(ImVec2(c.x - r * 0.75f, c.y), ImVec2(c.x + r * 0.75f, c.y), col, t * 1.8f);
        break;
    case GSend:
        dl->AddTriangleFilled(ImVec2(c.x + r * 0.95f, c.y - r * 0.55f), ImVec2(c.x - r * 0.95f, c.y + r * 0.10f),
                              ImVec2(c.x - r * 0.10f, c.y + r * 0.20f), col);
        dl->AddTriangleFilled(ImVec2(c.x + r * 0.95f, c.y - r * 0.55f), ImVec2(c.x - r * 0.10f, c.y + r * 0.20f),
                              ImVec2(c.x - r * 0.30f, c.y + r * 0.90f), col);
        break;
    case GX:
        dl->AddLine(ImVec2(c.x - r * 0.70f, c.y - r * 0.70f), ImVec2(c.x + r * 0.70f, c.y + r * 0.70f), col, t * 1.8f);
        dl->AddLine(ImVec2(c.x + r * 0.70f, c.y - r * 0.70f), ImVec2(c.x - r * 0.70f, c.y + r * 0.70f), col, t * 1.8f);
        break;
    case GRocket:
        dl->AddBezierCubic(ImVec2(c.x - r * 0.45f, c.y + r * 0.45f), ImVec2(c.x - r * 0.35f, c.y - r * 0.90f),
                           ImVec2(c.x + r * 0.35f, c.y - r * 0.90f), ImVec2(c.x + r * 0.45f, c.y + r * 0.45f), col, t);
        dl->AddTriangleFilled(ImVec2(c.x - r * 0.45f, c.y + r * 0.10f), ImVec2(c.x - r * 0.95f, c.y + r * 0.85f),
                              ImVec2(c.x - r * 0.30f, c.y + r * 0.60f), col);
        dl->AddTriangleFilled(ImVec2(c.x + r * 0.45f, c.y + r * 0.10f), ImVec2(c.x + r * 0.95f, c.y + r * 0.85f),
                              ImVec2(c.x + r * 0.30f, c.y + r * 0.60f), col);
        break;
    case GBone:
        dl->AddCircleFilled(ImVec2(c.x - r * 0.55f, c.y - r * 0.55f), r * 0.26f, col);
        dl->AddCircleFilled(ImVec2(c.x + r * 0.55f, c.y + r * 0.55f), r * 0.26f, col);
        dl->AddLine(ImVec2(c.x - r * 0.45f, c.y - r * 0.45f), ImVec2(c.x + r * 0.45f, c.y + r * 0.45f), col, t * 1.6f);
        break;
    case GGlobe:
        dl->AddCircle(c, r * 0.90f, col, 0, t);
        dl->AddLine(ImVec2(c.x - r * 0.90f, c.y), ImVec2(c.x + r * 0.90f, c.y), col, t * 0.8f);
        dl->AddBezierCubic(ImVec2(c.x - r * 0.40f, c.y - r * 0.80f), ImVec2(c.x - r * 0.90f, c.y),
                           ImVec2(c.x - r * 0.90f, c.y), ImVec2(c.x - r * 0.40f, c.y + r * 0.80f), col, t * 0.8f);
        dl->AddBezierCubic(ImVec2(c.x + r * 0.40f, c.y - r * 0.80f), ImVec2(c.x + r * 0.90f, c.y),
                           ImVec2(c.x + r * 0.90f, c.y), ImVec2(c.x + r * 0.40f, c.y + r * 0.80f), col, t * 0.8f);
        break;
    case GTrash:
        dl->AddLine(ImVec2(c.x - r * 0.75f, c.y - r * 0.55f), ImVec2(c.x + r * 0.75f, c.y - r * 0.55f), col, t * 1.6f);
        dl->AddRect(ImVec2(c.x - r * 0.55f, c.y - r * 0.55f), ImVec2(c.x + r * 0.55f, c.y + r * 0.85f), col, S(2.0f), 0, t);
        dl->AddLine(ImVec2(c.x - r * 0.25f, c.y - r * 0.55f), ImVec2(c.x - r * 0.25f, c.y - r * 0.95f), col, t);
        dl->AddLine(ImVec2(c.x + r * 0.25f, c.y - r * 0.55f), ImVec2(c.x + r * 0.25f, c.y - r * 0.95f), col, t);
        break;
    default: break;
    }
}

// ===========================================================================
//  P O I N T E R   L A Y E R
//  ---------------------------------------------------------------------------
//  One press = one owner.  A widget can only react on the frame the finger is
//  lifted, on top of the widget that claimed the press, and only if the finger
//  did not travel further than kSlop.  Everything else is a drag.
// ===========================================================================
namespace P
{

inline float Slop() { return S(9.0f); }   // px of travel that turns a tap into a drag

struct State
{
    ImVec2 pos       = ImVec2(0, 0);
    ImVec2 downPos   = ImVec2(0, 0);
    ImVec2 delta     = ImVec2(0, 0);
    bool   down      = false;
    bool   wentDown  = false;
    bool   wentUp    = false;
    bool   moved     = false;     // travelled further than kSlop
    float  downTime  = 0.0f;
    ImU32  owner     = 0;         // widget that owns the current press
    ImU32  hot       = 0;         // widget under the finger this frame
    bool   stroke    = false;     // a finger is on the glass (or just left)

    // geometry frozen on the frame the press landed: the panel can be dragged
    // while the finger is down, so nothing may be compared against live rects
    ImRect ownerRect = ImRect(ImVec2(0, 0), ImVec2(0, 0));   // rect of the owner
    ImRect pressArea = ImRect(ImVec2(0, 0), ImVec2(0, 0));   // scrollable area
};
inline State st;

inline ImVec2 lastPos = ImVec2(-10000.0f, -10000.0f);
inline bool   prevDown = false;

// per frame bookkeeping: which widget rects exist this frame (used by the
// desktop harness to tap real widgets without repeating the layout maths)
inline ImU32 Id(const char* s) { return (s && s[0]) ? ImHashStr(s, 0, 0) : 0u; }
inline std::map<ImU32, ImRect>& Rects() { static std::map<ImU32, ImRect> m; return m; }
inline void  Note(ImU32 id, const ImRect& r) { if (id) Rects()[id] = r; }
inline bool  FindRect(const char* id, ImRect& out)
{
    std::map<ImU32, ImRect>& m = Rects();
    std::map<ImU32, ImRect>::iterator it = m.find(Id(id));
    if (it == m.end()) return false;
    out = it->second;
    return true;
}

inline void BeginFrame()
{
    ImGuiIO& io = ImGui::GetIO();
    st.pos  = io.MousePos;
    st.down = io.MouseDown[0];
    st.delta = (lastPos.x < -1000.0f) ? ImVec2(0, 0) : ImVec2(st.pos.x - lastPos.x, st.pos.y - lastPos.y);
    lastPos = st.pos;

    st.wentDown = st.down && !prevDown;
    st.wentUp   = !st.down && prevDown;

    if (st.wentDown)
    {
        st.downPos  = st.pos;
        st.moved    = false;
        st.owner    = 0;
        st.ownerRect = ImRect(ImVec2(0, 0), ImVec2(0, 0));
        st.pressArea = ImRect(ImVec2(0, 0), ImVec2(0, 0));
        st.downTime = ImGui::GetTime();
    }
    if (st.down && !st.moved)
    {
        float dx = st.pos.x - st.downPos.x, dy = st.pos.y - st.downPos.y;
        const float slop = Slop();
        if ((dx * dx + dy * dy) > (slop * slop)) st.moved = true;
    }
    st.hot = 0;
    st.stroke = st.down;
    Rects().clear();
    prevDown = st.down;
}

// the scrollable area offers itself; captured on the frame the press lands
inline void SetPressArea(const ImRect& area) { if (st.wentDown) st.pressArea = area; }

// was the widget that owns this press inside the area the finger landed in?
// (both rects come from the frame the press landed on, so a panel drag cannot
// turn into a scroll half way through a stroke)
inline bool OwnerWasInsidePressArea()
{
    if (st.owner == 0) return true;
    if (st.pressArea.GetHeight() <= 0.0f) return false;
    // overlap (not containment): a row that is half cut off by the bottom edge of
    // the list is still a row of the list, and a stroke from it must scroll
    const float top = ImMax(st.ownerRect.Min.y, st.pressArea.Min.y);
    const float bot = ImMin(st.ownerRect.Max.y, st.pressArea.Max.y);
    return (bot - top) > 1.0f;
}

// a widget asks for the press; the first one inside the rect wins
inline bool Claim(ImU32 id, const ImRect& r)
{
    Note(id, r);
    if (!st.wentDown) return false;
    if (st.owner != 0) return false;
    if (!r.Contains(st.pos)) return false;
    st.owner     = id;
    st.hot       = id;
    st.ownerRect = r;
    return true;
}

// an area may take the press away once the finger starts dragging (scrolling)
inline void Steal(ImU32 id) { st.owner = id; }
inline bool Owns(ImU32 id) { return st.owner == id; }

// fires on the release frame only
inline bool Tap(ImU32 id, const ImRect& r)
{
    Note(id, r);
    if (!st.wentUp) return false;
    if (st.owner != id) return false;
    if (st.moved) return false;
    return r.Contains(st.pos);
}

// hover (highlight) state
inline bool Hot(ImU32 id, const ImRect& r) { return st.owner ? (st.owner == id) : r.Contains(st.pos); }

// a released press must not stay owned, or a later stroke could fire it again
inline void EndFrame() { if (!st.down) st.owner = 0; }

} // namespace P

// ===========================================================================
//  L A Y O U T   C U R S O R
// ===========================================================================
struct Layout
{
    ImDrawList* dl       = nullptr;
    float x = 0.0f, y = 0.0f;
    float width = 0.0f;
    float clipTop = 0.0f, clipBot = 0.0f;
    float scrollY = 0.0f;
    float contentH = 0.0f;
};
inline Layout lt;

inline void BeginLayout(ImDrawList* dl, float x, float y, float width,
                        float clipTop, float clipBot, float scrollY)
{
    lt.dl = dl; lt.x = x; lt.y = y; lt.width = width;
    lt.clipTop = clipTop; lt.clipBot = clipBot;
    lt.scrollY = scrollY;
}

inline ImRect Alloc(float h, float indent = 0.0f)
{
    ImRect r(ImVec2(lt.x + indent, lt.y), ImVec2(lt.x + indent + lt.width, lt.y + h));
    lt.y += h + S(theme.rowGap);
    return r;
}

// the drawn position of a row (the cursor already includes the scroll offset)
inline bool OnScreen(const ImRect& r) { return r.Max.y > lt.clipTop - S(2.0f) && r.Min.y < lt.clipBot + S(2.0f); }

inline void Gap(float h) { lt.y += S(h); }
inline void ContentEnd() { lt.contentH = lt.y - (lt.clipTop - lt.scrollY); }

// ===========================================================================
//  W I D G E T S
// ===========================================================================

inline void Rule(ImDrawList* dl, const ImRect& r, ImVec4 col)
{
    dl->AddRectFilledMultiColor(r.Min, r.Max, U32(col, 0.0f), U32(col, 0.45f), U32(col, 0.45f), U32(col, 0.0f));
}

inline void SectionLabel(const char* text, Glyph icon = GNone)
{
    ImDrawList* dl = lt.dl;
    if (!dl) return;
    ImRect r = Alloc(S(30.0f));
    if (!OnScreen(r)) return;

    float x = r.Min.x + S(4.0f);
    if (icon != GNone)
    {
        GlyphDraw(dl, icon, ImVec2(x + S(7.0f), r.GetCenter().y), S(15.0f), U32(pal.accent, 0.95f));
        x += S(20.0f);
    }
    ImU32 col = U32(pal.accent);
    TextSpaced(dl, text, ImVec2(x, r.GetCenter().y - TextSize(text, 0.72f).y * 0.5f), col, 0.72f, S(1.4f));
    float tw = TextSpacedWidth(text, 0.72f, S(1.4f));
    ImRect line(ImVec2(x + tw + S(10.0f), r.GetCenter().y - S(0.5f)), ImVec2(r.Max.x - S(4.0f), r.GetCenter().y + S(0.5f)));
    if (line.Max.x > line.Min.x)
        Rule(dl, line, pal.accent);
}

// ---- switch (used by Row) ----
inline void SwitchVisual(ImDrawList* dl, ImVec2 center, float on, float hv)
{
    const float w = S(44.0f), h = S(23.0f), rad = h * 0.5f;
    ImRect tr(ImVec2(center.x - w * 0.5f, center.y - h * 0.5f), ImVec2(center.x + w * 0.5f, center.y + h * 0.5f));

    ImVec4 track = Mix(pal.trackOff, pal.accent, on);
    dl->AddRectFilled(tr.Min, tr.Max, U32(track, 0.55f + 0.40f * on), rad);
    if (on > 0.02f) GlowRound(dl, tr, rad, pal.accent, on * (0.85f + 0.25f * hv), 3);
    dl->AddRect(tr.Min, tr.Max, U32(on > 0.5f ? pal.accentHi : pal.borderSoft, 0.75f + 0.25f * hv), rad, 0, S(1.2f));

    float kx = ImLerp(tr.Min.x + rad, tr.Max.x - rad, on);
    float kr = rad - S(3.0f);
    dl->AddCircleFilled(ImVec2(kx, center.y), kr + S(0.6f), U32(pal.shadow, 0.55f));
    dl->AddCircleFilled(ImVec2(kx, center.y), kr, on > 0.5f ? IM_COL32(255, 255, 255, 255) : U32(pal.textSoft));
}

// ---- boolean row ----
inline bool Row(const char* id, const char* label, bool* value, Glyph icon = GNone, const char* hint = nullptr)
{
    ImDrawList* dl = lt.dl;
    if (!dl || !value) return false;

    const float h = S(theme.rowHeight);
    ImRect r = Alloc(h);
    if (!OnScreen(r)) return false;

    const ImU32 wid = P::Id(id && id[0] ? id : label);
    P::Claim(wid, r);
    bool changed = false;
    if (P::Tap(wid, r)) { *value = !*value; changed = true; }

    const float on = Anim(wid ^ 0x9E37u, *value ? 1.0f : 0.0f);
    const float hv = Anim(wid ^ 0x1D2Fu, (P::Hot(wid, r) && !P::st.moved) ? 1.0f : 0.0f);
    const float rr = S(theme.radius);

    ImVec4 fill = Mix(Mix(pal.bgRow, pal.bgRowHot, hv), pal.bgRowOn, on);
    if (on > 0.02f) GlowRound(dl, r, rr, pal.accent, on * 0.55f * theme.glowStrength);
    dl->AddRectFilled(r.Min, r.Max, U32(fill, fill.w * theme.windowOpacity), rr);
    dl->AddRect(r.Min, r.Max, U32(on > 0.02f ? pal.accent : pal.borderSoft, 0.35f + 0.55f * on + 0.15f * hv), rr, 0, S(1.2f));

    // active edge bar
    if (on > 0.02f)
    {
        ImRect bar(ImVec2(r.Min.x + S(1.0f), r.Min.y + S(5.0f)), ImVec2(r.Min.x + S(3.6f), r.Max.y - S(5.0f)));
        dl->AddRectFilled(bar.Min, bar.Max, U32(pal.accentHi, 0.30f + 0.65f * on), S(2.0f));
    }

    // icon tile
    float x = r.Min.x + S(13.0f);
    if (icon != GNone)
    {
        ImRect tile(ImVec2(x, r.GetCenter().y - S(14.0f)), ImVec2(x + S(28.0f), r.GetCenter().y + S(14.0f)));
        dl->AddRectFilled(tile.Min, tile.Max, U32(on > 0.02f ? pal.accent : pal.borderSoft, 0.16f + 0.24f * on), S(9.0f));
        GlyphDraw(dl, icon, tile.GetCenter(), S(16.0f), U32(on > 0.4f ? pal.accentHi : pal.textSoft, 0.85f + 0.15f * on), 1.0f);
        x = tile.Max.x + S(11.0f);
    }

    const float swArea = S(62.0f);                       // space reserved for the switch
    ImVec2 labelSize = TextSize(label, 1.0f);
    float labelY = hint ? (r.GetCenter().y - labelSize.y - S(1.0f)) : (r.GetCenter().y - labelSize.y * 0.5f);
    TextAt(dl, label, ImVec2(x, labelY), U32(Mix(pal.textSoft, pal.text, on)), 1.0f);
    if (hint && hint[0])
    {
        ImVec2 hs = TextSize(hint, 0.74f);
        float hx = r.Max.x - swArea - hs.x - S(4.0f);
        if (hx < x + labelSize.x + S(8.0f)) hx = x + labelSize.x + S(8.0f);
        TextAt(dl, hint, ImVec2(hx, r.GetCenter().y + S(1.0f)), U32(pal.textDim), 0.74f);
    }

    SwitchVisual(dl, ImVec2(r.Max.x - S(34.0f), r.GetCenter().y), on, hv);
    return changed;
}

// ---- float slider (drag the whole row horizontally) ----
inline bool Slider(const char* id, const char* label, float* value, float mn, float mx,
                   const char* fmt = "%.1f", Glyph icon = GNone, const char* suffix = nullptr)
{
    ImDrawList* dl = lt.dl;
    if (!dl || !value) return false;

    ImRect r = Alloc(S(theme.rowHeight + 16.0f));
    if (!OnScreen(r)) return false;

    const ImU32 wid = P::Id(id && id[0] ? id : label);
    P::Claim(wid, r);
    const bool active = P::Owns(wid) && P::st.down;

    bool changed = false;
    if (active)
    {
        ImRect track(ImVec2(r.Min.x + S(12.0f), r.Max.y - S(20.0f)), ImVec2(r.Max.x - S(12.0f), r.Max.y - S(8.0f)));
        float t = (P::st.pos.x - track.Min.x) / ImMax(track.GetWidth(), 1.0f);
        t = Clamp01(t);
        float nv = mn + (mx - mn) * t;
        if (nv != *value) { *value = nv; changed = true; }
    }

    float t = (*value - mn) / (mx - mn != 0.0f ? (mx - mn) : 1.0f);
    t = Clamp01(t);
    const float anim = Anim(wid ^ 0x77u, t);
    const float hv   = Anim(wid ^ 0x33u, (P::Hot(wid, r) || active) ? 1.0f : 0.0f);

    const float rr = S(theme.radius);
    ImVec4 fill = Mix(pal.bgRow, pal.bgRowHot, hv * 0.8f);
    if (active) fill = Mix(fill, pal.bgRowOn, 0.7f);
    dl->AddRectFilled(r.Min, r.Max, U32(fill, fill.w * theme.windowOpacity), rr);
    dl->AddRect(r.Min, r.Max, U32(active ? pal.accent : pal.borderSoft, active ? 0.95f : (0.40f + 0.30f * hv)), rr, 0, S(1.2f));

    float x = r.Min.x + S(13.0f);
    if (icon != GNone)
    {
        GlyphDraw(dl, icon, ImVec2(x + S(8.0f), r.Min.y + S(20.0f)), S(16.0f), U32(pal.accent, 0.80f + 0.20f * hv));
        x += S(26.0f);
    }
    TextAt(dl, label, ImVec2(x, r.Min.y + S(13.0f)), U32(pal.text, 0.92f), 1.0f);

    char buf[64];
    char num[48];
    snprintf(num, sizeof(num), fmt, *value);
    if (suffix && suffix[0]) snprintf(buf, sizeof(buf), "%s%s", num, suffix);
    else                     snprintf(buf, sizeof(buf), "%s", num);

    ImRect chip(ImVec2(r.Max.x - S(14.0f) - ImMax(TextSize(buf, 0.9f).x + S(18.0f), S(58.0f)), r.Min.y + S(9.0f)),
                ImVec2(r.Max.x - S(14.0f), r.Min.y + S(33.0f)));
    dl->AddRectFilled(chip.Min, chip.Max, U32(pal.accent, active ? 0.26f : 0.13f), S(9.0f));
    dl->AddRect(chip.Min, chip.Max, U32(pal.accent, active ? 0.75f : 0.38f), S(9.0f), 0, S(1.1f));
    TextCenter(dl, chip, buf, U32(active ? pal.accentHi : pal.text), 0.90f);

    // glowing track
    ImRect track(ImVec2(r.Min.x + S(12.0f), r.Max.y - S(20.0f)), ImVec2(r.Max.x - S(12.0f), r.Max.y - S(8.0f)));
    float ty = track.GetCenter().y;
    dl->AddRectFilledMultiColor(ImVec2(track.Min.x, ty - S(2.0f)), ImVec2(track.Max.x, ty + S(2.0f)),
                                U32(pal.accentDeep, 0.45f), U32(pal.accent, 0.55f),
                                U32(pal.accent, 0.55f), U32(pal.accentDeep, 0.45f));
    if (anim > 0.01f)
    {
        dl->AddRectFilled(ImVec2(track.Min.x, ty - S(3.0f)), ImVec2(track.Min.x + track.GetWidth() * anim, ty + S(3.0f)),
                          U32(pal.accent, 0.75f), S(3.0f));
        GlowRound(dl, ImRect(ImVec2(track.Min.x, ty - S(3.0f)), ImVec2(track.Min.x + track.GetWidth() * anim, ty + S(3.0f))),
                  S(3.0f), pal.accent, 0.55f * (0.5f + hv), 3);
    }
    dl->AddCircleFilled(ImVec2(track.Min.x + track.GetWidth() * anim, ty), S(6.5f), IM_COL32(255, 255, 255, 255));
    dl->AddCircle(ImVec2(track.Min.x + track.GetWidth() * anim, ty), S(8.5f), U32(pal.accent, 0.65f), 0, S(1.4f));
    return changed;
}

// ---- integer slider ----
inline bool SliderInt(const char* id, const char* label, int* value, int mn, int mx,
                      const char* fmt = "%d", Glyph icon = GNone, const char* suffix = nullptr)
{
    if (!value) return false;
    float f = (float)*value;
    if (Slider(id, label, &f, (float)mn, (float)mx, fmt, icon, suffix))
    {
        int nv = (int)(f + (f >= 0.0f ? 0.5f : -0.5f));
        if (nv < mn) nv = mn;
        if (nv > mx) nv = mx;
        if (nv != *value) { *value = nv; return true; }
    }
    return false;
}

// ---- segmented choice ----
inline bool Choice(const char* id, const char* label, int* index, const char* const* items, int count)
{
    ImDrawList* dl = lt.dl;
    if (!dl || !index || !items || count <= 0) return false;

    ImRect r = Alloc(S(theme.rowHeight + 20.0f));
    if (!OnScreen(r)) return false;

    if (label && label[0])
        TextAt(dl, label, ImVec2(r.Min.x + S(13.0f), r.Min.y + S(9.0f)), U32(pal.textSoft, 0.95f), 0.92f);

    ImRect bar(ImVec2(r.Min.x + S(10.0f), r.Min.y + S(30.0f)), ImVec2(r.Max.x - S(10.0f), r.Max.y - S(4.0f)));
    dl->AddRectFilled(bar.Min, bar.Max, U32(pal.bgBar, 0.92f * theme.windowOpacity), S(10.0f));
    dl->AddRect(bar.Min, bar.Max, U32(pal.borderSoft, 0.9f), S(10.0f), 0, S(1.1f));

    const float gap = S(3.0f);
    float cw = (bar.GetWidth() - gap * (count - 1)) / (float)count;
    bool changed = false;

    for (int i = 0; i < count; ++i)
    {
        ImRect cell(ImVec2(bar.Min.x + (cw + gap) * i, bar.Min.y), ImVec2(bar.Min.x + (cw + gap) * i + cw, bar.Max.y));
        char cid[96];
        snprintf(cid, sizeof(cid), "%s#%d", (id && id[0]) ? id : label, i);
        const ImU32 wid = P::Id(cid);

        P::Claim(wid, cell);
        if (P::Tap(wid, cell) && *index != i) { *index = i; changed = true; }

        const bool sel = (*index == i);
        const float a = Anim(wid ^ 0x55u, sel ? 1.0f : 0.0f);
        const float hv = Anim(wid ^ 0xAAu, (P::Hot(wid, cell) && !sel) ? 1.0f : 0.0f);

        if (a > 0.02f)
        {
            GlowRound(dl, cell, S(8.0f), pal.accent, a * 0.85f, 3);
            dl->AddRectFilled(cell.Min, cell.Max, U32(pal.accent, 0.22f + 0.24f * a), S(8.0f));
        }
        else if (hv > 0.02f)
        {
            dl->AddRectFilled(cell.Min, cell.Max, U32(pal.bgRowHot, 0.75f * hv), S(8.0f));
        }
        dl->AddRect(cell.Min, cell.Max, U32(sel ? pal.accentHi : pal.borderSoft, sel ? 0.85f : 0.55f), S(8.0f), 0, S(1.1f));
        TextCenter(dl, cell, items[i], U32(sel ? pal.accentHi : pal.textSoft, 0.75f + 0.25f * a), 0.86f);
    }
    return changed;
}

// ---- generic button inside an explicit rect ----
inline bool Btn(const char* id, const ImRect& r, const char* label, Glyph icon = GNone,
                bool primary = false, bool danger = false)
{
    ImDrawList* dl = lt.dl;
    if (!dl) return false;

    const ImU32 wid = P::Id(id && id[0] ? id : label);
    P::Claim(wid, r);
    bool fired = false;
    if (P::Tap(wid, r)) fired = true;

    const float hv  = Anim(wid ^ 0x5Au, (P::Hot(wid, r) && !P::st.moved) ? 1.0f : 0.0f);
    const float prs = Anim(wid ^ 0xA5u, (P::Owns(wid) && P::st.down) ? 1.0f : 0.0f);
    const float rr  = S(theme.radius * 0.85f);

    ImVec4 edge = danger ? pal.danger : pal.accent;
    ImVec4 base = primary ? Mix(pal.accentDeep, pal.accent, 0.35f + 0.30f * hv)
                          : Mix(pal.bgBar, pal.bgRowHot, hv);
    if (prs > 0.02f) base = Mix(base, edge, 0.25f * prs);

    if (primary) GlowRound(dl, r, rr, edge, (0.75f + 0.45f * hv) * theme.glowStrength, 4);
    dl->AddRectFilled(r.Min, r.Max, U32(base, primary ? (0.72f + 0.18f * hv) : (0.92f * theme.windowOpacity)), rr);
    dl->AddRect(r.Min, r.Max, U32(edge, primary ? 0.90f : (0.38f + 0.45f * hv)), rr, 0, S(1.2f));

    float tw = TextSize(label, 0.94f).x;
    float gs = (icon != GNone) ? S(20.0f) : 0.0f;
    float x  = r.GetCenter().x - (tw + gs) * 0.5f;
    if (icon != GNone)
    {
        GlyphDraw(dl, icon, ImVec2(x + S(7.0f), r.GetCenter().y), S(16.0f),
                  primary ? U32(pal.accentHi) : U32(pal.textSoft), 1.0f);
        x += gs;
    }
    TextVCenter(dl, r, x, label, primary ? U32(pal.accentHi) : U32(pal.text, 0.92f), 0.94f);
    return fired;
}

// ---- square keycap (keypad) ----
inline bool Keycap(const char* id, const char* label, const ImRect& r, bool accent = false, Glyph icon = GNone)
{
    ImDrawList* dl = lt.dl;
    if (!dl) return false;

    const ImU32 wid = P::Id(id);
    P::Claim(wid, r);
    bool fired = false;
    if (P::Tap(wid, r)) fired = true;

    const float hv  = Anim(wid ^ 0x11u, (P::Hot(wid, r) && !P::st.moved) ? 1.0f : 0.0f);
    const float prs = Anim(wid ^ 0x22u, (P::Owns(wid) && P::st.down) ? 1.0f : 0.0f);
    const float rr  = S(9.0f);

    ImVec4 edge = accent ? pal.accent : pal.border;
    ImVec4 fill = Mix(Mix(pal.bgBar, pal.bgRowHot, hv), pal.accent, 0.20f * prs);
    if (accent) GlowRound(dl, r, rr, edge, (0.55f + 0.45f * hv) * theme.glowStrength, 3);

    dl->AddRectFilled(r.Min, r.Max, U32(fill, (accent ? 0.80f : 0.90f) * theme.windowOpacity), rr);
    dl->AddRect(r.Min, r.Max, U32(edge, accent ? 0.85f : (0.40f + 0.45f * hv)), rr, 0, S(1.1f));

    if (icon != GNone) GlyphDraw(dl, icon, r.GetCenter(), S(17.0f), U32(accent ? pal.accentHi : pal.text, 0.95f), 1.0f);
    else               TextCenter(dl, r, label, U32(accent ? pal.accentHi : pal.text, 0.95f), 0.95f);
    return fired;
}

// ---- meter / progress ----
inline void Meter(const ImRect& r, float t, ImVec4 col)
{
    ImDrawList* dl = lt.dl;
    if (!dl) return;
    t = Clamp01(t);
    dl->AddRectFilled(r.Min, r.Max, U32(pal.bgBar, 0.85f), r.GetHeight() * 0.5f);
    ImRect fill(r.Min, ImVec2(r.Min.x + r.GetWidth() * t, r.Max.y));
    if (t > 0.01f)
    {
        GlowRound(dl, fill, r.GetHeight() * 0.5f, col, 0.5f, 3);
        dl->AddRectFilled(fill.Min, fill.Max, U32(col, 0.85f), r.GetHeight() * 0.5f);
    }
}

// ===========================================================================
//  L O G O
// ===========================================================================
inline ImTextureID LogoTexture = nullptr;
inline float       LogoAspect  = 1.0f;   // width / height, set when the texture is uploaded
bool CreateLogoTexture();
void ReleaseLogoTexture();

inline void LogoDraw(ImDrawList* dl, ImVec2 center, float size, float rounding = 0.0f, bool glow = true)
{
    if (!dl) return;
    const float rad = size * 0.5f;
    ImRect r(ImVec2(center.x - rad, center.y - rad), ImVec2(center.x + rad, center.y + rad));
    if (rounding <= 0.0f) rounding = rad;

    if (glow) GlowCircle(dl, center, rad, pal.accent, 1.0f * theme.glowStrength, 3);

    if (LogoTexture)
    {
        dl->AddImageRounded(LogoTexture, r.Min, r.Max, ImVec2(0, 0), ImVec2(1, 1),
                            IM_COL32(255, 255, 255, 255), rounding);
    }
    else
    {
        dl->AddCircleFilled(center, rad, U32(pal.accentDeep));
        const float sc = ScaleForPx(rad * 0.62f);      // monogram, sized to the disc
        ImVec2 t = TextSize("SM", sc);
        TextAt(dl, "SM", ImVec2(center.x - t.x * 0.5f, center.y - t.y * 0.5f), IM_COL32(255, 255, 255, 255), sc);
    }
    dl->AddCircle(center, rad - S(1.0f), U32(pal.accentHi, 0.85f), 0, S(1.6f));
    dl->AddCircle(center, rad + S(2.0f), U32(pal.accent, 0.35f), 0, S(2.0f));
}

// ===========================================================================
//  S T Y L E
// ===========================================================================
inline void ApplyAccent(int index)
{
    if (index < 0 || index >= kAccentCount) index = 0;
    theme.accentIndex = index;
    pal.accent     = kAccents[index].main;
    pal.accentDeep = kAccents[index].deep;
    pal.accentHi   = kAccents[index].hi;
    pal.bgRowOn    = ImVec4(kAccents[index].deep.x * 0.55f + 0.04f,
                            kAccents[index].deep.y * 0.35f + 0.03f,
                            kAccents[index].deep.z * 0.55f + 0.09f, 0.960f);
}

inline void ApplyStyle()
{
    ImGuiStyle& s = ImGui::GetStyle();
    s.WindowRounding    = S(theme.radius);
    s.FrameRounding     = S(10.0f);
    s.GrabRounding      = S(10.0f);
    s.ScrollbarRounding = S(10.0f);
    s.WindowBorderSize  = 0.0f;
    s.WindowPadding     = ImVec2(0.0f, 0.0f);
    s.ItemSpacing       = ImVec2(S(8.0f), S(8.0f));
    s.Colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 0);
    s.Colors[ImGuiCol_Text]     = pal.text;
}

} // namespace ShadowUI
