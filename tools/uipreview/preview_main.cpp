// ============================================================================
//  SHADOW MODZ - desktop preview renderer
// ----------------------------------------------------------------------------
//  Compiles the real ImGui 1.89.2 + the real UI headers + the real font data
//  and rasterizes the result with a small software rasterizer, so the menu can
//  be inspected (and screenshotted) without an Android device.
//
//  Build (see tools/uipreview/README.md):
//    g++ -std=c++17 -O1 -I tools/uipreview -I "$JNI" -I "$JNI/imgui" -I "$JNI/Icon" \
//        -I "$JNI/SHADOWMODZ MODS" tools/uipreview/preview_main.cpp \
//        "$JNI"/imgui/imgui.cpp "$JNI"/imgui/imgui_draw.cpp \
//        "$JNI"/imgui/imgui_widgets.cpp "$JNI"/imgui/imgui_tables.cpp -o preview
//    ./preview aim docs/ui_aim.ppm [WxH]
//  scenarios: lock lockfail aim esp catch misc system launcher toast
// ============================================================================

#include "imgui.h"
#include "imgui_internal.h"

#include "host_stub.hpp"

#include "Icon/Icon.h"
#include "Icon/Font.h"
#include "Icon/Iconcpp.h"

#include "SHADOWMODZ MODS/ShadowTouch.hpp"
#include "SHADOWMODZ MODS/ShadowAuth.hpp"
#include "SHADOWMODZ MODS/ShadowTheme.hpp"
#include "SHADOWMODZ MODS/ShadowMenu.hpp"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
//  software rasterizer
// ---------------------------------------------------------------------------
static std::vector<unsigned char> g_atlas;
static int g_aw = 0, g_ah = 0;

struct Canvas
{
    int w = 0, h = 0;
    std::vector<unsigned int> px;
    void init(int W, int H) { w = W; h = H; px.assign((size_t)W * H, 0xFF0A0A0Cu); }
    inline void blend(int x, int y, float r, float g, float b, float a)
    {
        if (x < 0 || y < 0 || x >= w || y >= h || a <= 0.0f) return;
        unsigned int& d = px[(size_t)y * w + x];
        float dr = (float)((d >> 16) & 0xFF) / 255.0f;
        float dg = (float)((d >> 8) & 0xFF) / 255.0f;
        float db = (float)(d & 0xFF) / 255.0f;
        r = r * a + dr * (1.0f - a);
        g = g * a + dg * (1.0f - a);
        b = b * a + db * (1.0f - a);
        unsigned int R = (unsigned int)(r * 255.0f + 0.5f);
        unsigned int G = (unsigned int)(g * 255.0f + 0.5f);
        unsigned int B = (unsigned int)(b * 255.0f + 0.5f);
        d = 0xFF000000u | (R << 16) | (G << 8) | B;
    }
};

static inline void SampleBilinear(float u, float v, float* out, float* alpha)
{
    if (g_aw <= 0 || g_ah <= 0) { out[0] = out[1] = out[2] = 1.0f; *alpha = 1.0f; return; }
    float x = u * (float)g_aw - 0.5f;
    float y = v * (float)g_ah - 0.5f;
    int x0 = (int)floorf(x), y0 = (int)floorf(y);
    float fx = x - (float)x0, fy = y - (float)y0;
    const unsigned char* p = g_atlas.data();
    auto texel = [&](int tx, int ty, int c) -> float
    {
        if (tx < 0) tx = 0; if (ty < 0) ty = 0;
        if (tx >= g_aw) tx = g_aw - 1; if (ty >= g_ah) ty = g_ah - 1;
        return (float)p[((size_t)ty * g_aw + tx) * 4 + c] / 255.0f;
    };
    for (int c = 0; c < 4; ++c)
    {
        float v00 = texel(x0, y0, c), v10 = texel(x0 + 1, y0, c);
        float v01 = texel(x0, y0 + 1, c), v11 = texel(x0 + 1, y0 + 1, c);
        float val = (v00 * (1 - fx) + v10 * fx) * (1 - fy) + (v01 * (1 - fx) + v11 * fx) * fy;
        if (c == 3) *alpha = val; else out[c] = val;
    }
}

static void Rasterize(const ImDrawData* dd, Canvas& cv)
{
    for (int n = 0; n < dd->CmdListsCount; ++n)
    {
        const ImDrawList* cl = dd->CmdLists[n];
        const ImDrawVert* vtx = cl->VtxBuffer.Data;
        const ImDrawIdx* idx = cl->IdxBuffer.Data;

        for (int ci = 0; ci < cl->CmdBuffer.Size; ++ci)
        {
            const ImDrawCmd* cmd = &cl->CmdBuffer[ci];
            if (cmd->UserCallback) continue;

            const float cx0 = cmd->ClipRect.x, cy0 = cmd->ClipRect.y;
            const float cx1 = cmd->ClipRect.z, cy1 = cmd->ClipRect.w;

            for (unsigned int i = 0; i + 2 < cmd->ElemCount; i += 3)
            {
                const ImDrawVert& a = vtx[idx[cmd->IdxOffset + i + 0]];
                const ImDrawVert& b = vtx[idx[cmd->IdxOffset + i + 1]];
                const ImDrawVert& c = vtx[idx[cmd->IdxOffset + i + 2]];

                float minx = ImMin(a.pos.x, ImMin(b.pos.x, c.pos.x));
                float maxx = ImMax(a.pos.x, ImMax(b.pos.x, c.pos.x));
                float miny = ImMin(a.pos.y, ImMin(b.pos.y, c.pos.y));
                float maxy = ImMax(a.pos.y, ImMax(b.pos.y, c.pos.y));
                minx = ImMax(minx, cx0); miny = ImMax(miny, cy0);
                maxx = ImMin(maxx, cx1); maxy = ImMin(maxy, cy1);
                int x0 = (int)floorf(minx), x1 = (int)ceilf(maxx);
                int y0 = (int)floorf(miny), y1 = (int)ceilf(maxy);
                if (x1 <= x0 || y1 <= y0) continue;

                float area = (b.pos.x - a.pos.x) * (c.pos.y - a.pos.y) - (c.pos.x - a.pos.x) * (b.pos.y - a.pos.y);
                if (fabsf(area) < 1e-6f) continue;

                for (int y = y0; y < y1; ++y)
                {
                    for (int x = x0; x < x1; ++x)
                    {
                        float px = (float)x + 0.5f, py = (float)y + 0.5f;
                        float wa = ((b.pos.x - px) * (c.pos.y - py) - (c.pos.x - px) * (b.pos.y - py)) / area;
                        float wb = ((c.pos.x - px) * (a.pos.y - py) - (a.pos.x - px) * (c.pos.y - py)) / area;
                        float wc = 1.0f - wa - wb;
                        if (wa < 0.0f || wb < 0.0f || wc < 0.0f) continue;

                        float u = wa * a.uv.x + wb * b.uv.x + wc * c.uv.x;
                        float v = wa * a.uv.y + wb * b.uv.y + wc * c.uv.y;
                        float tex[4] = { 1, 1, 1, 1 };
                        float talpha = 1.0f;
                        SampleBilinear(u, v, tex, &talpha);

                        auto col = [&](unsigned int cc, int ch) { return (float)((cc >> (ch * 8)) & 0xFF) / 255.0f; };
                        float r  = wa * col(a.col, 0) + wb * col(b.col, 0) + wc * col(c.col, 0);
                        float g  = wa * col(a.col, 1) + wb * col(b.col, 1) + wc * col(c.col, 1);
                        float bl = wa * col(a.col, 2) + wb * col(b.col, 2) + wc * col(c.col, 2);
                        float al = wa * col(a.col, 3) + wb * col(b.col, 3) + wc * col(c.col, 3);

                        cv.blend(x, y, r * tex[0], g * tex[1], bl * tex[2], al * talpha);
                    }
                }
            }
        }
    }
}

static void WritePPM(const char* path, const Canvas& cv)
{
    FILE* f = fopen(path, "wb");
    if (!f) { printf("cannot write %s\n", path); return; }
    fprintf(f, "P6\n%d %d\n255\n", cv.w, cv.h);
    for (size_t i = 0; i < cv.px.size(); ++i)
    {
        unsigned int d = cv.px[i];
        unsigned char rgb[3] = { (unsigned char)((d >> 16) & 0xFF), (unsigned char)((d >> 8) & 0xFF), (unsigned char)(d & 0xFF) };
        fwrite(rgb, 1, 3, f);
    }
    fclose(f);
}

// ---------------------------------------------------------------------------
//  frame helpers
// ---------------------------------------------------------------------------
static ShadowMenu::Refs g_refs;
static float kW = 720.0f, kH = 1280.0f;

static void Frame(float mx = -FLT_MAX, float my = -FLT_MAX, bool down = false)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(kW, kH);
    io.DeltaTime   = 1.0f / 60.0f;
    if (mx > -1e30f) io.MousePos = ImVec2(mx, my);
    io.MouseDown[0] = down;
    ImGui::NewFrame();
    ShadowMenu::Draw(g_refs, kW, kH);
    ImGui::Render();
}

static void Settle(int n = 14) { for (int i = 0; i < n; ++i) Frame(); }

static void TapAt(float x, float y)
{
    Frame(x, y, false);
    Frame(x, y, true);
    Frame(x, y, false);
}

static bool TapWidget(const char* id)
{
    ImRect r;
    Frame();
    if (!ShadowUI::P::FindRect(id, r)) { printf("  (preview: '%s' not visible)\n", id); return false; }
    TapAt(r.GetCenter().x, r.GetCenter().y);
    return true;
}

// ---------------------------------------------------------------------------
int main(int argc, char** argv)
{
    const char* scenario = argc > 1 ? argv[1] : "aim";
    const char* outPath  = argc > 2 ? argv[2] : "preview.ppm";
    if (argc > 3)
    {
        float w = 720.0f, h = 1280.0f;
        if (sscanf(argv[3], "%fx%f", &w, &h) == 2 && w > 16.0f && h > 16.0f) { kW = w; kH = h; }
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(kW, kH);
    io.DeltaTime   = 1.0f / 60.0f;
    io.IniFilename = nullptr;

    // exactly the atlas the device builds (SetupImgui in Gui.hpp)
    ImFontConfig cf; cf.FontDataOwnedByAtlas = false;
    io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 26.0f, &cf);
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig ic; ic.MergeMode = true; ic.PixelSnapH = true; ic.OversampleH = 3; ic.OversampleV = 3;
    io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 26.0f, &ic, icons_ranges);
    io.Fonts->Build();

    unsigned char* pixels = nullptr;
    int aw = 0, ah = 0;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &aw, &ah);
    g_atlas.assign(pixels, pixels + (size_t)aw * ah * 4);
    g_aw = aw; g_ah = ah;
    io.Fonts->SetTexID((ImTextureID)(intptr_t)1);

    ShadowUI::theme.fontSize = 26.0f;
    ShadowUI::ApplyStyle();

#define BIND_BOOL(name, target)   g_refs.name = &target;
#define BIND_FLOAT(name, target)  g_refs.name = &target;
#define BIND_INT(name, target)    g_refs.name = &target;
    SHADOW_BOOL_FIELDS(BIND_BOOL)
    SHADOW_FLOAT_FIELDS(BIND_FLOAT)
    SHADOW_INT_FIELDS(BIND_INT)
#undef BIND_BOOL
#undef BIND_FLOAT
#undef BIND_INT

    ShadowMenu::hooks.openURL        = [](const char* u) { g_urlOpened = u; };
    ShadowMenu::hooks.applyAimHeight = [](int) {};
    ShadowUI::CreateLogoTexture();

    // the window is opened by the boot rule; every scenario below starts from
    // a known state instead of relying on it
    ShadowMenu::booted = true;

    const std::string sc(scenario);

    if (sc == "lock" || sc == "lockfail")
    {
        ShadowAuth::unlocked = false;
        ShadowMenu::keyFail  = false;
        ShadowMenu::KeyClear();
        ShadowMenu::open = true;
        if (sc == "lockfail")
        {
            ShadowMenu::KeyClear();
            const char* wrong = "ABCDEF";
            for (const char* p = wrong; *p; ++p)
            {
                int idx = (*p >= 'A' && *p <= 'Z') ? (*p - 'A') : (26 + (*p - '0'));
                char id[16]; snprintf(id, sizeof(id), "##key%d", idx);
                TapWidget(id);
            }
            TapWidget("##key_ok");
        }
        else
        {
            const char* part = "SHADO";
            for (const char* p = part; *p; ++p)
            {
                int idx = *p - 'A';
                char id[16]; snprintf(id, sizeof(id), "##key%d", idx);
                TapWidget(id);
            }
        }
        Settle(8);
    }
    else
    {
        // unlocked menu with a realistic mix of features on
        ShadowAuth::unlocked = true;
        ShadowMenu::open     = true;

        *g_refs.Enable = true;
        *g_refs.Aimbot = true;  *g_refs.Headshot = true; *g_refs.SilentAim = true;
        *g_refs.AimLock = true; *g_refs.Antidev = true;  *g_refs.SpinBot = true;
        *g_refs.EspBox = true;  *g_refs.EspLine = true;  *g_refs.EspHealth = true;
        *g_refs.EspBones = true; *g_refs.EspDistance = true; *g_refs.EspFovCircle = true;
        *g_refs.NightMode = true; *g_refs.SniperAlert = true;
        *g_refs.SpeedHack = true; *g_refs.FreeMove = true; *g_refs.NoRecoil = true;
        *g_refs.AutoRevive = true; *g_refs.GliderHack = true; *g_refs.ShurikenKill = true;
        *g_refs.Watermark = true; *g_refs.FirewallOn = true;
        *g_refs.FovAim = 120.0f; *g_refs.AimDistance = 220.0f; *g_refs.AimSpeed = 0.42f;
        *g_refs.LineThickness = 1.6f; *g_refs.BoxThickness = 1.4f;
        *g_refs.SpeedMultiplier = 2.4f; *g_refs.Sensitivity = 62.0f;
        *g_refs.AimHeight = 4; *g_refs.AimWhen = 0; *g_refs.BoxStyle = 1;
        *g_refs.LineMode = 1; *g_refs.CarYOffset = 3;
        *g_refs.AutoKillDelay = 250.0f; *g_refs.MedkitRate = 6.0f;
        *g_refs.MassKillRange = 24.0f; *g_refs.AimLockFov = 210.0f;

        ShadowMenu::tab = (sc == "esp") ? 1 : (sc == "catch") ? 2 : (sc == "misc") ? 3 :
                          (sc == "system") ? 4 : 0;
        ShadowMenu::scrollY = (sc == "system") ? 0.0f : 0.0f;
        Settle(10);

        if (sc == "toast")
        {
            ShadowMenu::Toast("SETTINGS SAVED", ImVec4(0.29f, 0.90f, 0.54f, 1.0f), 30.0f);
            Settle(8);
        }
        if (sc == "launcher")
        {
            TapWidget("##foot_hide");
            Settle(20);
        }
    }

    Canvas cv;
    cv.init((int)kW, (int)kH);
    Rasterize(ImGui::GetDrawData(), cv);
    WritePPM(outPath, cv);
    printf("wrote %s (%s)  %dx%d\n", outPath, scenario, cv.w, cv.h);
    return 0;
}
