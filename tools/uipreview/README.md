# SHADOW MODZ — UI preview & smoke tests

The floating menu (`app/src/main/jni/SHADOWMODZ MODS/ShadowTheme.hpp`,
`ShadowMenu.hpp`, `ShadowTouch.hpp`, `ShadowAuth.hpp`) is plain ImGui, so it can
be compiled and driven on a normal PC with the exact ImGui 1.89.2 copy that
ships in `app/src/main/jni/imgui` — no Android device needed.

| File | What it does |
|------|--------------|
| `preview_main.cpp` | renders one frame per scenario, rasterizes the ImGui draw data with a small software rasterizer and writes a PPM (this is how the `docs/ui_*.png` images are produced) |
| `smoke_test.cpp` | drives synthetic touches (in Unity screen space) through the real UI and asserts behaviour — **94 checks** |
| `generate_stub.py` | regenerates `host_stub.hpp` from the X-macro field lists in `ShadowMenu.hpp` |
| `host_stub.hpp` | auto-generated stub of the game globals (never edit by hand) |

## What the smoke test covers

1. **login gate** — a fresh session is locked, no feature tab exists, the keypad
   is on screen and tappable, `GET KEY` opens the Telegram channel, typing fills
   the field, backspace edits it, a wrong key raises the red failure state and
   keeps the menu locked, `SHADOWMODZ` unlocks it, `LOCK MENU` re-arms the gate.
2. **tap accuracy** — a tap toggles exactly the row it lands on (centre, both
   corners and the switch), a stroke that leaves the row does not fire, a tap in
   the gap between two rows hits neither, a tab tap switches tabs.
3. **scrolling** — the scroll area is registered, a swipe scrolls, the drag does
   not toggle the row it started on, over-scrolling clamps at both ends, every
   tab can be scrolled to its last row.
4. **window / footer** — the header drags the window, dragging the header does
   not scroll the list, the window is clamped to the screen on all four edges,
   `JOIN` opens Telegram, `HIDE` minimises to the launcher bubble, the bubble can
   be dragged, stays on screen and reopens the panel, `EXIT MOD` disables the mod.
5. **tabs / bindings / widgets** — all 81 feature pointers are bound, all five
   tabs activate, a row on every tab writes the right game global, slider drag
   clamps to its range, the choice widget selects and notifies the mod, all six
   accent presets apply.
6. **geometry** — 5 tabs × 4 UI scales × 14 scroll steps: every widget rect is
   well formed and on screen (nothing inverted, nothing pushed off the panel).
7. **resolutions** — 640×360, 720×1280, 1080×1920, 1280×720, 1440×2560,
   2340×1080, a tiny 420×300 screen and a 0×0 frame (must be ignored, not crash).

## Build & run

```sh
JNI=app/src/main/jni
FLAGS='-std=c++17 -O1 -I tools/uipreview -I '"$JNI"' -I '"$JNI"'/imgui -I '"$JNI"'/Icon -I '"$JNI/SHADOWMODZ MODS'
IMGUI='"$JNI"'/imgui/imgui.cpp '"$JNI"'/imgui/imgui_draw.cpp '"$JNI"'/imgui/imgui_widgets.cpp '"$JNI"'/imgui/imgui_tables.cpp'

g++ $FLAGS tools/uipreview/smoke_test.cpp   $IMGUI -o smoke
./smoke            # prints every assertion, exits non-zero on failure

g++ $FLAGS tools/uipreview/preview_main.cpp $IMGUI -o preview
for s in lock lockfail aim esp catch misc system toast launcher; do
    ./preview $s docs/ui_$s.ppm
    convert docs/ui_$s.ppm docs/ui_$s.png      # or any PPM -> PNG tool
done
./preview aim /tmp/ui_1080p.ppm 1080x1920      # optional: scale check
```

`./preview` scenarios: `lock`, `lockfail`, `aim`, `esp`, `catch`, `misc`,
`system`, `toast`, `launcher` — an optional third argument sets the resolution
(`WxH`, default `720x1280`).

`host_stub.hpp` replaces the game side (Il2Cpp, firewall, feature flags, the GL
callbacks), so the UI renders standalone. The X-macro lists in `ShadowMenu.hpp`
are the single source of truth for the bindings — after editing them run:

```sh
python3 tools/uipreview/generate_stub.py
```
