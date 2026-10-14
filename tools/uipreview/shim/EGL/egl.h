#pragma once
#include <stdint.h>
typedef void* EGLDisplay; typedef void* EGLSurface; typedef void* EGLContext; typedef void* EGLConfig;
typedef int32_t EGLBoolean; typedef int32_t EGLint; typedef unsigned int EGLenum;
#define EGL_WIDTH 0x3057
#define EGL_HEIGHT 0x3056
#define EGL_TRUE 1
#define EGL_FALSE 0
static inline EGLBoolean eglQuerySurface(EGLDisplay d, EGLSurface s, EGLint a, EGLint* v){ (void)d;(void)s;(void)a;(void)v; return EGL_TRUE; }
static inline EGLBoolean eglSwapBuffers(EGLDisplay d, EGLSurface s){ (void)d;(void)s; return EGL_TRUE; }
