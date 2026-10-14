#ifndef MAIN_H
#define MAIN_H

#include <map>
#include <string>
#include <SHADOWMODZ MODS/tools.hpp>

struct cfg {
    bool aimbot;
    int aim_type = 0;
    int aim_target = 1;
    struct _esp {
        bool Box = false;
        bool Box3d = false;
        bool BoxFilled = false;
        bool BoxHalfFilled = false;
        bool Line = false;
        bool Health = false;
        bool MAFIA = false;
        bool LineRgb = false;
        bool Hexagono = false;
        bool Distancia = false;
        bool Distance = false;
        bool Skeleton = false;
        bool Grenade = false;
        bool IsName = false;
        bool Path = false;
        bool Esp360 = false;
        bool Target = false;
    }ESP;
} inline Config;

inline std::map < std::string, void*> _methods;
inline std::map < std::string, size_t > _fields;

template<typename T>
inline T getFieldValue(void *instance,ulong offset) {
    return *(T*)((uintptr_t)instance + offset);
}

template<typename T>
inline T setFieldValue(void *instance,ulong offset, T value) {
    *(T*)((uintptr_t)instance + offset) = value;
}

void menu();
void *getRealAddr(ulong offset);

#endif 
