#include "math.h"
#include "test.h"

void test_math()
{
    TEST_GROUP(sin_fast, {
        uint32_t sum1 = 0;
        for (int a = 0; a <= 0x100; a++) {
            sum1 += sin_fast(a);
        }
        uint32_t sum2 = 0;
        for (int a = -1337; a < 1337; a++) {
            sum2 += sin_fast(a);
        }
        // clang-format off
        TEST(sin_0,             sin_fast(0)      == 0);
        TEST(sin_quarter,       sin_fast(0x100)  == 0x4000);
        TEST(sin_half,          sin_fast(0x200)  == 0);
        TEST(sin_third_quarter, sin_fast(0x300)  == -0x4000);
        TEST(sin_full,          sin_fast(0x400)  == 0);
        TEST(sin_wrap0,         sin_fast(0x00DE) == 16029);
        TEST(sin_wrap1,         sin_fast(0xC0DE) == 16029);
        TEST(sin_sum1,          sum1             == 2678373);
        TEST(sin_sum2,          sum2             == -15392);
        // clang-format on
    });

    TEST_GROUP(cos_fast, {
        uint32_t sum1 = 0;
        for (int a = 13; a < 37; a++) {
            sum1 += cos_fast(a);
        }
        uint32_t sum2 = 0;
        for (int a = -13; a < 137; a++) {
            sum2 += cos_fast(a);
        }
        // clang-format off
        TEST(cos_0,             cos_fast(0)      == 0x4000);
        TEST(cos_quarter,       cos_fast(0x100)  == 0);
        TEST(cos_half,          cos_fast(0x200)  == -0x4000);
        TEST(cos_third_quarter, cos_fast(0x300)  == 0);
        TEST(cos_full,          cos_fast(0x400)  == 0x4000);
        TEST(cos_wrap0,         cos_fast(0x000D) == 16332);
        TEST(cos_wrap1,         cos_fast(0xF00D) == 16332);
        TEST(cos_sum1,          sum1             == 388432);
        TEST(cos_sum2,          sum2             == 2204904);
        // clang-format on
    });
}
