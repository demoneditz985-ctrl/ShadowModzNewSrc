#pragma once
// ============================================================================
//  SHADOW MODZ - TOUCH HANDLING
// ----------------------------------------------------------------------------
//  Unity only exposes Input.GetTouch(0): "a" finger, not "the" finger.  When
//  the finger we were following lifts while another one is still on the glass,
//  index 0 silently becomes that other finger, so the position jumps across
//  the screen.
//
//  The old code wrote io.MousePos straight from that value every frame, which
//  caused the two symptoms that were reported:
//    * a tap could land on a different row than the one pressed (the cursor
//      teleported between the press and the release frame),
//    * ImGui's ActiveId could be left stranded when the up-edge never arrived,
//      so the next tap "went somewhere else" entirely.
//
//  This version owns one finger id at a time:
//    * a press belongs to the finger that started it,
//    * if that finger disappears while other fingers are down, the gesture is
//      cancelled without emitting a click and input stays ignored until the
//      whole glass is free (so a second finger cannot fire a ghost tap),
//    * a tap that lasts less than one rendered frame is stretched to two
//      frames at the exact position it happened, so it is never dropped,
//    * a drag follows the same finger 1:1 (no offset, no lag).
// ============================================================================

#include "imgui.h"

namespace ShadowTouch
{

// A press shorter than one rendered frame is still delivered this many frames,
// so a fast tap cannot be lost between two SwapBuffers calls.
inline const int kMinDownFrames = 2;

struct State
{
    int    finger      = -1;
    bool   active      = false;
    bool   swallow     = false;    // ignore input until all fingers are lifted
    bool   releaseNext = false;
    int    downFrames  = 0;
    ImVec2 last        = ImVec2(0.0f, 0.0f);
    bool   wentUp      = false;    // a stroke ended on this update
};

inline State st;

// Called once per frame, before ImGui::NewFrame().
//   touchCount  UnityEngine.Input.touchCount
//   fingerId    Input.GetTouch(0).fingerId
//   x, y        Input.GetTouch(0).position  (Unity: origin bottom-left)
//   screenH     panel height in pixels (for the Y flip)
// Everything is expressed in the same pixel space as io.DisplaySize.
inline void Update(int touchCount, int fingerId, float x, float y, float screenH)
{
    ImGuiIO& io = ImGui::GetIO();
    const float fy = (screenH > 1.0f) ? (screenH - y) : y;
    const ImVec2 pos(x, fy);
    const bool   wasDown = io.MouseDown[0];

    if (touchCount > 0)
    {
        if (st.swallow)
        {
            // somebody else's finger is on the glass: never emit a click
            io.MouseDown[0] = false;
            st.wentUp = wasDown;
            return;
        }

        if (!st.active)
        {
            // new stroke
            st.active      = true;
            st.finger      = fingerId;
            st.downFrames  = 1;
            st.releaseNext = false;
            st.last        = pos;
            io.MousePos     = pos;
            io.MouseDown[0] = true;
            st.wentUp = false;
            return;
        }

        if (fingerId == st.finger)
        {
            // the same finger: follow it exactly
            st.last = pos;
            io.MousePos     = pos;
            io.MouseDown[0] = true;
            if (st.downFrames < 1000) ++st.downFrames;
            st.wentUp = false;
            return;
        }

        // Our finger is gone but another one took slot 0.  Cancel the gesture
        // and keep the cursor parked instead of teleporting onto a new row.
        io.MousePos     = pos;
        io.MouseDown[0] = false;
        st.active   = false;
        st.finger   = -1;
        st.swallow  = true;
        st.wentUp   = wasDown;
        return;
    }

    // ---- nothing on the glass ----
    if (st.releaseNext)
    {
        io.MouseDown[0] = false;
        st.releaseNext  = false;
        st.active       = false;
        st.finger       = -1;
        st.swallow      = false;
        st.wentUp       = wasDown;
        return;
    }

    if (st.active && st.downFrames < kMinDownFrames)
    {
        // very fast tap: hold it one extra frame at the position it happened
        io.MousePos     = st.last;
        io.MouseDown[0] = true;
        st.releaseNext  = true;
        st.wentUp       = false;
        return;
    }

    io.MouseDown[0] = false;
    st.active       = false;
    st.finger       = -1;
    st.swallow      = false;
    st.wentUp       = wasDown;
}

// True while a finger is being tracked (or one is still holding the glass).
inline bool Busy() { return st.active || st.swallow; }

} // namespace ShadowTouch
