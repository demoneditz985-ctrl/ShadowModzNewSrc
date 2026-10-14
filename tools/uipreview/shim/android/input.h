#pragma once
#include <stdint.h>
typedef struct AInputEvent AInputEvent;
typedef struct ANativeWindow ANativeWindow;
typedef struct AConfiguration AConfiguration;
enum { AINPUT_EVENT_TYPE_KEY = 1, AINPUT_EVENT_TYPE_MOTION = 2 };
enum { AKEY_EVENT_ACTION_DOWN = 0, AKEY_EVENT_ACTION_UP = 1, AKEY_EVENT_ACTION_MULTIPLE = 2 };
enum { AMOTION_EVENT_ACTION_DOWN = 0, AMOTION_EVENT_ACTION_UP = 1, AMOTION_EVENT_ACTION_MOVE = 2, AMOTION_EVENT_ACTION_CANCEL = 3 };
static inline int32_t AInputEvent_getType(const AInputEvent* e){ (void)e; return 0; }
static inline int32_t AKeyEvent_getKeyCode(const AInputEvent* e){ (void)e; return 0; }
static inline int32_t AMotionEvent_getX(const AInputEvent* e, size_t p){ (void)e;(void)p; return 0; }
static inline int32_t AMotionEvent_getY(const AInputEvent* e, size_t p){ (void)e;(void)p; return 0; }
static inline int32_t ANativeWindow_getWidth(ANativeWindow* w){ (void)w; return 0; }
static inline int32_t ANativeWindow_getHeight(ANativeWindow* w){ (void)w; return 0; }
static inline int32_t AConfiguration_getDensity(AConfiguration* c){ (void)c; return 0; }
