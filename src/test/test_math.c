#include "math.h"
#include "test.h"

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
        // clang-format on
    });

    TEST_GROUP(int_atan2, {
        uint32_t sum1 = 0;
        for (int x = -13, y = 13; x < 13; x++, y--) {
            sum1 += int_atan2(x, y);
        }
        uint32_t sum2 = 0;
        for (int x = -1337, y = 0xC0DE; x < 1337; x++, y--) {
            sum2 += int_atan2(x, y);
        }
        // clang-format off
        TEST(atan2_north,      int_atan2( 0,  1) == 0);
        TEST(atan2_north_east, int_atan2( 1,  1) == 0x80);
        TEST(atan2_east,       int_atan2( 1,  0) == 0x100);
        TEST(atan2_south_east, int_atan2( 1, -1) == 0x180);
        TEST(atan2_south,      int_atan2( 0, -1) == 0x200);
        TEST(atan2_south_west, int_atan2(-1, -1) == -0x180);
        TEST(atan2_west,       int_atan2(-1,  0) == -0x100);
        TEST(atan2_north_west, int_atan2(-1,  1) == -0x080);
        TEST(atan2_zero,       int_atan2( 0,  0) == 0);
        TEST(atan2_big,        int_atan2(0x4000, 0x4000)         == 0x80);
        TEST(atan2_edge1,      int_atan2(0x200  - 1, 0x200)      == 0x80);
        TEST(atan2_edge2,      int_atan2(0x4000 - 1, 0x4000)     == 0x80);
        TEST(atan2_edge3,      int_atan2(0x8000 - 2, 0x8000 - 1) == 0x80);

        TEST(atan2_sum1,       sum1 == 2944);
        TEST(atan2_sum2,       sum2 == 847);
        // clang-format on
    });
}
