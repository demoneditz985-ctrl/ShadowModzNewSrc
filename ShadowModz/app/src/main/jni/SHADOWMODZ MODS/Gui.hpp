#pragma once

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <dlfcn.h>
#include <vector>

#include "imgui.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"
#include "main.h"
#include "SHADOWMODZ MODS/tools.hpp"

#include "Icon/OPPOSans-H.h"
#include "Icon/Icon.h"
#include "Icon/Iconcpp.h"
#include "Icon/Font.h"

#include <SHADOWMODZ MODS/shadow_functions.h>
#include "SHADOWMODZ MODS/firewall.h"

// SHADOW MODZ UI engine (black + purple design system + floating menu)
#include "SHADOWMODZ MODS/ShadowTouch.hpp"
#include "SHADOWMODZ MODS/ShadowTheme.hpp"
#include "SHADOWMODZ MODS/ShadowMenu.hpp"

inline static bool g_IsSetup = false;
inline int prevWidth, prevHeight;

inline void (*origInput)(void*, void*, void*);
inline void myInput(void *thiz, void *ex_ab, void *ex_ac) {
    origInput(thiz, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
    return;
}

float density = -1;

ImFont *font;

inline void SetupImgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)g_GlWidth, (float)g_GlHeight);
    io.IniFilename = nullptr;                 // never write imgui.ini on the device
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init("#version 300 es");

    // ------------------------------------------------------------------
    //  TYPOGRAPHY
    // ------------------------------------------------------------------
    //  The glyph atlas is rasterised at the panel resolution instead of a
    //  fixed 26px.  On a 1080p+ phone the old atlas was upscaled and every
    //  letter/icon came out soft.  The UI metrics use the very same scale
    //  (ShadowUI::theme.uiScale), so text and rows stay 1:1 with the atlas.
    //  720x1280 is the 1.0 reference, the smaller axis decides - keep this in
    //  sync with ShadowMenu::Draw(), which scales the widget metrics.
    float screenScale = 1.0f;
    if (g_GlWidth > 0 && g_GlHeight > 0)
        screenScale = ImMin((float)g_GlWidth / 720.0f, (float)g_GlHeight / 1280.0f);
    if (screenScale < 0.75f) screenScale = 0.75f;
    if (screenScale > 2.00f) screenScale = 2.00f;
    const float fontSize = 26.0f * screenScale;

    // the design system scales text by uiScale, so its base size stays 26 and
    // the rasterised atlas above meets it exactly at userScale == 1.0
    ShadowUI::theme.fontSize = 26.0f;

    // full FontAwesome range so every icon used by the menu is available
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 3;
    icons_config.OversampleV = 3;
    ImFontConfig CustomFont;
    CustomFont.FontDataOwnedByAtlas = false;
    io.Fonts->AddFontFromMemoryTTF(
        const_cast<std::uint8_t*>(Custom),
        sizeof(Custom),
        fontSize,
        &CustomFont
    );
    io.Fonts->AddFontFromMemoryCompressedTTF(
        font_awesome_data,
        font_awesome_size,
        fontSize,
        &icons_config,
        icons_ranges
    );
    ImFontConfig cfg;                          // last-resort fallback font
    cfg.SizePixels = fontSize;
    io.Fonts->AddFontDefault(&cfg);
    ImGui::GetStyle().ScaleAllSizes(1.8f);

    // ------------------------------------------------------------------
    //  INPUT
    // ------------------------------------------------------------------
    //  Touch panels wobble a few pixels while a finger rests on the glass,
    //  and two deliberate taps in a row must never be merged into one
    //  double-click (that swallowed the second tap on the old build).
    io.MouseDrawCursor                = false;
    io.MouseDragThreshold             = 24.0f;
    io.MouseDoubleClickTime           = 0.60f;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    LOGD("SHADOW MODZ: imgui ready (atlas %.0fpx)", fontSize);
}

/*
void SetupImgui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle* style = &ImGui::GetStyle();
    io.DisplaySize = ImVec2((float)g_GlWidth, (float)g_GlHeight);

    ImGui::StyleColorsDark();
    
    style->WindowRounding = 5.3f;
    style->FrameRounding = 2.3f;
    style->ScrollbarRounding = 0;
    style->WindowTitleAlign = ImVec2(0.5, 0.5);        
    style->FramePadding = ImVec2(8, 6);
    style->ScaleAllSizes(1.0f);
    style->ScrollbarSize /= 1;

    
    ImGui_ImplOpenGL3_Init("#version 300 es");
    
    // We load the default font with increased size to improve readability on many devices with "high" DPI.
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 22.0f;
    io.Fonts->AddFontDefault(&font_cfg);

    // Arbitrary scale-up
    ImGui::GetStyle().ScaleAllSizes(3.0f);
}

*/

JavaVM *jvm;
JNIEnv *genv;

std::string getClipboard() {
    std::string result;
    
    jvm->AttachCurrentThread(&genv, NULL);
    
    auto looperClass = genv->FindClass(OBFUSCATE("android/os/Looper"));
    auto prepareMethod = genv->GetStaticMethodID(looperClass, OBFUSCATE("prepare"), OBFUSCATE("()V"));
    genv->CallStaticVoidMethod(looperClass, prepareMethod);
    
    jclass activityThreadClass = genv->FindClass(OBFUSCATE("android/app/ActivityThread"));
    jfieldID sCurrentActivityThreadField = genv->GetStaticFieldID(activityThreadClass, OBFUSCATE("sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
    jobject sCurrentActivityThread = genv->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);
    
    jfieldID mInitialApplicationField = genv->GetFieldID(activityThreadClass, OBFUSCATE("mInitialApplication"), OBFUSCATE("Landroid/app/Application;"));
    jobject mInitialApplication = genv->GetObjectField(sCurrentActivityThread, mInitialApplicationField);
    
    auto contextClass = genv->FindClass(OBFUSCATE("android/content/Context"));
    auto getSystemServiceMethod = genv->GetMethodID(contextClass, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
    
    auto str = genv->NewStringUTF(OBFUSCATE("clipboard"));
    auto clipboardManager = genv->CallObjectMethod(mInitialApplication, getSystemServiceMethod, str);
    genv->DeleteLocalRef(str);
    
    jclass ClipboardManagerClass = genv->FindClass(OBFUSCATE("android/content/ClipboardManager"));
    auto getText = genv->GetMethodID(ClipboardManagerClass, OBFUSCATE("getText"), OBFUSCATE("()Ljava/lang/CharSequence;"));

    jclass CharSequenceClass = genv->FindClass(OBFUSCATE("java/lang/CharSequence"));
    auto toStringMethod = genv->GetMethodID(CharSequenceClass, OBFUSCATE("toString"), OBFUSCATE("()Ljava/lang/String;"));

    auto text = genv->CallObjectMethod(clipboardManager, getText);
    if (text) {
        str = (jstring) genv->CallObjectMethod(text, toStringMethod);
        result = genv->GetStringUTFChars(str, 0);
        genv->DeleteLocalRef(str);
        genv->DeleteLocalRef(text);
    }
    genv->DeleteLocalRef(CharSequenceClass);
    genv->DeleteLocalRef(ClipboardManagerClass);
    genv->DeleteLocalRef(clipboardManager);
    genv->DeleteLocalRef(contextClass);
    genv->DeleteLocalRef(mInitialApplication);
    genv->DeleteLocalRef(activityThreadClass);   
    jvm->DetachCurrentThread();
    return result.c_str();
}

jclass UnityPlayer_cls;
jfieldID UnityPlayer_CurrentActivity_fid;

jobject getGlobalContext()
{
    jclass activityThread = genv->FindClass(OBFUSCATE("android/app/ActivityThread"));
    jmethodID currentActivityThread = genv->GetStaticMethodID(activityThread, OBFUSCATE("currentActivityThread"), OBFUSCATE("()Landroid/app/ActivityThread;"));
    jobject at = genv->CallStaticObjectMethod(activityThread, currentActivityThread);
    jmethodID getApplication = genv->GetMethodID(activityThread, OBFUSCATE("getApplication"), OBFUSCATE("()Landroid/app/Application;"));
    jobject context = genv->CallObjectMethod(at, getApplication);
    return context;
}

void displayKeyboard(bool pShow) {
    jclass ctx = genv->FindClass(OBFUSCATE("android/content/Context"));
    jfieldID fid = genv->GetStaticFieldID(ctx, OBFUSCATE("INPUT_METHOD_SERVICE"), OBFUSCATE("Ljava/lang/String;"));
    jmethodID mid = genv->GetMethodID(ctx, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
    jobject context = genv->GetStaticObjectField(UnityPlayer_cls, UnityPlayer_CurrentActivity_fid);
    jobject InputManObj = genv->CallObjectMethod(context, mid, (jstring) genv->GetStaticObjectField(ctx, fid));
    jclass ClassInputMethodManager = genv->FindClass(OBFUSCATE("android/view/inputmethod/InputMethodManager"));
    jmethodID toggleSoftInputId = genv->GetMethodID(ClassInputMethodManager, OBFUSCATE("toggleSoftInput"), OBFUSCATE("(II)V"));
    if (pShow) {
        genv->CallVoidMethod(InputManObj, toggleSoftInputId, 2, 0);
    } else {
        genv->CallVoidMethod(InputManObj, toggleSoftInputId, 0, 0);
    }
}

int ShowSoftKeyboardInput() {
    jint result;
    jint flags = 0;
    
    JNIEnv *env;
    jvm->AttachCurrentThread(&env, NULL);
    
    jclass looperClass = env->FindClass(OBFUSCATE("android/os/Looper"));
    auto prepareMethod = env->GetStaticMethodID(looperClass, OBFUSCATE("prepare"), OBFUSCATE("()V"));
    env->CallStaticVoidMethod(looperClass, prepareMethod);
    
    jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
    jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, OBFUSCATE("sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
    jobject sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);
    
    jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass, OBFUSCATE("mInitialApplication"), OBFUSCATE("Landroid/app/Application;"));
    jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread, mInitialApplicationField);
    
    jclass contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
    jfieldID fieldINPUT_METHOD_SERVICE = env->GetStaticFieldID(contextClass, OBFUSCATE("INPUT_METHOD_SERVICE"), OBFUSCATE("Ljava/lang/String;"));
    jobject INPUT_METHOD_SERVICE = env->GetStaticObjectField(contextClass, fieldINPUT_METHOD_SERVICE);
    jmethodID getSystemServiceMethod = env->GetMethodID(contextClass, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
    jobject callObjectMethod = env->CallObjectMethod(mInitialApplication, getSystemServiceMethod, INPUT_METHOD_SERVICE);
    
    jclass classInputMethodManager = env->FindClass(OBFUSCATE("android/view/inputmethod/InputMethodManager"));
    jmethodID toggleSoftInputId = env->GetMethodID(classInputMethodManager, OBFUSCATE("toggleSoftInput"), OBFUSCATE("(II)V"));
    
    if (result) {
        env->CallVoidMethod(callObjectMethod, toggleSoftInputId, 2, flags);
    } else {
        env->CallVoidMethod(callObjectMethod, toggleSoftInputId, flags, flags);
    }
    
    env->DeleteLocalRef(classInputMethodManager);
    env->DeleteLocalRef(callObjectMethod);
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(mInitialApplication);
    env->DeleteLocalRef(activityThreadClass);
    jvm->DetachCurrentThread();
    
    return result;
}

struct UnityEngine_Vector2_Fields {
    float x;
    float y;
};

struct UnityEngine_Vector2_o {
    UnityEngine_Vector2_Fields fields;
};

enum TouchPhase {
    Began = 0,
    Moved = 1,
    Stationary = 2,
    Ended = 3,
    Canceled = 4
};

struct UnityEngine_Touch_Fields {
    int32_t m_FingerId;
    struct UnityEngine_Vector2_o m_Position;
    struct UnityEngine_Vector2_o m_RawPosition;
    struct UnityEngine_Vector2_o m_PositionDelta;
    float m_TimeDelta;
    int32_t m_TapCount;
    int32_t m_Phase;
    int32_t m_Type;
    float m_Pressure;
    float m_maximumPossiblePressure;
    float m_Radius;
    float m_RadiusVariance;
    float m_AltitudeAngle;
    float m_AzimuthAngle;
};

