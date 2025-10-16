#pragma once

#include <restunts.h>
#include <stdarg.h>

#ifndef PLATFORM_STR
    #define PLATFORM_STR "unknown"
#endif

#ifndef DATE
    #define DATE "unknown"
#endif

#define _STR(x) #x

// When linking with the original Stunts code, we steal a memory location in
// the original data segment to put strings and other objects we need to pass
// to Stunts' functions as near pointers. This allows us to store data in the
// unit test segments that wouldn't otherwise fit in DGROUP. For each test we
// just copy over what is needed, then reset the buffer.
//
// The _(...) macro overloading trick let us do _("text") for strings and
// _(dataptr, datasize) for any other types.
#define _EXPAND(x) x

#define _GET_OVERLOAD(_1, _2, name, ...) name

#define _(...) _EXPAND(_GET_OVERLOAD(__VA_ARGS__, dg_ptr, dg_str)(__VA_ARGS__))

#ifdef PLATFORM_DOS16 // clang-format off
    #include <dos.h>
    #include <externs.h>

    extern uint8_t far* g_dg_buf;
    extern unsigned g_dg_buf_idx;

    // Set the DGROUP location we are using as a buffer and reset the offset.
    static inline void dg_reset()
    {
        g_dg_buf = (uint8_t far*)&audiotimers;
        g_dg_buf_idx = 0;
    }

    // Copy a null-terminated string to the buffer in DGROUP and return a near
    // pointer to it for use with DS.
    static inline char near* dg_str(const char* str)
    {
        int i = 0;
        do {
            g_dg_buf[g_dg_buf_idx + i] = str[i];
        } while (str[i++]);

        char* result = g_dg_buf + g_dg_buf_idx;
        g_dg_buf_idx += i;

        return (char near*)FP_OFF(result);
    }

    // Copy arbitrary bytes to the buffer in DGROUP.
    static inline void near* dg_ptr(const void* ptr, unsigned size)
    {
        int i;
        for (i = 0; i < size; i++) {
            g_dg_buf[g_dg_buf_idx + i] = ((const uint8_t*)ptr)[i];
        }

        void* result = g_dg_buf + g_dg_buf_idx;
        g_dg_buf_idx += i;

        return (void near*)FP_OFF(result);
    }

    // printf() convenience that copies the formatting string to DGROUP.
    #define tprintf(fmt, ...) printf(dg_str(fmt), ##__VA_ARGS__)

    // printf() in Microsoft's CRT used by Stunts accepts the "%Fs" formatting
    // string for `char far*`, so we use the FSTR(options) macro to pass
    // such parameters without having to copy that data over to DGROUP.
    #define FSTR(...) "%" _STR(__VA_ARGS__) "Fs"
#else
    #include <stdio.h>
    #include <strings.h>
    #define stricmp  strcasecmp
    #define testmain main

    // On any other platforms we just skip the whole memory song and dance.
    #define dg_reset()
    #define dg_str(str) str
    #define dg_ptr(ptr, size) ptr

    #define tprintf(fmt, ...) printf(fmt, ##__VA_ARGS__)

    #define FSTR(...) "%" _STR(__VA_ARGS__) "s"
#endif // clang-format on

// Conversion conveniences.
#define VEC(arg) _(arg, sizeof(VECTOR))

// dosemu, DOSBox and Windows 10 all support ANSI colours.
#define RED   "\033[31m"
#define GREEN "\033[32m"
#define DIM   "\033[2m"
#define RESET "\033[0m"

#define PRINT_COLOR(color, format, ...) \
    tprintf(color format RESET, ##__VA_ARGS__)

typedef struct {
    int pass;
    int fail;
} test_result_t;

extern test_result_t g_total_tests;
extern test_result_t g_total_groups;
extern bool g_verbose;
extern bool g_extensive;

#define TEST(name, expr)                                                \
    do {                                                                \
        dg_reset();                                                     \
        if (g_verbose) {                                                \
            tprintf("    " FSTR(-20), #name);                           \
        }                                                               \
        if (!(expr)) {                                                  \
            res.fail++;                                                 \
            if (g_verbose) {                                            \
                PRINT_COLOR(RED, "FAIL");                               \
                PRINT_COLOR(                                            \
                        DIM, "  " FSTR() ":%d " FSTR() RESET, __FILE__, \
                        __LINE__, #expr                                 \
                );                                                      \
                tprintf("\n");                                          \
            }                                                           \
        }                                                               \
        else {                                                          \
            res.pass++;                                                 \
            if (g_verbose) {                                            \
                PRINT_COLOR(GREEN, "PASS");                             \
                tprintf("\n");                                          \
            }                                                           \
        }                                                               \
    } while (0)

#define TEST_GROUP(name, ...)                              \
    do {                                                    \
        dg_reset();                                         \
        test_result_t res = { 0, 0 };                       \
        if (g_verbose) {                                    \
            PRINT_COLOR(DIM, "  " FSTR() "\n", #name);      \
        }                                                   \
        else {                                              \
            tprintf("  " FSTR(-20), #name);                 \
        }                                                   \
                                                            \
        __VA_ARGS__;                                        \
                                                            \
        if (g_verbose) {                                    \
            print_result(&res, "                        "); \
        }                                                   \
        else {                                              \
            if (res.fail) {                                 \
                PRINT_COLOR(RED, "FAIL");                   \
            }                                               \
            else {                                          \
                PRINT_COLOR(GREEN, "PASS");                 \
            }                                               \
            tprintf("\n");                                  \
        }                                                   \
        g_total_tests.pass += res.pass;                     \
        g_total_tests.fail += res.fail;                     \
        if (res.fail) {                                     \
            g_total_groups.fail++;                          \
        }                                                   \
        else {                                              \
            g_total_groups.pass++;                          \
        }                                                   \
    } while (0)

// Very simple deterministic RNGs. The shift constants were picked by searching
// GitHub for xorshift16 and xorshift32, then picking the values I saw
// recurring, counting on the wisdom of the crowd.
static inline uint16_t xorshift16(uint16_t x)
{
    x ^= x << 7;
    x ^= x >> 9;
    x ^= x << 8;
    return x;
}

static inline uint32_t xorshift32(uint32_t x)
{
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x >> 5;
    return x;
}

void print_result(test_result_t* res, const char* indent);
void test_math();
