// minimal JNI shim - only for host syntax checking of the native sources
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <string.h>
typedef int8_t jbyte; typedef uint16_t jchar; typedef int16_t jshort; typedef int32_t jint;
typedef int64_t jlong; typedef float jfloat; typedef double jdouble; typedef uint8_t jboolean;
typedef jint jsize;
typedef unsigned char jboolean_uc;
#define JNI_FALSE 0
#define JNI_TRUE 1
#define JNI_VERSION_1_6 0x00010006
#define JNI_OK 0
#define JNI_COMMIT 1
#define JNI_ABORT 2
#ifdef __cplusplus
struct _jobject;
typedef _jobject* jobject; typedef _jobject* jclass; typedef _jobject* jstring;
typedef _jobject* jarray; typedef _jobject* jobjectArray; typedef _jobject* jthrowable;
typedef _jobject* jweak; typedef _jobject* jmethodID; typedef _jobject* jfieldID;
typedef union jvalue { jboolean z; jbyte b; jchar c; jshort s; jint i; jlong j; jfloat f; jdouble d; jobject l; } jvalue;
struct JNINativeMethod { char* name; char* signature; void* fnPtr; };
struct JNIEnv;
struct JavaVM {
    jint (*AttachCurrentThread)(JNIEnv** env, void* args);
    jint (*DetachCurrentThread)();
    jint (*GetEnv)(void** env, jint version);
};
typedef struct JNINativeInterface_* jniNativeInterfacePtr;
struct JNIEnv {
    void* reserved0; void* reserved1; void* reserved2; void* reserved3;
    jint (*GetVersion)(void*);
    jclass (*FindClass)(const char*);
    jmethodID (*GetMethodID)(jclass, const char*, const char*);
    jmethodID (*GetStaticMethodID)(jclass, const char*, const char*);
    jfieldID (*GetFieldID)(jclass, const char*, const char*);
    jfieldID (*GetStaticFieldID)(jclass, const char*, const char*);
    jobject (*NewObject)(jclass, jmethodID, ...);
    jobject (*AllocObject)(jclass);
    jmethodID (*GetStaticMethodIDV)(jclass, const char*, const char*);
    jobject (*CallObjectMethod)(jobject, jmethodID, ...);
    jobject (*CallStaticObjectMethod)(jclass, jmethodID, ...);
    jboolean (*CallBooleanMethod)(jobject, jmethodID, ...);
    jint (*CallIntMethod)(jobject, jmethodID, ...);
    jfloat (*CallFloatMethod)(jobject, jmethodID, ...);
    void (*CallVoidMethod)(jobject, jmethodID, ...);
    void (*CallStaticVoidMethod)(jclass, jmethodID, ...);
    jobject (*GetObjectField)(jobject, jfieldID);
    jobject (*GetStaticObjectField)(jclass, jfieldID);
    void (*SetObjectField)(jobject, jfieldID, jobject);
    const char* (*GetStringUTFChars)(jstring, jboolean*);
    void (*ReleaseStringUTFChars)(jstring, const char*);
    jsize (*GetStringUTFLength)(jstring);
    jstring (*NewStringUTF)(const char*);
    jboolean (*IsInstanceOf)(jobject, jclass);
    jint (*GetArrayLength)(jarray);
    void* (*GetPrimitiveArrayCritical)(jarray, jboolean*);
    void (*ReleasePrimitiveArrayCritical)(jarray, void*, jint);
    void (*DeleteLocalRef)(jobject);
    void (*DeleteGlobalRef)(jobject);
    jobject (*NewGlobalRef)(jobject);
    jboolean (*IsSameObject)(jobject, jobject);
    jint (*Throw)(jthrowable);
    jthrowable (*ExceptionOccurred)();
    void (*ExceptionDescribe)();
    void (*ExceptionClear)();
    void (*ExceptionCheckV)();
};
typedef JNIEnv* JNIEnvPtr;
#else
typedef void* jobject; typedef void* jclass; typedef void* jstring; typedef void* jmethodID; typedef void* jfieldID;
typedef void* JNIEnv; typedef void* JavaVM;
struct JNINativeMethod { char* name; char* signature; void* fnPtr; };
#endif
#define JNIEXPORT __attribute__((visibility("default")))
#define JNICALL
#define JNI_OnLoad_Name "JNI_OnLoad"
