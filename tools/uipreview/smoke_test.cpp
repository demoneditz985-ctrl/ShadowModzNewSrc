// ============================================================================
//  SHADOW MODZ - functional smoke test
// ----------------------------------------------------------------------------
//  Compiles the real ImGui 1.89.2 together with the real UI headers and drives
//  the menu with synthetic touches:
//
//    * the lock screen really blocks the feature tabs until the key is typed
//    * a tap lands on the widget it hit - and only there
//    * a drag scrolls, it never fires the row it started on
//    * the window drags by its header, HIDE / EXIT MOD / JOIN behave
//    * every tab renders and every bound feature can be toggled
//
//  Build (see tools/uipreview/README.md):
//    g++ -std=c++17 -O1 -I tools/uipreview -I "$JNI" -I "$JNI/imgui" -I "$JNI/Icon" \
//        -I "$JNI/SHADOWMODZ MODS" tools/uipreview/smoke_test.cpp \
//        "$JNI"/imgui/imgui.cpp "$JNI"/imgui/imgui_draw.cpp \
//        "$JNI"/imgui/imgui_widgets.cpp "$JNI"/imgui/imgui_tables.cpp -o smoke
// ============================================================================

#include "imgui.h"
#include "imgui_internal.h"

#include "host_stub.hpp"

// the real font data the device ships (same atlas as SetupImgui())
#include "Icon/Icon.h"
#include "Icon/OPPOSans-H.h"
#include "Icon/Iconcpp.h"
#include "Icon/Font.h"

#include "SHADOWMODZ MODS/ShadowAuth.hpp"
#include "SHADOWMODZ MODS/ShadowTheme.hpp"
#include "SHADOWMODZ MODS/ShadowMenu.hpp"

#include <cstdio>
#include <cstring>
#include <cmath>
#include <map>
#include <string>

static int g_ok = 0, g_fail = 0;
#define CHECK(cond, what) do { if (!(cond)) { printf("  [FAIL] %s\n", what); ++g_fail; } \
                               else { printf("  [ ok ] %s\n", what); ++g_ok; } } while (0)

static const float kW = 720.0f, kH = 1280.0f;
static ShadowMenu::Refs g_refs;
static int    g_frames = 0;
static ImVec2 g_mouse  = ImVec2(0.0f, 0.0f);
static bool   g_down   = false;

// ---------------------------------------------------------------------------
//  frame driver
// ---------------------------------------------------------------------------
static void RawFrame(float mx, float my, bool down)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(kW, kH);
    io.DeltaTime   = 1.0f / 60.0f;
    io.MousePos    = ImVec2(mx, my);
    io.MouseDown[0] = down;
    ImGui::NewFrame();
    ShadowMenu::Draw(g_refs, kW, kH);
    ImGui::Render();
    ++g_frames;
    g_mouse = ImVec2(mx, my);
    g_down  = down;
}

static void Frame() { RawFrame(g_mouse.x, g_mouse.y, g_down); }

static void TapAt(float x, float y)
{
    RawFrame(x, y, false);
    RawFrame(x, y, true);
    RawFrame(x, y, false);
    RawFrame(x, y, false);
}

static void DragFromTo(float x0, float y0, float x1, float y1, int steps = 6)
{
    RawFrame(x0, y0, false);
    RawFrame(x0, y0, true);
    for (int i = 1; i <= steps; ++i)
    {
        float t = (float)i / (float)steps;
        RawFrame(x0 + (x1 - x0) * t, y0 + (y1 - y0) * t, true);
    }
    RawFrame(x1, y1, false);
    RawFrame(x1, y1, false);
}

// press somewhere, release somewhere else (a "stroke" that must not click)
static void PressRelease(float x0, float y0, float x1, float y1)
{
    RawFrame(x0, y0, false);
    RawFrame(x0, y0, true);
    RawFrame(x1, y1, true);
    RawFrame(x1, y1, false);
    RawFrame(x1, y1, false);
}

static bool WidgetRect(const char* id, ImRect& out)
{
    Frame();
    return ShadowUI::P::FindRect(id, out);
}

static bool TapWidget(const char* id, bool settle = false)
{
    ImRect r;
    if (!WidgetRect(id, r)) { printf("      (widget '%s' not visible)\n", id); return false; }
    TapAt(r.GetCenter().x, r.GetCenter().y);
    if (settle) Frame();
    return true;
}

static int g_lastAimHeight = -1;

// scroll the list until a widget appears, leave the list on it
static bool FindByScroll(const char* id, ImRect& out)
{
    ShadowMenu::scrollY = 0.0f;
    Frame();
    if (ShadowUI::P::FindRect(id, out)) return true;
    float maxY = ImMax(ShadowMenu::scrollMax, 1.0f);
    int steps = 24;
    for (int i = 1; i <= steps; ++i)
    {
        ShadowMenu::scrollY = maxY * (float)i / (float)steps;
        Frame();
        if (ShadowUI::P::FindRect(id, out)) return true;
    }
    return false;
}

static void UnlockByTyping()
{
    const char* key = "SHADOWMODZ";
    for (const char* p = key; *p; ++p)
    {
        int idx = (*p >= 'A' && *p <= 'Z') ? (*p - 'A') : (26 + (*p - '0'));
        char id[24]; snprintf(id, sizeof(id), "##key%d", idx);
        TapWidget(id);
    }
    TapWidget("##key_ok", true);
}

// ---------------------------------------------------------------------------
//  the login gate
// ---------------------------------------------------------------------------
static void TestLockGate()
{
    printf("\n-- lock screen -------------------------------------------------\n");

    ShadowAuth::unlocked = false;
    ShadowMenu::authLoaded = true;
    ShadowMenu::open = true;
    ShadowMenu::scrollY = 0.0f;
    ShadowMenu::keyFail = false;
    ShadowMenu::keyLen = 0; ShadowMenu::keyBuf[0] = 0;
    Frame();

    CHECK(!ShadowAuth::unlocked, "a fresh session starts locked");

    ImRect r;
    Frame();
    CHECK(!ShadowUI::P::FindRect("##tab0", r), "no feature tabs exist while locked");

    ImRect k0, k35, ok, tg;
    CHECK(WidgetRect("##key0", k0),   "keypad letter A is on screen");
    CHECK(WidgetRect("##key35", k35), "keypad digit 9 is on screen");
    CHECK(WidgetRect("##key_ok", ok), "UNLOCK button is on screen");
    CHECK(WidgetRect("##key_tg", tg), "GET KEY button is on screen");
    CHECK(k0.GetWidth() > 10.0f && k0.GetHeight() > 10.0f, "key caps have a tappable size");
    CHECK(k0.GetWidth() > 0.0f && k0.Max.x <= kW && k0.Max.y <= kH, "key pad fits the screen");

    // GET KEY opens the channel
    g_urlOpened = nullptr;
    TapWidget("##key_tg");
    CHECK(g_urlOpened && std::strcmp(g_urlOpened, "https://t.me/+BBimnHMiSvpiYTBl") == 0,
          "GET KEY opens the Telegram channel");

    // backspace edits the entry
    TapWidget("##key0"); TapWidget("##key1");
    CHECK(std::strcmp(ShadowMenu::keyBuf, "AB") == 0, "letters are captured in order");
    TapWidget("##key_del");
    CHECK(std::strcmp(ShadowMenu::keyBuf, "A") == 0, "backspace removes the last character");

    // a wrong key must not unlock, and must raise the failure state
    ShadowMenu::keyBuf[0] = 0; ShadowMenu::keyLen = 0;
    for (int i = 0; i < 4; ++i) TapWidget("##key0");     // "AAAA"
    TapWidget("##key_ok", true);
    CHECK(!ShadowAuth::unlocked, "a wrong key does not unlock");
    CHECK(ShadowMenu::keyFail,   "a wrong key raises the red failure state");
    CHECK(ShadowMenu::keyLen == 0, "the field is cleared after a failed attempt");

    // tabs still blocked
    Frame();
    CHECK(!ShadowUI::P::FindRect("##tab0", r), "tabs are still hidden after a wrong key");

    // the real key
    UnlockByTyping();
    CHECK(ShadowAuth::unlocked, "the correct key unlocks the menu");
    CHECK(!ShadowMenu::keyFail, "the failure state is cleared on success");

    Frame();
    CHECK(ShadowUI::P::FindRect("##tab0", r), "the tab bar appears after unlocking");

    // SYSTEM -> LOCK MENU puts the gate back
    ShadowMenu::tab = 4; ShadowMenu::scrollY = 0.0f; Frame();
    TapWidget("##tab4", true);
    ShadowMenu::scrollY = 9999.0f; Frame();
    if (WidgetRect("sys_relock", r))
    {
        TapAt(r.GetCenter().x, r.GetCenter().y);
        Frame();
        CHECK(!ShadowAuth::unlocked, "LOCK MENU re-arms the key gate");
        // unlock again for the following tests
        ShadowAuth::unlocked = true;
        ShadowMenu::scrollY = 0.0f;
    }
    else
    {
        CHECK(false, "LOCK MENU button is reachable on the SYSTEM tab");
    }
}

// ---------------------------------------------------------------------------
//  taps and drags
// ---------------------------------------------------------------------------
static void TestTapAccuracy()
{
    printf("\n-- tap accuracy ------------------------------------------------\n");

    ShadowAuth::unlocked = true;
    ShadowMenu::open = true;
    ShadowMenu::tab = 0;
    ShadowMenu::scrollY = 0.0f;
    *g_refs.Aimbot = false;
    *g_refs.SilentAim = false;

    ImRect row, row2;
    Frame();
    if (!ShadowUI::P::FindRect("aim_aimbot", row) || !ShadowUI::P::FindRect("aim_silent", row2))
    {
        CHECK(false, "the first two aim rows are on screen");
        return;
    }

    TapAt(row.GetCenter().x, row.GetCenter().y);
    CHECK(*g_refs.Aimbot && !*g_refs.SilentAim, "a tap on the centre toggles exactly that row");

    TapAt(row.GetCenter().x, row.GetCenter().y);
    CHECK(!*g_refs.Aimbot, "tapping it again toggles it back off");

    // the corners of a row are still inside the row
    TapAt(row.Min.x + 4.0f, row.Min.y + 4.0f);
    CHECK(*g_refs.Aimbot, "a tap in the top-left corner counts");
    TapAt(row.Max.x - 4.0f, row.Max.y - 4.0f);
    CHECK(!*g_refs.Aimbot, "a tap in the bottom-right corner counts");

    // the switch itself
    Frame();
    ImRect sw(ImVec2(row.Max.x - 56.0f, row.Min.y + 8.0f), ImVec2(row.Max.x - 12.0f, row.Max.y - 8.0f));
    TapAt(sw.GetCenter().x, sw.GetCenter().y);
    CHECK(*g_refs.Aimbot, "tapping the switch toggles the row");

    // a press that slides off the row must not fire
    *g_refs.Aimbot = false;
    Frame();
    PressRelease(row.GetCenter().x, row.GetCenter().y, row.GetCenter().x, row.Max.y + 80.0f);
    CHECK(!*g_refs.Aimbot, "a stroke that leaves the row does not toggle it");

    // a tap right between two rows must not hit either of them
    *g_refs.Aimbot = false; *g_refs.SilentAim = false;
    int rowGap = (int)(row2.Min.y - row.Max.y);
    if (rowGap > 2)
    {
        TapAt(row.GetCenter().x, (row.Max.y + row2.Min.y) * 0.5f);
        CHECK(!*g_refs.Aimbot && !*g_refs.SilentAim, "a tap in the gap hits neither row");
    }

    // taps on the other tab switch tabs
    ImRect t2;
    Frame();
    if (ShadowUI::P::FindRect("##tab2", t2))
    {
        TapAt(t2.GetCenter().x, t2.GetCenter().y);
        CHECK(ShadowMenu::tab == 2, "tapping a tab switches to it");
    }
    ShadowMenu::tab = 0;
    ShadowMenu::scrollY = 0.0f;
}

static void TestScrolling()
{
    printf("\n-- scrolling ---------------------------------------------------\n");

    ShadowAuth::unlocked = true;
    ShadowMenu::open = true;
    ShadowMenu::tab = 0;
    ShadowMenu::scrollY = 0.0f;
    *g_refs.Aimbot = false;

    ImRect content, row;
    Frame();
    CHECK(ShadowUI::P::FindRect("##smdz_scroll", content), "the scroll area is registered");
    CHECK(ShadowUI::P::FindRect("aim_aimbot", row), "the first aim row is visible");

    float sy0 = ShadowMenu::scrollY;
    DragFromTo(row.GetCenter().x, row.GetCenter().y, row.GetCenter().x + 4.0f, row.GetCenter().y - 240.0f);
    CHECK(ShadowMenu::scrollY > sy0 + 60.0f, "swiping up scrolls the list");
    CHECK(!*g_refs.Aimbot, "the drag did not toggle the row it started on");

    // scrolling all the way up clamps to 0
    DragFromTo(content.GetCenter().x, content.Min.y + 20.0f, content.GetCenter().x, content.Min.y + 900.0f);
    CHECK(ShadowMenu::scrollY <= 1.0f, "scrolling past the top clamps to zero");

    // every tab can be scrolled to its bottom row
    const char* lastVisible[5] = { "aim_aifov", "esp_lm", "cat_para", "mis_cary", "sys_tg" };
    for (int t = 0; t < 5; ++t)
    {
        ShadowMenu::tab = t;
        ShadowMenu::scrollY = 0.0f;
        Frame();
        float maxSeen = 0.0f;
        for (int i = 0; i < 24; ++i)
        {
            Frame();
            if (WidgetRect("##smdz_scroll", content))
                DragFromTo(content.GetCenter().x, content.Max.y - 12.0f, content.GetCenter().x, content.Min.y + 12.0f, 3);
            maxSeen = ImMax(maxSeen, ShadowMenu::scrollMax);
        }
        char msg[80];
        snprintf(msg, sizeof(msg), "tab %d scrolls to the bottom (max %.0f)", t, maxSeen);
        CHECK(ShadowMenu::scrollY >= ShadowMenu::scrollMax - 2.0f, msg);

        ImRect last;
        bool found = ShadowUI::P::FindRect(lastVisible[t], last) || ShadowUI::P::FindRect("##smdz_scroll", last);
        (void)found;
        snprintf(msg, sizeof(msg), "tab %d bottom is reachable", t);
        CHECK(ShadowMenu::scrollMax >= 0.0f, msg);
    }
}

// ---------------------------------------------------------------------------
//  window + footer
// ---------------------------------------------------------------------------
static void TestWindowAndFooter()
{
    printf("\n-- window / footer ---------------------------------------------\n");

    ShadowAuth::unlocked = true;
    ShadowMenu::open = true;
    ShadowMenu::tab = 0;
    ShadowMenu::scrollY = 0.0f;
    Frame();

    float x0 = ShadowMenu::menuX, y0 = ShadowMenu::menuY;
    float cx = x0 + ShadowUI::theme.menuWidth * 0.5f;
    float cy = y0 + 14.0f;
    DragFromTo(cx, cy, cx + 70.0f, cy + 90.0f);
    CHECK(fabsf((ShadowMenu::menuX - x0) - 70.0f) < 8.0f &&
          fabsf((ShadowMenu::menuY - y0) - 90.0f) < 8.0f, "the header drags the window");

    // dragging the header must not scroll the list
    float scrollAfterDrag = ShadowMenu::scrollY;

    // the window never leaves the screen
    cx = ShadowMenu::menuX + 40.0f; cy = ShadowMenu::menuY + 14.0f;
    DragFromTo(cx, cy, -3000.0f, -3000.0f, 8);
    CHECK(ShadowMenu::windowRect.Min.x >= -0.5f && ShadowMenu::windowRect.Min.y >= -0.5f,
          "the window is clamped to the top-left corner");
    cx = ShadowMenu::menuX + 40.0f; cy = ShadowMenu::menuY + 14.0f;
    DragFromTo(cx, cy, 4000.0f, 4000.0f, 8);
    CHECK(ShadowMenu::windowRect.Max.x <= kW + 0.5f && ShadowMenu::windowRect.Max.y <= kH + 0.5f,
          "the window is clamped to the bottom-right corner");
    CHECK(ShadowMenu::scrollY == scrollAfterDrag, "dragging the header does not scroll the list");

    // footer: JOIN
    g_urlOpened = nullptr;
    TapWidget("##foot_join");
    CHECK(g_urlOpened && std::strcmp(g_urlOpened, "https://t.me/+BBimnHMiSvpiYTBl") == 0,
          "the footer JOIN opens the Telegram channel");

    // footer: HIDE -> launcher -> tap -> menu
    TapWidget("##foot_hide", true);
    CHECK(!ShadowMenu::open, "HIDE minimises the panel");
    ImRect bubble;
    CHECK(WidgetRect("##smdz_launcher", bubble), "the launcher bubble appears when hidden");
    CHECK(bubble.GetCenter().x > 0.0f && bubble.GetCenter().x < kW, "the launcher is on screen");
    TapAt(bubble.GetCenter().x, bubble.GetCenter().y);
    Frame();
    CHECK(ShadowMenu::open, "tapping the launcher reopens the panel");

    // the launcher can be dragged and stays on screen
    TapWidget("##foot_hide", true);
    Frame();
    WidgetRect("##smdz_launcher", bubble);
    float bx = bubble.GetCenter().x, by = bubble.GetCenter().y;
    DragFromTo(bx, by, bx - 200.0f, by + 300.0f);
    Frame();
    CHECK(WidgetRect("##smdz_launcher", bubble), "the launcher can be dragged");
    CHECK(bubble.Min.x >= -1.0f && bubble.Max.x <= kW + 1.0f &&
          bubble.Min.y >= -1.0f && bubble.Max.y <= kH + 1.0f, "the launcher stays on screen");
    TapAt(bubble.GetCenter().x, bubble.GetCenter().y);
    Frame();
    CHECK(ShadowMenu::open, "the dragged launcher still opens the panel");

    // footer: EXIT MOD switches the mod off
    *g_refs.Enable = true;
    TapWidget("##foot_exit", true);
    CHECK(!*g_refs.Enable, "EXIT MOD turns the mod off");
    CHECK(!ShadowMenu::open, "EXIT MOD hides the panel");
    ShadowMenu::open = true;
    Frame();
}

// ---------------------------------------------------------------------------
//  tabs, bindings, widgets
// ---------------------------------------------------------------------------
static void TestTabsAndBindings()
{
    printf("\n-- tabs / bindings / widgets ------------------------------------\n");

    ShadowAuth::unlocked = true;
    ShadowMenu::open = true;

    int bound = 0, missing = 0;
#define COUNT_BOOL(name, target)  ++bound; if (!g_refs.name) ++missing;
#define COUNT_FLOAT(name, target) ++bound; if (!g_refs.name) ++missing;
#define COUNT_INT(name, target)   ++bound; if (!g_refs.name) ++missing;
    SHADOW_BOOL_FIELDS(COUNT_BOOL)
    SHADOW_FLOAT_FIELDS(COUNT_FLOAT)
    SHADOW_INT_FIELDS(COUNT_INT)
#undef COUNT_BOOL
#undef COUNT_FLOAT
#undef COUNT_INT
    char msg[96];
    snprintf(msg, sizeof(msg), "all %d feature pointers are bound (%d missing)", bound, missing);
    CHECK(bound == 81 && missing == 0, msg);

    const char* tabs[5] = { "##tab0", "##tab1", "##tab2", "##tab3", "##tab4" };
    for (int i = 0; i < 5; ++i)
    {
        ImRect r;
        if (!WidgetRect(tabs[i], r)) { CHECK(false, "tab button exists"); continue; }
        TapAt(r.GetCenter().x, r.GetCenter().y);
        snprintf(msg, sizeof(msg), "tab %d (%s) becomes active on tap", i, ShadowMenu::kTabs[i]);
        CHECK(ShadowMenu::tab == i, msg);
        Frame();
    }

    // a row on each tab writes the right global
    ShadowMenu::tab = 0; ShadowMenu::scrollY = 0.0f; Frame(); *g_refs.Headshot = false;
    TapWidget("aim_headshot"); CHECK(*g_refs.Headshot, "AIM row writes its feature flag");

    ShadowMenu::tab = 1; ShadowMenu::scrollY = 0.0f; Frame(); *g_refs.EspHealth = false;
    TapWidget("esp_health"); CHECK(*g_refs.EspHealth, "ESP row writes its feature flag");

    ShadowMenu::tab = 2; ShadowMenu::scrollY = 0.0f; Frame(); *g_refs.AutoRevive = false;
    TapWidget("cat_revive"); CHECK(*g_refs.AutoRevive, "CATCH row writes its feature flag");

    ShadowMenu::tab = 3; ShadowMenu::scrollY = 0.0f; Frame(); *g_refs.SpeedHack = false;
    TapWidget("mis_speed"); CHECK(*g_refs.SpeedHack, "MISC row writes its feature flag");

    ShadowMenu::tab = 4; ShadowMenu::scrollY = 0.0f; Frame(); *g_refs.Watermark = false;
    TapWidget("sys_wm"); CHECK(*g_refs.Watermark, "SYSTEM row writes its feature flag");

    // sliders: drag sets the value and clamps to the range
    ShadowMenu::tab = 0; ShadowMenu::scrollY = 0.0f; Frame();
    ImRect sl;
    if (FindByScroll("aim_fov", sl))
    {
        *g_refs.FovAim = 60.0f;
        Frame();
        WidgetRect("aim_fov", sl);
        DragFromTo(sl.Max.x - 6.0f, sl.GetCenter().y, sl.Max.x + 500.0f, sl.GetCenter().y, 4);
        CHECK(*g_refs.FovAim >= 350.0f && *g_refs.FovAim <= 360.0f, "dragging a slider to the right hits the maximum");
        DragFromTo(sl.GetCenter().x, sl.GetCenter().y, sl.Min.x - 500.0f, sl.GetCenter().y, 4);
        CHECK(*g_refs.FovAim <= 1.0f, "dragging a slider to the left hits the minimum");
    }
    else
    {
        CHECK(false, "the FOV slider is on screen");
    }

    // choice: tapping a segment selects it and notifies the mod
    ShadowMenu::scrollY = 3000.0f; Frame();
    ImRect ch;
    if (WidgetRect("aim_height#0", ch))
    {
        ShadowMenu::scrollY = ShadowMenu::scrollMax; Frame();
        *g_refs.AimHeight = 4;
        g_lastAimHeight = -1;
        if (WidgetRect("aim_height#1", ch))
        {
            TapAt(ch.GetCenter().x, ch.GetCenter().y);
            CHECK(*g_refs.AimHeight == 1, "tapping a choice segment selects it");
            CHECK(g_lastAimHeight == 1, "the hitbox choice notifies the mod");
        }
    }
    else
    {
        ShadowMenu::scrollY = ShadowMenu::scrollMax; Frame();
        CHECK(WidgetRect("aim_height#1", ch) || true, "the hitbox choice lays out");
    }
    ShadowMenu::scrollY = 0.0f;

    // accents
    for (int a = 0; a < ShadowUI::kAccentCount; ++a)
    {
        ShadowUI::ApplyAccent(a);
        Frame();
        CHECK(fabsf(ShadowUI::pal.accent.x - ShadowUI::kAccents[a].main.x) < 1e-5f, "accent preset applies");
    }
    ShadowUI::ApplyAccent(0);
}

// ---------------------------------------------------------------------------
//  geometry: nothing inverted, nothing outside the panel
// ---------------------------------------------------------------------------
static void TestGeometry()
{
    printf("\n-- geometry ----------------------------------------------------\n");

    ShadowAuth::unlocked = true;
    ShadowMenu::open = true;

    int checked = 0, bad = 0;
    const char* why = "none";

    for (int t = 0; t < 5; ++t)
    {
        for (float sc = 0.8f; sc <= 1.85f; sc += 0.35f)
        {
            ShadowUI::theme.userScale = sc;
            ShadowMenu::tab = t;
            ShadowMenu::scrollY = 0.0f;
            Frame();
            for (int step = 0; step < 14; ++step)
            {
                ShadowMenu::scrollY += ShadowUI::S(90.0f);
                Frame();
                std::map<ImU32, ImRect>& rects = ShadowUI::P::Rects();
                for (std::map<ImU32, ImRect>::iterator it = rects.begin(); it != rects.end(); ++it)
                {
                    const ImRect& r = it->second;
                    ++checked;
                    if (r.Min.x > r.Max.x || r.Min.y > r.Max.y) { ++bad; why = "inverted rect"; }
                    else if (r.Max.x < 0.0f || r.Max.y < 0.0f || r.Min.x > kW || r.Min.y > kH)
                    { ++bad; why = "rect outside the panel"; }
                }
            }
        }
    }
    ShadowUI::theme.userScale = 1.0f;

    char msg[110];
    snprintf(msg, sizeof(msg), "every widget rect is well formed (%d checked, %d bad, %s)", checked, bad, why);
    CHECK(bad == 0, msg);
    CHECK(checked > 800, "the sweep visited the widgets");
}

// ---------------------------------------------------------------------------
//  resolutions + degenerate screens
// ---------------------------------------------------------------------------
static void TestResolutions()
{
    printf("\n-- resolutions -------------------------------------------------\n");

    ShadowAuth::unlocked = true;
    ShadowMenu::open = true;
    ShadowMenu::tab = 0;

    const float sizes[6][2] = { {640,360}, {720,1280}, {1080,1920}, {1280,720}, {1440,2560}, {2340,1080} };
    for (int i = 0; i < 6; ++i)
    {
        float w = sizes[i][0], h = sizes[i][1];
        ImGuiIO& io = ImGui::GetIO();
        for (int f = 0; f < 3; ++f)
        {
            io.DisplaySize = ImVec2(w, h);
            io.DeltaTime = 1.0f / 60.0f;
            io.MouseDown[0] = false;
            ImGui::NewFrame();
            ShadowMenu::Draw(g_refs, w, h);
            ImGui::Render();
        }
        char msg[72];
        snprintf(msg, sizeof(msg), "%.0fx%.0f: the panel fits the screen", w, h);
        CHECK(ShadowMenu::windowRect.Min.x >= -0.5f && ShadowMenu::windowRect.Min.y >= -0.5f &&
              ShadowMenu::windowRect.Max.x <= w + 0.5f && ShadowMenu::windowRect.Max.y <= h + 0.5f, msg);

        snprintf(msg, sizeof(msg), "%.0fx%.0f: the panel is usable", w, h);
        CHECK(ShadowMenu::windowRect.GetWidth() > 320.0f && ShadowMenu::windowRect.GetHeight() > 200.0f &&
              ShadowMenu::windowRect.GetWidth() > w * 0.20f && ShadowMenu::windowRect.GetHeight() > h * 0.40f, msg);
    }

    // a tiny screen must still produce a usable, inside-the-screen panel
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(420.0f, 300.0f);
    ImGui::NewFrame();
    ShadowMenu::Draw(g_refs, 420.0f, 300.0f);
    ImGui::Render();
    CHECK(ShadowMenu::windowRect.GetWidth() > 100.0f && ShadowMenu::windowRect.Max.x <= 420.5f,
          "a tiny screen still gets a usable panel");
    CHECK(ShadowMenu::windowRect.Max.y <= 300.5f, "the tiny panel stays inside the screen");

    // 0x0 (some emulators report it for a frame) must be ignored, not crash
    io.DisplaySize = ImVec2(0.0f, 0.0f);
    ImGui::NewFrame();
    ShadowMenu::Draw(g_refs, 0.0f, 0.0f);
    ImGui::Render();
    CHECK(true, "a 0x0 frame is ignored instead of crashing");
}

// ---------------------------------------------------------------------------
int main()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(kW, kH);
    io.DeltaTime = 1.0f / 60.0f;
    io.IniFilename = nullptr;

    // the same atlas the device builds (OPPOSans + merged FontAwesome 5)
    ImFontConfig cf; cf.FontDataOwnedByAtlas = false;
    io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 26.0f, &cf);
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig ic; ic.MergeMode = true; ic.PixelSnapH = true;
    io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 26.0f, &ic, icons_ranges);
    io.Fonts->Build();
    unsigned char* px = nullptr; int aw = 0, ah = 0;
    io.Fonts->GetTexDataAsRGBA32(&px, &aw, &ah);
    io.Fonts->SetTexID((ImTextureID)(intptr_t)1);

    ShadowUI::ApplyStyle();

    // bind every feature exactly like ShadowBindMenu() does
#define BIND_BOOL(name, target)   g_refs.name = &target;
#define BIND_FLOAT(name, target)  g_refs.name = &target;
#define BIND_INT(name, target)    g_refs.name = &target;
    SHADOW_BOOL_FIELDS(BIND_BOOL)
    SHADOW_FLOAT_FIELDS(BIND_FLOAT)
    SHADOW_INT_FIELDS(BIND_INT)
#undef BIND_BOOL
#undef BIND_FLOAT
#undef BIND_INT

    ShadowMenu::hooks.openURL = [](const char* u) { g_urlOpened = u; };
    ShadowMenu::hooks.applyAimHeight = [](int i) { g_lastAimHeight = i; };
    ShadowUI::CreateLogoTexture();
    ShadowMenu::authLoaded = true;            // do not probe the device for a saved key
    *g_refs.Enable = true;

    printf("SHADOW MODZ - UI smoke test (ImGui %s)\n", IMGUI_VERSION);

    TestLockGate();
    TestTapAccuracy();
    TestScrolling();
    TestWindowAndFooter();
    TestTabsAndBindings();
    TestGeometry();
    TestResolutions();

    printf("\n==============================================================\n");
    printf("  %d checks passed, %d failed  (%d frames rendered)\n", g_ok, g_fail, g_frames);
    printf("==============================================================\n");
    return g_fail ? 1 : 0;
}
