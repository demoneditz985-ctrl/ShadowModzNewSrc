# SHADOW MODZ — UI preview & smoke tests

The floating menu in `app/src/main/jni/SHADOWMODZ MODS/ShadowTheme.hpp` and
`ShadowMenu.hpp` is written in plain ImGui, so it can be compiled and driven on
a normal PC (no Android device needed) together with the exact ImGui 1.89.2
copy that ships in `app/src/main/jni/imgui`.

Two harnesses are provided:

| File | What it does |
|------|--------------|
| `preview_main.cpp` | simulates a 720x1280 frame, rasterizes ImGui draw data with a small software rasterizer and writes a PPM screenshot (used to produce the `docs/ui_*.png` images) |
| `smoke_test.cpp`   | drives simulated touches through the UI and asserts behaviour: toggles flip on tap, sliders set their value, taps don't fire while scrolling, flick momentum keeps scrolling, the header drags the window, the launcher drags/snaps/taps, nav switches tabs, presets save & load, every tab renders, UI scale sweep 0.7x–1.4x |

`host_stub.hpp` replaces the game globals (Il2Cpp / firewall / feature flags)
so the UI can be exercised standalone; the X-macro field lists in
`ShadowMenu.hpp` are the single source of truth for the bindings, so the stub
and the real `ShadowBindMenu()` stay in sync.

## Build & run

```sh
JNI=app/src/main/jni
g++ -std=c++17 -O1 -I tools/uipreview -I "$JNI" -I "$JNI/imgui" -I "$JNI/imgui/fonts" \
    -I "$JNI/Icon" -I "$JNI/SHADOWMODZ MODS" \
    tools/uipreview/preview_main.cpp \
    "$JNI"/imgui/imgui.cpp "$JNI"/imgui/imgui_draw.cpp \
    "$JNI"/imgui/imgui_widgets.cpp "$JNI"/imgui/imgui_tables.cpp -o preview
./preview menu_aimbot aim.ppm          # menu_visual | menu_network | menu_settings | launcher

g++ -std=c++17 -O1 -I tools/uipreview -I "$JNI" -I "$JNI/imgui" -I "$JNI/imgui/fonts" \
    -I "$JNI/Icon" -I "$JNI/SHADOWMODZ MODS" \
    tools/uipreview/smoke_test.cpp \
    "$JNI"/imgui/imgui.cpp "$JNI"/imgui/imgui_draw.cpp \
    "$JNI"/imgui/imgui_widgets.cpp "$JNI"/imgui/imgui_tables.cpp -o smoke
./smoke                                # prints the assertions, exits non-zero on failure
```
