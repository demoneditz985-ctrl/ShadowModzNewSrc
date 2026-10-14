#!/usr/bin/env bash
# ============================================================================
#  SHADOW MODZ - device translation unit syntax check (PC, no NDK needed)
# ----------------------------------------------------------------------------
#  Compiles the two device-only translation units with -fsyntax-only and the
#  real mod headers, using the hand-written Android stubs in
#  tools/uipreview/shim/ instead of the NDK headers.
#
#  This catches the class of error that only shows up deep in a CI build
#  (missing declaration, macro collision with Class.h, wrong call signature),
#  in a couple of seconds:
#
#     $ tools/uipreview/device_syntax_check.sh
#
#  Note: gcc can not digest the x86-style macros in Class.h/obfuscate.h the way
#  clang (the NDK compiler) does, so a handful of errors from those two stock
#  game headers is expected - only the Shadow*.hpp / shadowmodz_*.cpp errors
#  matter, and the script filters them out.
# ============================================================================
set -u
cd "$(dirname "$0")/../.." || exit 1

JNI="ShadowModz/app/src/main/jni"
SHIM="tools/uipreview/shim"
SRC="$JNI/shadowmodz_logo.cpp $JNI/shadowmodz_main.cpp"

FLAGS=(-std=c++17 -fsyntax-only -w -fpermissive
       -D__fastcall= -D__thiscall= -D__cdecl= -D__stdcall=
       -I "$SHIM" -I "$JNI" -I "$JNI/include" -I "$JNI/Dobby" -I "$JNI/imgui"
       -I "$JNI/KittyMemory" -I "$JNI/Unity" -I "$JNI/include/And64InlineHook"
       -I "$JNI/include/Substrate")

fail=0
for f in $SRC; do
    out=$(g++ "${FLAGS[@]}" "$f" 2>&1 | grep -E "error" \
          | grep -E "Shadow(Theme|Menu|Auth|Touch)\.hpp|shadowmodz_(main|logo)\.cpp")
    if [ -n "$out" ]; then
        echo "--- $f"
        echo "$out" | head -20
        fail=1
    else
        echo "ok  : $f"
    fi
done
exit $fail
