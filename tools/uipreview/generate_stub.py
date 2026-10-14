#!/usr/bin/env python3
"""Regenerate host_stub.hpp from the X-macro field lists in ShadowMenu.hpp.

The X-macro lists are the single source of truth for the menu <-> feature
bindings, so the host test stub (and therefore the whole desktop harness) is
generated from them instead of being maintained by hand.

Bindings may point at members of a global struct ("Config.ESP.Box",
"Mass.Sucks"), so those structs are rebuilt from the paths too - that keeps
the stub byte-compatible with what ShadowBindMenu() compiles against.

    python3 tools/uipreview/generate_stub.py
"""
import re, os

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
MENU = os.path.join(ROOT, "ShadowModz", "app", "src", "main", "jni",
                    "SHADOWMODZ MODS", "ShadowMenu.hpp")
OUT  = os.path.join(ROOT, "tools", "uipreview", "host_stub.hpp")

src = open(MENU, encoding="utf-8").read()

def fields(macro):
    body = src[src.index("#define %s(X)" % macro):]
    body = body[:body.index("\n\n")]
    return re.findall(r'X\(\s*(\w+)\s*,\s*(\w+(?:\.\w+)*)\s*\)', body)

bools  = fields("SHADOW_BOOL_FIELDS")
floats = fields("SHADOW_FLOAT_FIELDS")
ints   = fields("SHADOW_INT_FIELDS")

FLOAT_DEFAULTS = {"FovAim": "100.0f", "AimDistance": "200.0f",
                  "AimSpeed": "0.50f", "LineThickness": "1.5f"}

# ---- split bindings into plain globals and nested struct members ----------
plain = []      # (type, name, default)
tree  = {}      # root -> nested dict, leaves are (type, default)

def add(path, typ, default):
    parts = path.split(".")
    if len(parts) == 1:
        plain.append((typ, path, default))
        return
    node = tree
    for part in parts[:-1]:
        node = node.setdefault(part, {})
    node[parts[-1]] = (typ, default)

for _, t in bools:  add(t, "bool",  "false")
for _, t in floats: add(t, "float", FLOAT_DEFAULTS.get(t, "0.0f"))
for _, t in ints:   add(t, "int",   "4" if t == "g_AimHeightIndex" else "0")

L = []
L.append("#pragma once")
L.append("// AUTO-GENERATED host stub of the game globals used by ShadowMenu.hpp.")
L.append("// The X-macro field lists in SHADOWMODZ MODS/ShadowMenu.hpp are the source")
L.append("// of truth, so this stub and the real ShadowBindMenu() stay in sync.")
L.append("// Regenerate with: python3 tools/uipreview/generate_stub.py")
L.append("")
L.append('#include "imgui.h"')
L.append("")

if tree:
    L.append("// ---- nested feature configs (Config.ESP.*, Mass.*, ...) ----")

    def emit_struct(node, path):
        name = "Struct_" + "_".join(path)
        L.append("struct %s" % name)
        L.append("{")
        for key, val in node.items():
            if isinstance(val, dict):
                emit_struct(val, path + [key])
                L.append("    Struct_%s %s;" % ("_".join(path + [key]), key))
            else:
                typ, default = val
                L.append("    %s %s = %s;" % (typ, key, default))
        L.append("};")

    for root, sub in tree.items():
        emit_struct(sub, [root])
        L.append("inline Struct_%s %s;" % (root, root))
        L.append("")

L.append("// ---- plain globals ----")
L += ["inline %s %s = %s;" % (typ, name, default) for typ, name, default in plain]
L.append("")
L.append("// ---- harness extras ----")
L.append("inline const char* g_urlOpened = nullptr;   // records hooks.openURL()")
L.append("")
L.append("namespace ShadowUI")
L.append("{")
L.append("inline bool CreateLogoTexture() { return false; }   // no GL context on the host")
L.append("inline void ReleaseLogoTexture() {}")
L.append("}")
L.append("")

open(OUT, "w", encoding="utf-8").write("\n".join(L))
print("wrote %s  (%d bool / %d float / %d int fields, %d nested roots)"
      % (OUT, len(bools), len(floats), len(ints), len(tree)))
