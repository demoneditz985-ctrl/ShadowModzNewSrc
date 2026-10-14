// ============================================================================
//  SHADOW MODZ - desktop preview / smoke test harness
//  Compiles the real ImGui 1.89.2 together with the mod's new UI headers and
//  rasterizes the resulting draw data with a tiny software rasterizer so the
//  menu can be verified (and screenshotted) without an Android device.
// ============================================================================

#include "imgui.h"
#include "imgui_internal.h"

#include "host_stub.hpp"

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
    void init(int W, int H) { w = W; h = H; px.assign((size_t)W * H, 0xFF000000u); }
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
                        float r = wa * col(a.col, 0) + wb * col(b.col, 0) + wc * col(c.col, 0);
                        float g = wa * col(a.col, 1) + wb * col(b.col, 1) + wc * col(c.col, 1);
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
static int  g_frames = 0;
static bool g_crashed = false;

static void RunUI()
{
    const ImVec2 disp = ImGui::GetIO().DisplaySize;
    ShadowMenu::Draw(g_refs, disp.x, disp.y);
}

static void Frame(float mx = -FLT_MAX, float my = -FLT_MAX, bool down = false)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(720.0f, 1280.0f);
    io.DeltaTime = 1.0f / 60.0f;
    if (mx > -1e30f) io.MousePos = ImVec2(mx, my);
    io.MouseDown[0] = down;
    ImGui::NewFrame();
    RunUI();
    ImGui::Render();
    ++g_frames;
}

static void ClickAt(float x, float y)
{
    Frame(x, y, false);
    Frame(x, y, true);
    Frame(x, y, false);
}

// ---------------------------------------------------------------------------
int main(int argc, char** argv)
{
    const char* scenario = argc > 1 ? argv[1] : "aimbot";
    const char* outPath  = argc > 2 ? argv[2] : "preview.ppm";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(720.0f, 1280.0f);
    io.DeltaTime = 1.0f / 60.0f;
    io.IniFilename = nullptr;
    io.Fonts->AddFontDefault();

    unsigned char* pixels = nullptr;
    int aw = 0, ah = 0;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &aw, &ah);
    g_atlas.assign(pixels, pixels + (size_t)aw * ah * 4);
    g_aw = aw; g_ah = ah;
    io.Fonts->SetTexID((ImTextureID)1);

    // same binding the mod performs in ShadowBindMenu() (shadowmodz_main.cpp)
#define SHADOW_BIND_BOOL(name, target)   g_refs.name = &target;
#define SHADOW_BIND_FLOAT(name, target)  g_refs.name = &target;
#define SHADOW_BIND_INT(name, target)    g_refs.name = &target;
    SHADOW_BOOL_FIELDS(SHADOW_BIND_BOOL)
    SHADOW_FLOAT_FIELDS(SHADOW_BIND_FLOAT)
    SHADOW_INT_FIELDS(SHADOW_BIND_INT)
#undef SHADOW_BIND_BOOL
#undef SHADOW_BIND_FLOAT
#undef SHADOW_BIND_INT

    // ---------------------------------------------------------------- scenario
    std::string sc(scenario);

    if (sc == "menu_aimbot" || sc == "menu_visual" || sc == "menu_network" || sc == "menu_settings")
    {
        ShadowMenu::open = true;
        ShadowMenu::tab  = (sc == "menu_aimbot") ? 0 : (sc == "menu_visual") ? 1 : (sc == "menu_network") ? 3 : 4;

        *g_refs.Enable = true; *g_refs.Aimbot = true; *g_refs.Headshot = true;
        *g_refs.SilentAim = true; *g_refs.SilentAim = false;
        *g_refs.AimKill1 = true; *g_refs.AimLock = true; *g_refs.AutoKill = false;
        *g_refs.AimVisibleThread = true; *g_refs.Antidev = true;
        *g_refs.EspBox = true; *g_refs.EspLine = true; *g_refs.EspHealth = true;
        *g_refs.EspBones = true; *g_refs.EspDistance = true; *g_refs.EspFovCircle = true;
        *g_refs.SpeedHack = true; *g_refs.FreeMove = true; *g_refs.NoRecoil = true;
        *g_refs.AutoRevive = true; *g_refs.SpoofName = true;
        *g_refs.Watermark = true;
        ShadowMenu::SavePreset(g_refs, 0);

        g_FwStats.totalChecks = 1286; g_FwStats.totalAllowed = 1102; g_FwStats.totalBlocked = 184;
        g_FwStats.anogsBlocked = 96; g_FwStats.domainBlocked = 41; g_FwStats.portBlocked = 33;
        g_FwStats.privIpBlocked = 14; g_FwStats.libsHooked = 4; g_FwStats.hooksOk = true;
        const char* libs[4] = { "libanogs.so", "libunity.so", "libil2cpp.so", "libmain.so" };
        const char* tgt[4]  = { "cloud.anogs.com", "10.0.0.4", "api.garena.com", "lixuim.net" };
        const char* reas[4] = { "BLACKLIST domain", "Private IP range", "Port 443 blocked", "whitelisted" };
        for (int i = 0; i < 14; ++i)
        {
            FW_LogEntry e;
            char t[32]; snprintf(t, sizeof(t), "13:%02d:%02d", 40 + i / 2, (i * 7) % 60);
            e.time = t; e.libName = libs[i % 4]; e.target = tgt[i % 4];
            e.port = (i % 3) ? "443" : "8080"; e.isBlocked = (i % 3) != 2; e.reason = reas[i % 4];
            g_fwLogs.push_back(e);
        }
        while (g_fwLogs.size() > FW_MAX_LOGS) g_fwLogs.erase(g_fwLogs.begin());
    }
    else if (sc == "launcher")
    {
        ShadowMenu::open = false;
        *g_refs.Enable = true; *g_refs.EspBox = true;
    }

    // settle animations
    for (int i = 0; i < 60; ++i) Frame(360.0f, 1180.0f);

    // mouse parked away from the ui for the screenshot
    Frame(8.0f, 1264.0f);

    if (getenv("DUMP"))
    {
        ImDrawData* dd = ImGui::GetDrawData();
        printf("=== draw lists: %d\n", dd->CmdListsCount);
        for (int i = 0; i < dd->CmdListsCount; ++i)
        {
            const ImDrawList* cl = dd->CmdLists[i];
            printf("list %d: cmds=%d verts=%d idx=%d\n", i, cl->CmdBuffer.Size, cl->VtxBuffer.Size, cl->IdxBuffer.Size);
            for (int c = 0; c < cl->CmdBuffer.Size; ++c)
            {
                const ImDrawCmd* cmd = &cl->CmdBuffer[c];
                float minx = 1e9f, maxx = -1e9f, miny = 1e9f, maxy = -1e9f;
                for (int e = 0; e < cmd->ElemCount; ++e)
                {
                    const ImDrawVert& v = cl->VtxBuffer[cl->IdxBuffer[cmd->IdxOffset + e]];
                    minx = ImMin(minx, v.pos.x); maxx = ImMax(maxx, v.pos.x);
                    miny = ImMin(miny, v.pos.y); maxy = ImMax(maxy, v.pos.y);
                }
                if (c < 8)
                    printf("   cmd %2d elems=%5d clip=(%.0f,%.0f,%.0f,%.0f) bbox=(%.0f,%.0f)-(%.0f,%.0f)\n",
                           c, cmd->ElemCount, cmd->ClipRect.x, cmd->ClipRect.y, cmd->ClipRect.z, cmd->ClipRect.w,
                           minx, miny, maxx, maxy);
            }
        }
    }
    Canvas cv; cv.init(720, 1280);
    Rasterize(ImGui::GetDrawData(), cv);
    WritePPM(outPath, cv);
    printf("frames=%d scenario=%s verts=%d -> %s\n", g_frames, scenario,
           ImGui::GetDrawData() ? ImGui::GetDrawData()->TotalVtxCount : 0, outPath);
    (void)g_crashed;
    return 0;
}
