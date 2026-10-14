#pragma once
#include <stdint.h>
#include <stddef.h>
typedef unsigned int GLenum; typedef unsigned int GLuint; typedef int GLint; typedef int GLsizei;
typedef unsigned char GLboolean; typedef unsigned char GLubyte; typedef float GLfloat;
typedef char GLchar; typedef void GLvoid; typedef unsigned int GLbitfield;
typedef intptr_t GLintptr; typedef ptrdiff_t GLsizeiptr;
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_BINDING_2D 0x8069
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_LINEAR 0x2601
#define GL_NEAREST 0x2600
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_RGBA 0x1908
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_SHORT 0x1403
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_TRUE 1
#define GL_FALSE 0
#define GL_NO_ERROR 0
#define GL_BLEND 0x0BE2
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STREAM_DRAW 0x88E0
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_SCISSOR_TEST 0x0C11
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_VERSION 0x1F02
#define GL_EXTENSIONS 0x1F03
#define GL_TRIANGLES 0x0004
static inline void glGenTextures(GLsizei n, GLuint* t){ (void)n;(void)t; }
static inline void glDeleteTextures(GLsizei n, const GLuint* t){ (void)n;(void)t; }
static inline void glBindTexture(GLenum a, GLuint b){ (void)a;(void)b; }
static inline void glTexParameteri(GLenum a, GLenum b, GLint c){ (void)a;(void)b;(void)c; }
static inline void glTexImage2D(GLenum a, GLint b, GLint c, GLsizei d, GLsizei e, GLint f, GLenum g, GLenum h, const void* i){ (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g;(void)h;(void)i; }
static inline void glGetIntegerv(GLenum a, GLint* b){ (void)a;(void)b; }
static inline void glGetFloatv(GLenum a, GLfloat* b){ (void)a;(void)b; }
static inline void glEnable(GLenum a){ (void)a; }
static inline void glDisable(GLenum a){ (void)a; }
static inline void glBlendFunc(GLenum a, GLenum b){ (void)a;(void)b; }
static inline void glBlendEquation(GLenum a){ (void)a; }
static inline void glScissor(GLint a, GLint b, GLsizei c, GLsizei d){ (void)a;(void)b;(void)c;(void)d; }
static inline void glViewport(GLint a, GLint b, GLsizei c, GLsizei d){ (void)a;(void)b;(void)c;(void)d; }
static inline void glClearColor(GLfloat a, GLfloat b, GLfloat c, GLfloat d){ (void)a;(void)b;(void)c;(void)d; }
static inline void glClear(GLbitfield a){ (void)a; }
static inline void glDrawElements(GLenum a, GLsizei b, GLenum c, const void* d){ (void)a;(void)b;(void)c;(void)d; }
static inline void glDrawArrays(GLenum a, GLint b, GLsizei c){ (void)a;(void)b;(void)c; }
static inline void glReadPixels(GLint a, GLint b, GLsizei c, GLsizei d, GLenum e, GLenum f, void* g){ (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g; }
static inline const GLubyte* glGetString(GLenum a){ (void)a; return (const GLubyte*)""; }
static inline GLenum glGetError(void){ return 0; }
