// ============================================================================
//  SHADOW MODZ - branding logo texture
//  ---------------------------------------------------------------------------
//  Decodes the embedded JPEG (ShadowLogoData.h) with stb_image and uploads it
//  to an OpenGL texture used by the floating menu header / launcher.
//  Everything is best-effort: if anything fails the UI falls back to the
//  vector "SM" mark, so the menu keeps working on every device.
//
//  Telegram : https://t.me/+BBimnHMiSvpiYTBl
// ============================================================================

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <android/log.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "SHADOWMODZ MODS/ShadowLogoData.h"
#include "SHADOWMODZ MODS/ShadowTheme.hpp"

#ifndef SMDZ_LOGO_LOGI
#define SMDZ_LOGO_LOGI(...) __android_log_print(ANDROID_LOG_INFO, "ShadowModz", __VA_ARGS__)
#endif

namespace ShadowUI
{
    static unsigned int g_logoTex = 0;

    bool CreateLogoTexture()
    {
        if (g_logoTex != 0)
        {
            LogoTexture = (void*)(uintptr_t)g_logoTex;
            return true;
        }

        int w = 0, h = 0, channels = 0;
        unsigned char* pixels = stbi_load_from_memory(
            kShadowModzLogoJpeg, SHADOWMODZ_LOGO_BYTES, &w, &h, &channels, 4);

        if (!pixels)
        {
            SMDZ_LOGO_LOGI("logo decode failed, using fallback mark");
            return false;
        }

        GLint lastTex = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTex);

        GLuint tex = 0;
        glGenTextures(1, &tex);
        if (tex != 0)
        {
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            glBindTexture(GL_TEXTURE_2D, (GLuint)lastTex);
            g_logoTex = tex;
            LogoTexture = (void*)(uintptr_t)g_logoTex;
            LogoAspect = (h > 0) ? ((float)w / (float)h) : 1.0f;
            SMDZ_LOGO_LOGI("logo texture ready (%dx%d)", w, h);
        }
        else
        {
            SMDZ_LOGO_LOGI("logo texture allocation failed, using fallback mark");
        }

        stbi_image_free(pixels);
        return g_logoTex != 0;
    }

    void ReleaseLogoTexture()
    {
        if (g_logoTex != 0)
        {
            GLuint tex = g_logoTex;
            glDeleteTextures(1, &tex);
            g_logoTex = 0;
            LogoTexture = nullptr;
        }
    }
}
