# SHADOW MODZ — v4.0 PREMIUM EDITION

Fully rebranded **SHADOW MODZ** floating mod menu for the source that was
uploaded as `Shadow.zip`. Nothing about the cheat features was removed — every
original module is still wired to the same globals, the UI was rebuilt from
scratch and the whole project was rebranded.

<p align="center">
  <b>TELEGRAM → <a href="https://t.me/+BBimnHMiSvpiYTBl">https://t.me/+BBimnHMiSvpiYTBl</a></b>
</p>

---

## What changed

### 1. Brand new floating menu UI (not a recolour)
The old cyan "b-tik / ADITYA" ImGui window is gone. Everything on screen is now
rendered by a new design system written from scratch:

| File | Purpose |
|------|---------|
| `app/src/main/jni/SHADOWMODZ MODS/ShadowTheme.hpp` | Design system: black + purple palette, 6 accent presets, style engine, primitives (glow, gradients, dotted grid, cards, chips), touch-scroll + flick physics |
| `app/src/main/jni/SHADOWMODZ MODS/ShadowMenu.hpp`  | The floating menu itself: launcher, window, nav rail, 5 tabs, settings, presets, toasts |
| `app/src/main/jni/SHADOWMODZ MODS/ShadowLogoData.h`| Embedded branding logo (generated from `IMG-20261009-WA0000.jpg`) |
| `app/src/main/jni/shadowmodz_logo.cpp`             | Decodes the logo with stb_image → OpenGL texture (falls back to a vector "SM" mark) |

New layout and behaviour:

* **Draggable round launcher** with the logo, rotating dashed ring, pulsing
  purple glow and pink status dot. Drag it anywhere; it snaps to the nearest
  screen edge and remembers where you left it. Tap = open/close.
* **Draggable + resizable window** (drag the header, resize from the bottom-right
  grip). Black glass frame, purple neon edge, subtle dot grid.
* **Scrollable nav rail** with animated active state and active-module dots.
* **Scrollable content area** with finger-drag scrolling and flick momentum,
  plus a purple scrollbar. Rows cancel their own tap when your finger becomes a
  drag, so scrolling never toggles a switch by accident.
* **Custom touch widgets** designed for a phone screen, no desktop widgets:
  * `ToggleRow` — full-width row, animated purple switch, optional icon + hint
  * `SliderRow` / `IntSliderRow` — label, live value pill, ticked neon track
  * `SelectRow` — tap to cycle through options (no fiddly combo boxes)
  * `Button`, `IconButton`, `Chip`, `InfoRow`, `MeterBar`, `SectionTitle`,
    `CardBegin/CardEnd`, `DividerText`, toast notifications
* **Five tabs** — AIMBOT · VISUAL · MISC · NETWORK · SETTINGS
* **Correct finger → ImGui coordinate mapping** via the same EGL swap hook, and
  the launcher is hit-tested directly so it can never steal menu clicks.

### 2. Every original feature kept working
All modules are exposed through an X-macro table (`SHADOW_BOOL_FIELDS`,
`SHADOW_FLOAT_FIELDS`, `SHADOW_INT_FIELDS`) and bound once in
`ShadowBindMenu()` (`shadowmodz_main.cpp`) to the real globals, so the UI and the
cheat logic can never drift apart:

* Aimbot / Silent aim / Aim silent / Headshot / Aim visible check + thread /
  ANTIDEV / AimKill / AutoKill / AimLock (+ through wall) / Custom fire rate /
  SpinBot and all their FOV, distance, delay, speed and burst sliders
* ESP line, 2D box, 3D box, filled, half, health, identity, distance, bone ESP,
  FOV circle, sniper alert, grenade ESP, night mode + line/box thickness,
  roundness, line mode and 12 box styles
* Speed hack, speed run, fast switch, auto run, free move, fast sensitivity,
  parachuting, no recoil, infinite range, sniper fast, PC swap anim, shuriken
  kill, quick reload, medkit faster rate/moving, stop fire anim, mass kill,
  camera height, set India, guest reset, spoof name, user level spoof, auto
  revive, glider hack/infinite, bypass login 400 and the value sliders
* Firewall with live console, traffic counters, hook status and whitelist info
* Telegram button in the header, in the nav rail and in SETTINGS → opens
  `https://t.me/+BBimnHMiSvpiYTBl` through `Application.OpenURL`

New in the UI on top of the old one: presets (3 save/load slots), accent colour
picker, UI scale, window opacity, window size, launcher size, glow strength,
compact rows, FPS counter, status bar and floating-button toggles.

### 3. Full rebrand
* Theme colour: cyan/teal → **black + purple** (`#07050B` / `#A755F7` / `#E879F9`)
* Watermark: `b-tik // CYBER` → **SHADOW MODZ**
* Log tags, window titles, nav labels, status bar and toasts all say SHADOW MODZ
* `ADITYA SRC ON TOP` → **SHADOW MODZ** (`strings.xml`, launcher label)
* Telegram `https://t.me/+BBimnHMiSvpiYTBl` in the menu, the launcher activity
  and this README
* Launcher icons regenerated from your logo (all mipmap densities),
  `assets/shadowmodz_logo.jpg` + embedded C array for the in-game logo

### 4. Renamed files / folders
| Before | After |
|--------|-------|
| `app/src/main/jni/btikmain.cpp` | `app/src/main/jni/shadowmodz_main.cpp` |
| `app/src/main/jni/BTIK MODS/` | `app/src/main/jni/SHADOWMODZ MODS/` |
| `BTIK MODS/btik_functions.h` | `SHADOWMODZ MODS/shadow_functions.h` |
| `BTIK MODS/btikaimbot.h` | `SHADOWMODZ MODS/shadow_aimbot.h` |
| `app/src/main/jni/BTIK/` | `app/src/main/jni/SHADOWMODZ/` |
| `BTIK/Tools/Tools.cpp/.h` | `SHADOWMODZ/Tools/ShadowTools.cpp/.h` |
| `app/src/main/jni/BTIK X SRC/` | `app/src/main/jni/SHADOWMODZ X SRC/` |
| `by btik[dont steel credits gays!].h` | `shadowmodz_credits.h` |
| `com/android/support/*.java` | `com/shadowmodz/support/*.java` (moved into `src/main/java` so Gradle compiles them and the manifest reference resolves) |
| `applicationId com.akn` | `com.shadowmodz` |
| native module `ADITYA` | `shadowmodz` → `libshadowmodz.so` |
| 3 junk emoji folders | `shadowmodz/` |

### 5. Build fixes found while validating
* `unity.h` vs `Unity.h`: case mismatch broke case-sensitive hosts (Linux/NDK) —
  added `Unity/unity.h` shim.
* `Icon/Icon.h` is FontAwesome **5**, but the menu asked for FA6 names — icons
  now use the FA5 names that exist in the bundled font.
* Font atlas only loaded `0xf000–0xf3ff`; widened to the full FA range so every
  icon used by the menu renders instead of showing a box.
* `ImGui::GetStateStorage()->GetFloatRef()` result was returned as a reference
  (pointer → reference bug) — animation state now works.
* The card background was drawn in the wrong ImDrawList channel for this ImGui
  build, which made every card invisible — fixed by flipping the channels.

---

## Build

Android Studio / Gradle (NDK + CMake-free `ndkBuild`, ABI `arm64-v8a`,
`armeabi-v7a`):

```
app/src/main/jni/Android.mk        -> libshadowmodz.so (+ prebuilt libdobby)
app/build.gradle                   -> applicationId com.shadowmodz, v4.0
app/src/main/AndroidManifest.xml   -> com.shadowmodz.support.MainActivity
```

1. Open the `ShadowModz` folder as an Android project (or run
   `./gradlew assembleRelease` from an Android Studio terminal).
2. Install the APK, open it, grant root when the injector asks — it copies
   `libshadowmodz.so` into `/dev/`, injects it into `com.dts.freefireth` and
   launches the game.

> The app injects into Free Fire and must be granted root (as in the original
> source). Use it with an account you can afford to lose.

---

## Repository layout

```
ShadowModz/
├── README.md                      <- this file
├── app/
│   ├── build.gradle               (com.shadowmodz, v4.0)
│   ├── src/main/
│   │   ├── AndroidManifest.xml
│   │   ├── assets/shadowmodz_logo.jpg
│   │   ├── java/com/shadowmodz/support/   MainActivity, Prefs, Utils, exec
│   │   ├── res/                            brand icons + black/purple theme
│   │   └── jni/
│   │       ├── shadowmodz_main.cpp        entry point, hooks, menu binding
│   │       ├── shadowmodz_logo.cpp        logo texture (stb_image + GLES3)
│   │       ├── SHADOWMODZ MODS/
│   │       │   ├── ShadowTheme.hpp        design system
│   │       │   ├── ShadowMenu.hpp         floating menu
│   │       │   ├── ShadowLogoData.h       embedded logo
│   │       │   ├── shadow_functions.h     ESP / hooks / all feature logic
│   │       │   ├── firewall.h, Bools.h, Class.h, tools.hpp ...
│   │       ├── SHADOWMODZ/                il2cpp SDK, hacks, tools, dobby
│   │       ├── SHADOWMODZ X SRC/          imagemui backends + helpers
│   │       ├── imgui/, Icon/, KittyMemory/, Unity/, xdl/, include/
│   │       └── Android.mk / Application.mk
└── docs/                          UI screenshots (see below)
```

## UI screenshots

Rendered from the real headers on a desktop host
(`docs/` folder — produced by compiling the UI against ImGui 1.89.2):

| File | Tab |
|------|-----|
| `docs/ui_aimbot.png`   | AIMBOT |
| `docs/ui_visual.png`   | VISUAL |
| `docs/ui_network.png`  | NETWORK (firewall console, traffic meters) |
| `docs/ui_settings.png` | SETTINGS (interface, accent, presets, about) |
| `docs/ui_launcher.png` | floating launcher |

---

**SHADOW MODZ** — Telegram: <https://t.me/+BBimnHMiSvpiYTBl>
