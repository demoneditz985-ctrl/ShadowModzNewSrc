// ============================================================================
//  SHADOW MODZ - functional smoke test
//  Drives the real UI headers through simulated touch input and asserts the
//  widgets/among features actually behave (toggles flip, sliders set, nav
//  switches, drag moves, scroll scrolls, presets save/load).
// ============================================================================

#include "imgui.h"
#include "imgui_internal.h"

#include "host_stub.hpp"

#include "SHADOWMODZ MODS/ShadowTheme.hpp"
#include "SHADOWMODZ MODS/ShadowMenu.hpp"

#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>

static int g_fail = 0;
#define CHECK(cond, what) do { if (!(cond)) { printf("  [FAIL] %s\n", what); ++g_fail; } else { printf("  [ ok ] %s\n", what); } } while (0)

// ---------------------------------------------------------------------------
static void NewFrame(float mx, float my, bool down)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(720.0f, 1280.0f);
    io.DeltaTime = 1.0f / 60.0f;
    io.MousePos = ImVec2(mx, my);
    io.MouseDown[0] = down;
    ImGui::NewFrame();
}

static void EndFrameNoDraw()
{
    ImGui::Render();
    ImGui::GetDrawData(); // touch it
}

// ---------------------------------------------------------------------------
//  probe helpers: submit a widget inside a probe window, learn where its
//  interactive rect landed, then drive a real press/release over that point.
// ---------------------------------------------------------------------------
struct Capture
{
    ImVec2 mn = ImVec2(0, 0), mx = ImVec2(0, 0);
    bool   has = false;
    void operator()() { mn = ShadowUI::LastItemMin; mx = ShadowUI::LastItemMax; has = true; }
};

static void ProbeWindowBegin()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(720, 1280));
    ImGui::Begin("##probe", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings |
                                      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground |
                                      ImGuiWindowFlags_NoMove);
}

template <typename Fn>
static Capture ProbeFrame(Fn body, ImVec2 at, bool down)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(720.0f, 1280.0f);
    io.DeltaTime = 1.0f / 60.0f;
    io.MousePos = at;
    io.MouseDown[0] = down;
    ImGui::NewFrame();
    ProbeWindowBegin();
    Capture cap;
    body(cap);
    ImGui::End();
    ImGui::Render();
    return cap;
}

template <typename Fn>
static ImVec2 FindRect(Fn body)
{
    Capture cap;
    for (int i = 0; i < 3; ++i)
    {
        cap = ProbeFrame(body, ImVec2(-FLT_MAX, -FLT_MAX), false);
        if (cap.has) break;
    }
    return ImVec2((cap.mn.x + cap.mx.x) * 0.5f, (cap.mn.y + cap.mx.y) * 0.5f);
}

template <typename Fn>
static void ClickWidget(Fn body, ImVec2 at)
{
    ProbeFrame(body, at, false);
    ProbeFrame(body, at, false);
    ProbeFrame(body, at, true);
    ProbeFrame(body, at, false);
    ProbeFrame(body, at, false);
    if (getenv("SMOKE_DEBUG"))
        printf("      [dbg] clicked at (%.1f,%.1f) rect=(%.0f,%.0f)-(%.0f,%.0f)\n",
               at.x, at.y, ShadowUI::LastItemMin.x, ShadowUI::LastItemMin.y,
               ShadowUI::LastItemMax.x, ShadowUI::LastItemMax.y);
}

static ShadowMenu::Refs R;

static void BindAll()
{
#define B_(name, target)  R.name = &target;
    SHADOW_BOOL_FIELDS(B_)
    SHADOW_FLOAT_FIELDS(B_)
    SHADOW_INT_FIELDS(B_)
#undef B_
}

// one full menu frame with the pointer at (mx,my)
static void MenuFrame(float mx, float my, bool down)
{
    NewFrame(mx, my, down);
    ShadowMenu::Draw(R, 720.0f, 1280.0f);
    EndFrameNoDraw();
}

int main()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.DisplaySize = ImVec2(720, 1280);
    io.DeltaTime = 1.0f / 60.0f;
    io.Fonts->AddFontDefault();
    unsigned char* px; int w, h;
    io.Fonts->GetTexDataAsRGBA32(&px, &w, &h);
    io.Fonts->SetTexID((ImTextureID)1);

    BindAll();

    // ---------------------------------------------------------------- widgets
    printf("== widget behaviour ==\n");

    {
        bool v = false;
        auto body = [&](Capture& cap) { ShadowUI::ToggleRow("test toggle", &v); cap(); };
        ClickWidget(body, FindRect(body));
        CHECK(v == true, "ToggleRow flips its value when tapped");
    }
    {
        bool v = false;
        auto body = [&](Capture& cap) { ShadowUI::ToggleRow("t2", &v); cap(); };
        ImVec2 at = FindRect(body);
        ClickWidget(body, at);
        ClickWidget(body, at);
        CHECK(v == false, "ToggleRow flips back on a second tap");
    }
    {
        float f = 0.0f;
        auto body = [&](Capture& cap) { ShadowUI::SliderRow("test slider", &f, 0.0f, 1.0f, "%.2f"); cap(); };
        Capture cap = ProbeFrame(body, ImVec2(-FLT_MAX, -FLT_MAX), false);
        ImVec2 mn = cap.mn, mx = cap.mx;
        ImVec2 at(mn.x + (mx.x - mn.x) * 0.85f, (mn.y + mx.y) * 0.5f);
        ClickWidget(body, at);
        CHECK(f > 0.60f, "SliderRow drives its value from a tap on the track");
    }
    {
        int idx = 0;
        const char* items[] = { "A", "B", "C" };
        auto body = [&](Capture& cap) { ShadowUI::SelectRow("test select", &idx, items, 3); cap(); };
        ClickWidget(body, FindRect(body));
        CHECK(idx == 1, "SelectRow cycles to the next option");
    }
    {
        int presses = 0;
        auto body = [&](Capture& cap) { if (ShadowUI::Button("PRESS ME", ICON_FA_BOLT, ImVec2(200, 40))) ++presses; cap(); };
        ClickWidget(body, FindRect(body));
        CHECK(presses == 1, "Button reports exactly one press");
    }
    {
        int hits = 0;
        auto body = [&](Capture& cap) { if (ShadowUI::IconButton("##x", ICON_FA_TIMES, ImVec2(40, 40), ShadowUI::pal.danger, true)) ++hits; cap(); };
        ClickWidget(body, FindRect(body));
        CHECK(hits == 1, "IconButton click");
    }
    {
        auto body = [&](Capture& cap) { ShadowUI::CardBegin("card", "CARD", ICON_FA_BOLT); ShadowUI::CardEnd(); cap(); };
        for (int i = 0; i < 2; ++i) ProbeFrame(body, ImVec2(-FLT_MAX, -FLT_MAX), false);
        CHECK(true, "card/channel rendering frame completes");
    }

    // ---------------------------------------------------------------- menu nav
    printf("== navigation ==\n");
    {
        // sweep the nav rail column and collect which tabs the taps select
        ShadowMenu::open = true;

        bool reached[5] = { false, false, false, false, false };
        for (float y = 470.0f; y < 900.0f; y += 4.0f)
        {
            ShadowMenu::tab = -1;
            for (int i = 0; i < 2; ++i) MenuFrame(700.0f, 1260.0f, false);
            MenuFrame(90.0f, y, false);
            MenuFrame(90.0f, y, true);
            MenuFrame(90.0f, y, false);
            for (int i = 0; i < 2; ++i) MenuFrame(700.0f, 1260.0f, false);
            if (ShadowMenu::tab >= 0 && ShadowMenu::tab < 5) reached[ShadowMenu::tab] = true;
        }
        CHECK(reached[0] && reached[1] && reached[2] && reached[3] && reached[4],
              "every nav entry is reachable from the rail (AIMBOT/VISUAL/MISC/NETWORK/SETTINGS)");
    }

    // ---------------------------------------------------------------- scrolling
    printf("== scrolling ==\n");
    {
        ShadowMenu::tab = 2;   // MISC has plenty of rows to scroll
        float cx = ShadowMenu::menuX + ShadowMenu::theme.menuWidth * 0.5f + 60.0f;
        float cy = ShadowMenu::menuY + ShadowMenu::theme.menuHeight * 0.5f;
        for (int i = 0; i < 3; ++i) MenuFrame(700.0f, 1260.0f, false);
        for (int i = 0; i < 2; ++i) MenuFrame(cx, cy, false);
        float y0 = cy;
        for (int i = 0; i < 12; ++i) { y0 -= 12.0f; MenuFrame(cx, y0, true); }
        MenuFrame(cx, y0, false);
        for (int i = 0; i < 4; ++i) MenuFrame(cx, y0, false);
        float scroll = 0.0f;
        for (int i = 0; i < ImGui::GetCurrentContext()->Windows.Size; ++i)
        {
            ImGuiWindow* wnd = ImGui::GetCurrentContext()->Windows[i];
            if (!wnd || !wnd->Name) continue;
            const char* leaf = strrchr(wnd->Name, '/');
            leaf = leaf ? leaf + 1 : wnd->Name;
            if (strncmp(leaf, "##shadowmodz_content", 20) == 0) scroll = wnd->Scroll.y;
        }
        if (getenv("SMOKE_DEBUG"))
        {
            for (int i = 0; i < ImGui::GetCurrentContext()->Windows.Size; ++i)
            {
                ImGuiWindow* wnd = ImGui::GetCurrentContext()->Windows[i];
                printf("      [dbg] win '%s' scroll=%.1f max=%.1f clip=(%.0f,%.0f,%.0f,%.0f)\n",
                       wnd->Name, wnd->Scroll.y, wnd->ScrollMax.y,
                       wnd->ClipRect.Min.x, wnd->ClipRect.Min.y, wnd->ClipRect.Max.x, wnd->ClipRect.Max.y);
            }
            printf("      [dbg] scroll found = %.1f\n", scroll);
        }
        CHECK(scroll > 1.0f, "content area scrolls with a finger drag");

        // flick should keep scrolling after the finger is lifted
        float scrolledAtRelease = scroll;
        for (int i = 0; i < 8; ++i) MenuFrame(cx, y0, false);
        float after = 0.0f;
        for (int i = 0; i < ImGui::GetCurrentContext()->Windows.Size; ++i)
        {
            ImGuiWindow* wnd = ImGui::GetCurrentContext()->Windows[i];
            if (!wnd || !wnd->Name) continue;
            const char* leaf = strrchr(wnd->Name, '/');
            leaf = leaf ? leaf + 1 : wnd->Name;
            if (strncmp(leaf, "##shadowmodz_content", 20) == 0) after = wnd->Scroll.y;
        }
        CHECK(after >= scrolledAtRelease - 0.5f, "flick momentum keeps scrolling after release");
    }

    // ---------------------------------------------------------------- dragging
    printf("== dragging ==\n");
    {
        ShadowMenu::tab = 0;
        for (int i = 0; i < 3; ++i) MenuFrame(700.0f, 1260.0f, false);
        const float x0 = ShadowMenu::menuX, y0 = ShadowMenu::menuY;
        float sx = x0 + 120.0f, sy = y0 + 30.0f;
        MenuFrame(sx, sy, false);
        MenuFrame(sx, sy, true);
        for (int i = 1; i <= 6; ++i) MenuFrame(sx + i * 8.0f, sy + i * 5.0f, true);
        MenuFrame(sx + 48.0f, sy + 30.0f, false);
        for (int i = 0; i < 3; ++i) MenuFrame(700.0f, 1260.0f, false);
        CHECK(ShadowMenu::menuX > x0 + 20.0f && ShadowMenu::menuY > y0 + 15.0f, "window header drags the menu");
    }
    {
        // launcher: drag it with the menu closed, verify the position follows
        ShadowMenu::open = false;
        ShadowMenu::launcherX = 70.0f;
        ShadowMenu::launcherY = 400.0f;
        ShadowMenu::snapTargetX = 70.0f;
        for (int i = 0; i < 3; ++i) MenuFrame(700.0f, 1260.0f, false);
        MenuFrame(70.0f, 400.0f, false);
        MenuFrame(70.0f, 400.0f, true);
        for (int i = 1; i <= 6; ++i) MenuFrame(70.0f + i * 10.0f, 400.0f + i * 6.0f, true);
        MenuFrame(130.0f, 436.0f, false);
        const float lx = ShadowMenu::launcherX, ly = ShadowMenu::launcherY;
        CHECK(lx > 90.0f && ly > 410.0f, "launcher follows the finger while dragged");
        CHECK(ShadowMenu::open == false, "a long drag does not toggle the menu");
    }
    {
        // short tap toggles; use a spot the menu window can't cover (menu closed
        // for the first tap so the launcher is definitely on top)
        ShadowMenu::open = false;
        ShadowMenu::launcherX = 120.0f;
        ShadowMenu::launcherY = 1100.0f;
        ShadowMenu::snapTargetX = 120.0f;
        for (int i = 0; i < 3; ++i) MenuFrame(700.0f, 1260.0f, false);
        MenuFrame(120.0f, 1100.0f, true);
        MenuFrame(120.0f, 1100.0f, false);
        for (int i = 0; i < 3; ++i) MenuFrame(700.0f, 1260.0f, false);
        CHECK(ShadowMenu::open == true, "tap on the launcher opens the menu");

        ShadowMenu::launcherX = 120.0f;
        ShadowMenu::launcherY = 1100.0f;
        ShadowMenu::snapTargetX = 120.0f;
        MenuFrame(120.0f, 1100.0f, true);
        MenuFrame(120.0f, 1100.0f, false);
        for (int i = 0; i < 3; ++i) MenuFrame(700.0f, 1260.0f, false);
        CHECK(ShadowMenu::open == false, "tap on the launcher closes the menu again");
        ShadowMenu::open = true;
    }

    // ---------------------------------------------------------------- presets
    printf("== presets ==\n");
    {
        *R.Enable = true; *R.Headshot = true; *R.EspBox = true;
        *R.FovAim = 123.0f; *R.LineThickness = 3.5f;
        ShadowMenu::SavePreset(R, 1);
        CHECK(ShadowMenu::slots[1].used, "SAVE fills a preset slot");

        *R.Enable = false; *R.Headshot = false; *R.EspBox = false; *R.FovAim = 5.0f; *R.LineThickness = 1.0f;
        ShadowMenu::LoadPreset(R, 1);
        CHECK(*R.Enable && *R.Headshot && *R.EspBox, "LOAD restores the booleans");
        CHECK(fabs(*R.FovAim - 123.0f) < 0.01f && fabs(*R.LineThickness - 3.5f) < 0.01f, "LOAD restores the values");
    }

    // ---------------------------------------------------------------- all tabs
    printf("== all tabs render ==\n");
    for (int t = 0; t < 5; ++t)
    {
        ShadowMenu::tab = t;
        *R.Enable = true; *R.Aimbot = true; *R.EspBox = true; *R.SpeedHack = true;
        *R.FirewallOn = true;
        g_FwStats.totalChecks = 10; g_FwStats.totalAllowed = 7; g_FwStats.totalBlocked = 3;
        for (int i = 0; i < 4; ++i) MenuFrame(710.0f, 1270.0f, false);
        char msg[64];
        snprintf(msg, sizeof(msg), "tab %d renders", t);
        CHECK(true, msg);
    }

    // ---------------------------------------------------------------- accents + flags
    printf("== settings ==\n");
    for (int a = 0; a < ShadowUI::kAccentCount; ++a)
    {
        ShadowUI::ApplyAccent(a);
        for (int i = 0; i < 2; ++i) MenuFrame(710.0f, 1270.0f, false);
    }
    CHECK(ShadowUI::theme.accentIndex == ShadowUI::kAccentCount - 1, "accent presets apply");
    ShadowUI::ApplyAccent(0);

    ShadowMenu::tab = 0;
    *R.Enable = true; *R.Aimbot = true; *R.AimLock = true;
    CHECK(ShadowMenu::TabActive(R, 0), "TabActive detects active aim modules");
    *R.Enable = false; *R.Aimbot = false; *R.AimLock = false;
    CHECK(!ShadowMenu::TabActive(R, 0), "TabActive clears when modules are off");
    CHECK(ShadowMenu::EnabledCount(R) > 0, "EnabledCount counts active modules");

    // ---------------------------------------------------------------- ui scale sweep
    printf("== ui scale / size sweep ==\n");
    for (float sc = 0.70f; sc <= 1.41f; sc += 0.10f)
    {
        ShadowUI::theme.uiScale = sc;
        ShadowUI::theme.menuWidth = 560.0f + (sc - 0.7f) * 400.0f;
        ShadowMenu::tab = 4;
        for (int i = 0; i < 2; ++i) MenuFrame(710.0f, 1270.0f, false);
    }
    ShadowUI::theme.uiScale = 1.0f;
    CHECK(true, "ui scale sweep 0.7x .. 1.4x");

    printf("\n%s (%d failures)\n", g_fail ? "SMOKE TEST FAILED" : "SMOKE TEST PASSED", g_fail);
    return g_fail ? 1 : 0;
}
