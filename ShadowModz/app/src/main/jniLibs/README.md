# app/src/main/jniLibs

Put the **injector** libraries here, one folder per ABI:

```
jniLibs/arm64-v8a/libcatchmeifyoucan.so     (root injector used by InjectRoot)
jniLibs/armeabi-v7a/libcatchmeifyoucan.so
jniLibs/arm64-v8a/libinject.so              (virtual / no-root injector, exec.java)
jniLibs/armeabi-v7a/libinject.so
```

They are **not part of the SHADOW MODZ source drop** - the original project
shipped them as prebuilt binaries, so add your own copies exactly like in the
original build.

The injected **payload** (`libmasgik.so`) is built by this project from
`app/src/main/jni` (`LOCAL_MODULE := libmasgik`) and lands in the APK
automatically - do not put it here.
