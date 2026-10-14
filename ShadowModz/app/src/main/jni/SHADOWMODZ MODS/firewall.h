#ifndef FIREWALL_H
#define FIREWALL_H

#include <jni.h>
#include <string>
#include <vector>
#include <mutex>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <dlfcn.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <android/log.h>

#include "Dobby/dobby.h"

#ifndef FW_LOG_TAG
#define FW_LOG_TAG "ShadowModzFirewall"
#endif

#ifndef FW_LOGD
#define FW_LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, FW_LOG_TAG, __VA_ARGS__)
#endif
#ifndef FW_LOGW
#define FW_LOGW(...) __android_log_print(ANDROID_LOG_WARN,  FW_LOG_TAG, __VA_ARGS__)
#endif
#ifndef FW_LOGE
#define FW_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, FW_LOG_TAG, __VA_ARGS__)
#endif

inline bool g_FirewallEnabled  = true;
inline bool g_FirewallVerbose  = true;   

struct FW_Stats {
    int totalChecks   = 0;
    int totalBlocked  = 0;
    int totalAllowed  = 0;
    int anogsBlocked  = 0;
    int domainBlocked = 0;
    int portBlocked   = 0;
    int privIpBlocked = 0;
    int libsHooked    = 0;
    bool hooksOk      = false;
};
inline FW_Stats g_FwStats;

struct FW_LogEntry {
    std::string time;
    std::string libName;
    std::string target;
    std::string port;
    bool isBlocked;
    std::string reason;
};

inline std::vector<FW_LogEntry> g_fwLogs;
inline std::mutex               g_fwLogMutex;
inline const int                FW_MAX_LOGS = 200;

inline const char* FW_ALLOWED_DOMAIN = "lixuim.site";
inline char        g_fwAllowedIpCache[128] = {0};

inline const int fw_blocked_ports[] = {
    0, 80, 443, 8080, 8443, 14006,
    8012, 9006, 11006,
    39800, 65535
};
inline const int fw_blocked_ports_count =
    sizeof(fw_blocked_ports) / sizeof(fw_blocked_ports[0]);

inline const char* fw_blocked_domains[] = {
    "dl.cdn.freefiremobile.com",
    "dl.dir.freefiremobile.com",
    "gin.freefiremobile.com",
    "na-gin.freefiremobile.com",
    "dl.castle.freefiremobile.com",
    "100067.msdk.garena.com",
    "100067.connect.garena.com",
    "bdversion.ggbluefox.com",
    "dd.garena.com",
    "vnnetwork.ggblueshark.com",
    "dl.cvs.freefiremobile.com",
    "loginbp.ggblueshark.com",
    "dl.listdl.com",
    "clientbp.ggblueshark.com",
    "dl.aw.freefiremobile.com",
    "dl.gmc.freefiremobile.com",
    "loginbp.ggpolarbear.com",
    "clientbp.ggpolarbear.com",
    "anogs.garena.com",
    "anticheat.garena.com",
    "antiextend.garena.com",
    "ff-report.garena.com",
    "report.garena.com",
    "bugly.qq.com",
    "msdk.qq.com",
    "tencentcs.com",
    "gcloud.garena.com",
    "gcloudsvr.garena.com",
    "ffmax.garena.com",
    "freefiremax.com",
    "dir.freefiremax.com",
    "anticheat.ff.garena.com",
    "ff.garena.com",
    "gin.freefireind.in",
    "gin.freefiremobile.com",
    "garena.co.id",
    "na.gin.freefiremobile.com",
    "gamesecurity.us.freefiremobile.com",
    "hotro.ff.garena.vn",
    "anticheat.garena.vn",
    "conversions.appsflyer.com",
    "gin.freefireind.in:443 ",
    nullptr
};
inline const int fw_blocked_domains_count =
    sizeof(fw_blocked_domains) / sizeof(fw_blocked_domains[0]) - 1;

inline std::string fw_get_current_time_str() {
    time_t now = time(nullptr);
    struct tm tstruct;
    char buf[32];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
    return std::string(buf);
}

inline void AddNetLog(const char* lib, const char* target, int port,
                      bool blocked, const char* reason) {
    if (!g_FirewallVerbose) return;

    std::lock_guard<std::mutex> lock(g_fwLogMutex);

    FW_LogEntry entry;
    entry.time      = fw_get_current_time_str();
    entry.libName   = (lib && strlen(lib) > 0) ? lib : "Unknown";
    entry.target    = (target) ? target : "N/A";
    entry.port      = (port > 0) ? std::to_string(port) : "DNS";
    entry.isBlocked = blocked;
    entry.reason    = (reason) ? reason : "";

    g_fwLogs.push_back(entry);
    if ((int)g_fwLogs.size() > FW_MAX_LOGS) {
        g_fwLogs.erase(g_fwLogs.begin());
    }

   
    g_FwStats.totalChecks++;
    if (blocked) {
        g_FwStats.totalBlocked++;
        if (entry.libName == "libanogs.so") g_FwStats.anogsBlocked++;
        if (strstr(reason, "BLACKLIST"))    g_FwStats.domainBlocked++;
        if (strstr(reason, "Port"))         g_FwStats.portBlocked++;
        if (strstr(reason, "Private"))      g_FwStats.privIpBlocked++;
    } else {
        g_FwStats.totalAllowed++;
    }
}

inline bool fw_is_blocked_port(int port) {
    for (int i = 0; i < fw_blocked_ports_count; i++) {
        if (port == fw_blocked_ports[i]) return true;
    }
    return false;
}

inline bool fw_is_in_blacklist(const char* node) {
    if (!node) return false;
    for (int i = 0; i < fw_blocked_domains_count; i++) {
        if (fw_blocked_domains[i] &&
            strstr(node, fw_blocked_domains[i]) != nullptr) {
            return true;
        }
    }
    return false;
}

inline bool fw_should_block_lib(const char* lib) {
    if (!lib) return false;
    const char* filename = strrchr(lib, '/');
    filename = filename ? filename + 1 : lib;

    return strcmp(filename, "libGGP.so")    == 0 ||
           strcmp(filename, "libanogs.so")  == 0 ||
           strcmp(filename, "libil2cpp.so") == 0 ||
           strcmp(filename, "libunity.so")  == 0;
}

inline bool fw_get_caller_lib(char* out_lib) {
    void* caller_addr = __builtin_return_address(0);
    Dl_info info;
    if (dladdr(caller_addr, &info) && info.dli_fname) {
        strncpy(out_lib, info.dli_fname, 255);
        out_lib[255] = '\0';
        return true;
    }
    strcpy(out_lib, "Unknown");
    return false;
}

inline bool fw_is_private_ip(const char* ip) {
    struct in_addr addr4;
    struct in6_addr addr6;

    if (inet_pton(AF_INET, ip, &addr4) == 1) {
        uint32_t ip_addr = ntohl(addr4.s_addr);
        if ((ip_addr >> 24) == 10) return true;
        if ((ip_addr >> 20) == ((172 << 4) | 1)) return true;
        if ((ip_addr >> 16) == ((192 << 8) | 168)) return true;
    }
    else if (inet_pton(AF_INET6, ip, &addr6) == 1) {
        if (ip[0] == 'f' && ip[1] == 'c') return true;
        if (ip[0] == 'f' && ip[1] == 'd') return true;
    }
    return false;
}

inline int (*fw_orig_connect)(int, const struct sockaddr*, socklen_t) = nullptr;
inline int (*fw_orig_getaddrinfo)(const char*, const char*,
                                  const struct addrinfo*,
                                  struct addrinfo**) = nullptr;
inline int (*fw_orig_socket)(int, int, int) = nullptr;

inline int fw_hooked_connect(int sockfd, const struct sockaddr* addr,
                             socklen_t addrlen) {
    if (!g_FirewallEnabled) {
        return fw_orig_connect(sockfd, addr, addrlen);
    }

    if (addr && (addr->sa_family == AF_INET || addr->sa_family == AF_INET6)) {
        char ip[INET6_ADDRSTRLEN] = {0};
        int  port = 0;

        if (addr->sa_family == AF_INET) {
            struct sockaddr_in* sin = (struct sockaddr_in*)addr;
            inet_ntop(AF_INET, &sin->sin_addr, ip, sizeof(ip));
            port = ntohs(sin->sin_port);
        } else {
            struct sockaddr_in6* sin6 = (struct sockaddr_in6*)addr;
            inet_ntop(AF_INET6, &sin6->sin6_addr, ip, sizeof(ip));
            port = ntohs(sin6->sin6_port);
        }

        
        if (strlen(g_fwAllowedIpCache) > 0 &&
            strcmp(ip, g_fwAllowedIpCache) == 0) {
            AddNetLog("Whitelist", ip, port, false, "Allowed lixuim.site");
            return fw_orig_connect(sockfd, addr, addrlen);
        }

        char libName[256];
        fw_get_caller_lib(libName);
        char* shortLib = strrchr(libName, '/');
        if (shortLib) shortLib++; else shortLib = libName;

        
        if (strcmp(shortLib, "libanogs.so") == 0) {
            AddNetLog(shortLib, ip, port, true, "STRICT BAN (Anogs)");
            errno = ECONNREFUSED;
            return -1;
        }

        if (fw_should_block_lib(libName)) {
            bool block = false;
            const char* reason = "None";

            if (fw_is_private_ip(ip)) {
                reason = "Private IP";
                block = true;
            } else if (fw_is_blocked_port(port)) {
                reason = "Blocked Port";
                block = true;
            }

            AddNetLog(shortLib, ip, port, block, block ? reason : "Allowed");

            if (block) {
                errno = ECONNREFUSED;
                return -1;
            }
        }
    }
    return fw_orig_connect(sockfd, addr, addrlen);
}

inline int fw_hooked_getaddrinfo(const char* node, const char* service,
                                 const struct addrinfo* hints,
                                 struct addrinfo** res) {
    if (!g_FirewallEnabled) {
        return fw_orig_getaddrinfo(node, service, hints, res);
    }

   
    if (node && strstr(node, FW_ALLOWED_DOMAIN) != nullptr) {
        int ret = fw_orig_getaddrinfo(node, service, hints, res);
        if (ret == 0 && *res) {
            struct sockaddr_in* sin = (struct sockaddr_in*)(*res)->ai_addr;
            if (sin) {
                inet_ntop(AF_INET, &sin->sin_addr,
                          g_fwAllowedIpCache, sizeof(g_fwAllowedIpCache));
            }
            AddNetLog("DNS", node, 0, false, "WHITELISTED");
        }
        return ret;
    }

    char libName[256];
    fw_get_caller_lib(libName);
    char* shortLib = strrchr(libName, '/');
    if (shortLib) shortLib++; else shortLib = libName;

    if (fw_should_block_lib(libName) && node) {
        if (fw_is_in_blacklist(node)) {
            AddNetLog(shortLib, node, 0, true, "BLACKLIST DOMAIN");
            return EAI_NONAME;
        }
        if (strcmp(node, "localhost") == 0 ||
            strcmp(node, "127.0.0.1") == 0) {
            AddNetLog(shortLib, node, 0, true, "LOCALHOST DNS");
            return EAI_NONAME;
        }
    }

    return fw_orig_getaddrinfo(node, service, hints, res);
}

inline int fw_hooked_socket(int domain, int type, int protocol) {
    return fw_orig_socket(domain, type, protocol);
}

inline void init_firewall() {
    void* libc = dlopen("libc.so", RTLD_NOW);
    if (!libc) {
        FW_LOGW("firewall: libc.so not found");
        return;
    }

    void* p_connect     = dlsym(libc, "connect");
    void* p_socket      = dlsym(libc, "socket");
    void* p_getaddrinfo = dlsym(libc, "getaddrinfo");

    int hooked = 0;

    if (p_connect) {
        if (DobbyHook(p_connect, (void*)fw_hooked_connect,
                      (void**)&fw_orig_connect) == RS_SUCCESS) {
            FW_LOGD("firewall: connect hooked");
            hooked++;
        }
    }
    if (p_socket) {
        if (DobbyHook(p_socket, (void*)fw_hooked_socket,
                      (void**)&fw_orig_socket) == RS_SUCCESS) {
            FW_LOGD("firewall: socket hooked");
            hooked++;
        }
    }
    if (p_getaddrinfo) {
        if (DobbyHook(p_getaddrinfo, (void*)fw_hooked_getaddrinfo,
                      (void**)&fw_orig_getaddrinfo) == RS_SUCCESS) {
            FW_LOGD("firewall: getaddrinfo hooked");
            hooked++;
        }
    }

    g_FwStats.libsHooked = hooked;
    g_FwStats.hooksOk    = (hooked >= 2);

    FW_LOGD("firewall: init done — %d hooks active", hooked);
}
#endif 