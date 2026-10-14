#pragma once
// ============================================================================
//  SHADOW MODZ - ACCESS KEY (in-game gate)
// ----------------------------------------------------------------------------
//  The floating menu shows a lock screen the first time it is injected into
//  the game: the feature tabs only exist after the access key has been typed
//  on the built-in keypad.  The key is verified with SHA-256 against a stored
//  digest - the key itself is never kept in the binary, exactly like the
//  launcher's Java gate (same salt, same digest).
//
//  Once a key has been accepted the mod remembers it, so the menu opens
//  directly on the feature tabs the next time (SYSTEM -> LOCK MENU).
// ============================================================================

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cctype>

namespace ShadowAuth
{

// ---------------------------------------------------------------------------
//  SHA-256 (self contained - no external dependency, no allocation)
// ---------------------------------------------------------------------------
struct Sha256
{
    uint32_t h[8];
    uint8_t  buf[64];
    uint64_t len;
    int      bufLen;

    static uint32_t Ror(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }

    void Init()
    {
        static const uint32_t k[8] =
        {
            0x6a09e667u, 0xbb67ae85u, 0x3c6ef372u, 0xa54ff53au,
            0x510e527fu, 0x9b05688cu, 0x1f83d9abu, 0x5be0cd19u
        };
        for (int i = 0; i < 8; ++i) h[i] = k[i];
        len = 0;
        bufLen = 0;
    }

    void Block(const uint8_t* p)
    {
        static const uint32_t k[64] =
        {
            0x428a2f98u,0x71374491u,0xb5c0fbcfu,0xe9b5dba5u,0x3956c25bu,0x59f111f1u,0x923f82a4u,0xab1c5ed5u,
            0xd807aa98u,0x12835b01u,0x243185beu,0x550c7dc3u,0x72be5d74u,0x80deb1feu,0x9bdc06a7u,0xc19bf174u,
            0xe49b69c1u,0xefbe4786u,0x0fc19dc6u,0x240ca1ccu,0x2de92c6fu,0x4a7484aau,0x5cb0a9dcu,0x76f988dau,
            0x983e5152u,0xa831c66du,0xb00327c8u,0xbf597fc7u,0xc6e00bf3u,0xd5a79147u,0x06ca6351u,0x14292967u,
            0x27b70a85u,0x2e1b2138u,0x4d2c6dfcu,0x53380d13u,0x650a7354u,0x766a0abbu,0x81c2c92eu,0x92722c85u,
            0xa2bfe8a1u,0xa81a664bu,0xc24b8b70u,0xc76c51a3u,0xd192e819u,0xd6990624u,0xf40e3585u,0x106aa070u,
            0x19a4c116u,0x1e376c08u,0x2748774cu,0x34b0bcb5u,0x391c0cb3u,0x4ed8aa4au,0x5b9cca4fu,0x682e6ff3u,
            0x748f82eeu,0x78a5636fu,0x84c87814u,0x8cc70208u,0x90befffau,0xa4506cebu,0xbef9a3f7u,0xc67178f2u
        };

        uint32_t w[64];
        for (int i = 0; i < 16; ++i)
            w[i] = ((uint32_t)p[i * 4] << 24) | ((uint32_t)p[i * 4 + 1] << 16) |
                   ((uint32_t)p[i * 4 + 2] << 8) | (uint32_t)p[i * 4 + 3];

        for (int i = 16; i < 64; ++i)
        {
            uint32_t s0 = Ror(w[i - 15], 7) ^ Ror(w[i - 15], 18) ^ (w[i - 15] >> 3);
            uint32_t s1 = Ror(w[i - 2], 17) ^ Ror(w[i - 2], 19) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }

        uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
        uint32_t e = h[4], f = h[5], g = h[6], hh = h[7];

        for (int i = 0; i < 64; ++i)
        {
            uint32_t S1 = Ror(e, 6) ^ Ror(e, 11) ^ Ror(e, 25);
            uint32_t ch = (e & f) ^ ((~e) & g);
            uint32_t t1 = hh + S1 + ch + k[i] + w[i];
            uint32_t S0 = Ror(a, 2) ^ Ror(a, 13) ^ Ror(a, 22);
            uint32_t mj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t t2 = S0 + mj;

            hh = g; g = f; f = e; e = d + t1;
            d = c; c = b; b = a; a = t1 + t2;
        }

        h[0] += a; h[1] += b; h[2] += c; h[3] += d;
        h[4] += e; h[5] += f; h[6] += g; h[7] += hh;
    }

    void Update(const uint8_t* data, int size)
    {
        len += (uint64_t)size * 8u;
        while (size > 0)
        {
            int take = 64 - bufLen;
            if (take > size) take = size;
            memcpy(buf + bufLen, data, (size_t)take);
            bufLen += take;
            data   += take;
            size   -= take;
            if (bufLen == 64) { Block(buf); bufLen = 0; }
        }
    }

    // out must hold 65 bytes
    void FinalHex(char* out)
    {
        static const char* hex = "0123456789abcdef";
        uint64_t bits = len;
        uint8_t pad = 0x80;
        Update(&pad, 1);
        uint8_t zero = 0;
        while (bufLen != 56) Update(&zero, 1);
        uint8_t sz[8];
        for (int i = 0; i < 8; ++i) sz[i] = (uint8_t)((bits >> (56 - i * 8)) & 0xFF);
        Update(sz, 8);

        for (int i = 0; i < 8; ++i)
            for (int b = 0; b < 4; ++b)
            {
                uint8_t v = (uint8_t)((h[i] >> (24 - b * 8)) & 0xFF);
                *out++ = hex[(v >> 4) & 0xF];
                *out++ = hex[v & 0xF];
            }
        *out = 0;
    }
};

inline void Sha256Hex(const char* data, int size, char out[65])
{
    Sha256 s;
    s.Init();
    s.Update((const uint8_t*)data, size);
    s.FinalHex(out);
}

// ---------------------------------------------------------------------------
//  the key
// ---------------------------------------------------------------------------
// digest of  sha256("SHADOWMODZ::v4::GATE" + "SHADOWMODZ") - identical to the
// launcher's Java gate, so one key unlocks both.
inline const char* kSalt    = "SHADOWMODZ::v4::GATE";
inline const char* kKeyHash = "ee6370ffb4f3e26df4a4b4f02a74778dfc840cb1903ff312ac31f00b120ac3df";

inline const int kKeyMax    = 24;    // typed key buffer (menu keypad)

inline bool unlocked        = false; // the gate state the menu reads
inline bool autoAccepted    = false; // set when a remembered key was found

// uppercase + drop everything that is not A-Z / 0-9
inline void Normalize(const char* in, char* out, int cap)
{
    int n = 0;
    for (const char* p = in; p && *p && n < cap - 1; ++p)
    {
        unsigned char ch = (unsigned char)*p;
        if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '-') continue;
        char up = (char)toupper(ch);
        if ((up >= 'A' && up <= 'Z') || (up >= '0' && up <= '9'))
            out[n++] = up;
    }
    out[n] = 0;
}

// constant time compare, so a wrong key cannot be brute forced by timing
inline bool SameHash(const char* a, const char* b)
{
    unsigned char diff = 0;
    for (int i = 0; i < 64; ++i) diff |= (unsigned char)(a[i] ^ b[i]);
    return diff == 0;
}

inline bool Verify(const char* key)
{
    if (!key || !key[0]) return false;
    char norm[kKeyMax * 2 + 1];
    Normalize(key, norm, (int)sizeof(norm));
    if (!norm[0]) return false;

    char data[128];
    snprintf(data, sizeof(data), "%s%s", kSalt, norm);

    char hash[65];
    Sha256Hex(data, (int)strlen(data), hash);
    return SameHash(hash, kKeyHash);
}

// ---------------------------------------------------------------------------
//  "remember the key" token -  SMDZ-<first 12 hex of the digest>-OK
// ---------------------------------------------------------------------------
inline void Token(char* out, int cap)
{
    snprintf(out, cap, "SMDZ-%.12s-OK", kKeyHash);
}

inline bool SaveUnlock()
{
    static const char* paths[] =
    {
        "/data/data/com.dts.freefireth/files/.smdz.cfg",
        "/data/user/0/com.dts.freefireth/files/.smdz.cfg",
        "/sdcard/Android/data/com.dts.freefireth/files/.smdz.cfg",
        "/sdcard/.smdz.cfg",
        "/data/local/tmp/.smdz.cfg",
    };

    char token[48];
    Token(token, sizeof(token));

    for (size_t i = 0; i < sizeof(paths) / sizeof(paths[0]); ++i)
    {
        FILE* f = fopen(paths[i], "wb");
        if (!f) continue;
        size_t n = fwrite(token, 1, strlen(token), f);
        fclose(f);
        if (n == strlen(token)) return true;
    }
    return false;
}

inline bool LoadUnlock()
{
    static const char* paths[] =
    {
        "/data/data/com.dts.freefireth/files/.smdz.cfg",
        "/data/user/0/com.dts.freefireth/files/.smdz.cfg",
        "/sdcard/Android/data/com.dts.freefireth/files/.smdz.cfg",
        "/sdcard/.smdz.cfg",
        "/data/local/tmp/.smdz.cfg",
    };

    char want[48];
    Token(want, sizeof(want));

    for (size_t i = 0; i < sizeof(paths) / sizeof(paths[0]); ++i)
    {
        FILE* f = fopen(paths[i], "rb");
        if (!f) continue;
        char got[64] = { 0 };
        size_t n = fread(got, 1, sizeof(got) - 1, f);
        fclose(f);
        got[n] = 0;
        if (n > 0 && strcmp(got, want) == 0) { autoAccepted = true; return true; }
    }
    return false;
}

} // namespace ShadowAuth
