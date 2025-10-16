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

    TEST_GROUP(int_hypot, {
        int32_t sum1 = 0;
        for (int16_t x = -50; x <= 50; x++) {
            for (int16_t y = -50; y <= 50; y++) {
                sum1 += int_hypot(x, y);
            }
        }
        int32_t sum2 = 0;
        for (int16_t x = -0x400; x <= 0x400; x += 0x10) {
            for (int16_t y = -0x400; y <= 0x400; y += 0x10) {
                sum2 += int_hypot(x, y);
            }
        }
        int16_t hash1 = 0;
        for (uint16_t i = 0, state = xorshift16(42); i < 500; i++) {
            int16_t x = xorshift16(state);
            int16_t y = state = xorshift16(x);
            hash1 ^= int_hypot(x, y);
        }
        int16_t hash2 = 0;
        for (uint16_t i = 0, state = xorshift16(1991); i < 500; i++) {
            int16_t x = xorshift16(state);
            int16_t y = state = xorshift16(x);
            hash2 ^= int_hypot(x, y);
        }
        // clang-format off
        TEST(hypot_north,      int_hypot(  0,  50) == 50);
        TEST(hypot_north_east, int_hypot( 25,  25) == 35);
        TEST(hypot_east,       int_hypot( 15,   0) == 15);
        TEST(hypot_south_east, int_hypot( 15, -50) == 52);
        TEST(hypot_south,      int_hypot(  0, -26) == 26);
        TEST(hypot_south_west, int_hypot(-16, -16) == 22);
        TEST(hypot_west,       int_hypot(-20,   0) == 20);
        TEST(hypot_north_west, int_hypot(-14,  14) == 19);
        TEST(hypot_zero,       int_hypot( 0,  0) == 0);
        TEST(hypot_sum1,       sum1  == 389716);
        TEST(hypot_sum2,       sum2  == 13132424);
        TEST(hypot_hash1,      hash1 == -18957);
        TEST(hypot_hash2,      hash2 == 24307);
        #ifdef PLATFORM_DOS16
            if (g_extensive) {
                extern int16_t cdecl int_hypot_asm_(int16_t x, int16_t y);
                tprintf("    hypot_extensive...\n");
                for (int16_t x = -0x200; x <= 0x200; x++) {
                    for (int16_t y = -0x200; y <= 0x200; y++) {
                        int16_t c_res = int_hypot(x, y);
                        int16_t asm_res = int_hypot_asm_(x, y);
                        if (c_res != asm_res) {
                            tprintf("x=%d, y=%d => C=%d, ASM=%d\n", x, y, c_res, asm_res);
                        }
                    }
                }
            }
        #endif
        // clang-format on
    });

    TEST_GROUP(int_hypot_3d, {
        VECTOR v;
        int32_t sum1 = 0;
        for (int16_t x = -10; x <= 10; x++) {
            for (int16_t y = -20; y <= 20; y += 2) {
                dg_reset();
                for (int16_t z = -30; z <= 30; z += 3) {
                    v.x = x;
                    v.y = y;
                    v.z = z;
                    sum1 += int_hypot_3d(VEC(&v));
                }
            }
        }
        int32_t sum2 = 0;
        for (int16_t x = -0x400; x <= 0x400; x += 0x20) {
            for (int16_t y = -0x400; y <= 0x400; y += 0x20) {
                dg_reset();
                for (int16_t z = 0x400; z <= 0x400; z += 0x20) {
                    v.x = x;
                    v.y = y;
                    v.z = z;
                    sum2 += int_hypot_3d(VEC(&v));
                }
            }
        }
        int16_t hash1 = 0;
        for (uint16_t i = 0, state = xorshift16(0x42); i < 500; i++) {
            dg_reset();
            v.x = xorshift16(state);
            v.y = xorshift16(v.x);
            v.z = state = xorshift16(v.y);
            hash1 ^= int_hypot_3d(VEC(&v));
        }
        int16_t hash2 = 0;
        for (uint16_t i = 0, state = xorshift16(0x1991); i < 500; i++) {
            dg_reset();
            v.x = xorshift16(state);
            v.y = xorshift16(v.x);
            v.z = state = xorshift16(v.y);
            hash2 ^= int_hypot_3d(VEC(&v));
        }
        // clang-format off
        VECTOR v1 = { -2, 6, -6 };
        TEST(hypot3d_1,     int_hypot_3d(VEC(&v1)) == 8);
        VECTOR v2 = { -6, 8, 24 };
        TEST(hypot3d_2,     int_hypot_3d(VEC(&v2)) == 26);
        VECTOR v3 = { 1, 4, 3 };
        TEST(hypot3d_3,     int_hypot_3d(VEC(&v3)) == 5);
        VECTOR v4 = { 4, -14, -21 };
        TEST(hypot3d_4,     int_hypot_3d(VEC(&v4)) == 25);
        VECTOR v5 = { 5, 6, -12 };
        TEST(hypot3d_5,     int_hypot_3d(VEC(&v5)) == 13);
        VECTOR zero = { 0, 0, 0 };
        TEST(hypot3d_zero,  int_hypot_3d(VEC(&zero)) == 0);
        TEST(hypot3d_sum1,  sum1  == 191612);
        TEST(hypot3d_sum2,  sum2  == 5570928);
        TEST(hypot3d_hash1, hash1 == -16894);
        TEST(hypot3d_hash2, hash2 == 8836);
        #ifdef PLATFORM_DOS16
            if (g_extensive) {
                extern int16_t cdecl int_hypot_3d_asm_(const VECTOR* vec);
                tprintf("    hypot_3d_extensive...\n");
                for (int16_t x = -0x20; x <= 0x20; x++) {
                    for (int16_t y = -0x20; y <= 0x20; y++) {
                        for (int16_t z = -0x20; z <= 0x20; z++) {
                            dg_reset();
                            v.x = x;
                            v.y = y;
                            v.z = z;
                            int16_t c_res = int_hypot_3d(VEC(&v));
                            int16_t asm_res = int_hypot_3d_asm_(VEC(&v));
                            if (c_res != asm_res) {
                                tprintf("x=%d, y=%d, z=%d => C=%d, ASM=%d\n", x, y, z, c_res, asm_res);
                            }
                        }
                    }
                }
            }
        #endif
        // clang-format on
    });
}
