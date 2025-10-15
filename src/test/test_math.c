#include <math.h>
#include "test.h"

// Shim for calling int_atan2 with AX = 0 on DOS16. The original implementation
// has undefined behaviour that depend on AX being 0. Since the DOS16 build of
// these tests use large memory model, AX is set to DS.
int16_t cdecl call_int_atan2(int16_t x, int16_t y);
int16_t cdecl call_int_atan2_asm_(int16_t x, int16_t y);
extern int16_t cdecl int_atan2_asm_(int16_t x, int16_t y);
#ifdef PLATFORM_DOS16
    #pragma aux call_int_atan2 = \
            "push cx"            \
            "push bx"            \
            "xor ax, ax"         \
            "call int_atan2"     \
            "add sp, 4" parm caller[bx][cx] modify exact[ax];
    #pragma aux call_int_atan2_asm_ = \
            "push cx"                 \
            "push bx"                 \
            "xor ax, ax"              \
            "call int_atan2_asm_"     \
            "add sp, 4" parm caller[bx][cx] modify exact[ax];
#else
    #define call_int_atan2 int_atan2
#endif

void test_math()
{
    TEST_GROUP(int_sin, {
        uint32_t sum1 = 0;
        for (int a = 0; a <= 0x100; a++) {
            sum1 += int_sin(a);
        }
        uint32_t sum2 = 0;
        for (int a = -1337; a < 1337; a++) {
            sum2 += int_sin(a);
        }
        // clang-format off
        TEST(sin_0,             int_sin(0)      == 0);
        TEST(sin_quarter,       int_sin(0x100)  == 0x4000);
        TEST(sin_half,          int_sin(0x200)  == 0);
        TEST(sin_third_quarter, int_sin(0x300)  == -0x4000);
        TEST(sin_full,          int_sin(0x400)  == 0);
        TEST(sin_wrap0,         int_sin(0x00DE) == 16029);
        TEST(sin_wrap1,         int_sin(0xC0DE) == 16029);
        TEST(sin_sum1,          sum1            == 2678373);
        TEST(sin_sum2,          sum2            == -15392);

        #ifdef PLATFORM_DOS16
            if (g_extensive) {
                extern int16_t cdecl int_sin_asm_(uint16_t angle);
                tprintf("    sin_extensive...\n");
                for (uint16_t a = 0; a < UINT16_MAX; a++) {
                    int16_t c_res = int_sin(a);
                    int16_t asm_res = int_sin_asm_(a);
                    if (c_res != asm_res) {
                        tprintf("a=%d => C=%d, ASM=%d\n", a, c_res, asm_res);
                    }
                }
            }
        #endif
        // clang-format on
    });

    TEST_GROUP(int_cos, {
        uint32_t sum1 = 0;
        for (int a = 13; a < 37; a++) {
            sum1 += int_cos(a);
        }
        uint32_t sum2 = 0;
        for (int a = -13; a < 137; a++) {
            sum2 += int_cos(a);
        }
        // clang-format off
        TEST(cos_0,             int_cos(0)      == 0x4000);
        TEST(cos_quarter,       int_cos(0x100)  == 0);
        TEST(cos_half,          int_cos(0x200)  == -0x4000);
        TEST(cos_third_quarter, int_cos(0x300)  == 0);
        TEST(cos_full,          int_cos(0x400)  == 0x4000);
        TEST(cos_wrap0,         int_cos(0x000D) == 16332);
        TEST(cos_wrap1,         int_cos(0xF00D) == 16332);
        TEST(cos_sum1,          sum1            == 388432);
        TEST(cos_sum2,          sum2            == 2204904);

        #ifdef PLATFORM_DOS16
            if (g_extensive) {
                extern int16_t cdecl int_cos_asm_(uint16_t angle);
                tprintf("    cos_extensive...\n");
                for (uint16_t a = 0; a < UINT16_MAX; a++) {
                    int16_t c_res = int_cos(a);
                    int16_t asm_res = int_cos_asm_(a);
                    if (c_res != asm_res) {
                        tprintf("a=%d => C=%d, ASM=%d\n", a, c_res, asm_res);
                    }
                }
            }
        #endif
        // clang-format on
    });

    TEST_GROUP(int_atan2, {
        uint32_t sum1 = 0;
        for (int16_t x = -13, y = 13; x < 13; x++, y--) {
            sum1 += call_int_atan2(x, y);
        }
        uint32_t sum2 = 0;
        for (int16_t x = -1337, y = 0xC0DE; x < 1337; x++, y--) {
            sum2 += call_int_atan2(x, y);
        }
        // clang-format off
        TEST(atan2_north,      call_int_atan2( 0,  1) == 0);
        TEST(atan2_north_east, call_int_atan2( 1,  1) == 0x80);
        TEST(atan2_east,       call_int_atan2( 1,  0) == 0x100);
        TEST(atan2_south_east, call_int_atan2( 1, -1) == 0x180);
        TEST(atan2_south,      call_int_atan2( 0, -1) == 0x200);
        TEST(atan2_south_west, call_int_atan2(-1, -1) == -0x180);
        TEST(atan2_west,       call_int_atan2(-1,  0) == -0x100);
        TEST(atan2_north_west, call_int_atan2(-1,  1) == -0x080);
        TEST(atan2_zero,       call_int_atan2( 0,  0) == 0);
        TEST(atan2_big,        call_int_atan2(0x4000, 0x4000)         == 0x80);
        TEST(atan2_edge1,      call_int_atan2(0x200  - 1, 0x200)      == 0x80);
        TEST(atan2_edge2,      call_int_atan2(0x4000 - 1, 0x4000)     == 0x80);
        TEST(atan2_edge3,      call_int_atan2(0x8000 - 2, 0x8000 - 1) == 0x80);

        TEST(atan2_sum1,       sum1 == 2944);
        TEST(atan2_sum2,       sum2 == 847);

        #ifdef PLATFORM_DOS16
            if (g_extensive) {
                extern int16_t cdecl int_atan2_asm_(int16_t x, int16_t y);
                tprintf("    atan2_extensive...\n");
                for (int16_t x = -0x400; x <= 0x400; x++) {
                    for (int16_t y = -0x400; y <= 0x400; y++) {
                        int16_t c_res = call_int_atan2(x, y);
                        int16_t asm_res = call_int_atan2_asm_(x, y);
                        if (c_res != asm_res) {
                            tprintf("x=%d, y=%d => C=%d, ASM=%d\n", x, y, c_res, asm_res);
                        }
                    }
                }
            }
        #endif
        // clang-format on
    });
}
