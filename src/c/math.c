#include "math.h"

#ifdef ASSERT_UNDEFINED
    #include "externs.h"
#endif

// Quarter-wave lookup table for sin_fast() in 0x100 steps + the peak endpoint.
// The value for a given angle is obtained by getting the index from the lower
// 8 bits, the quadrant from the next 2 bits, and the remaining bits can be
// discarded as the sine wraps around every 0x400 steps. The values are scaled
// by 0x4000.
#define SIN_STEPS           0x100
#define SIN_GET_STEP(a)     (a & 0xFF)
#define SIN_GET_QUADRANT(a) ((a >> 8) & 3)
static int16_t sine_table[] = {
    0,     101,   201,   302,   402,   503,   603,   704,   804,   904,   1005,
    1105,  1205,  1306,  1406,  1506,  1606,  1706,  1806,  1906,  2006,  2105,
    2205,  2305,  2404,  2503,  2603,  2702,  2801,  2900,  2999,  3098,  3196,
    3295,  3393,  3492,  3590,  3688,  3786,  3883,  3981,  4078,  4176,  4273,
    4370,  4467,  4563,  4660,  4756,  4852,  4948,  5044,  5139,  5235,  5330,
    5425,  5520,  5614,  5708,  5803,  5897,  5990,  6084,  6177,  6270,  6363,
    6455,  6547,  6639,  6731,  6823,  6914,  7005,  7096,  7186,  7276,  7366,
    7456,  7545,  7635,  7723,  7812,  7900,  7988,  8076,  8163,  8250,  8337,
    8423,  8509,  8595,  8680,  8765,  8850,  8935,  9019,  9102,  9186,  9269,
    9352,  9434,  9516,  9598,  9679,  9760,  9841,  9921,  10001, 10080, 10159,
    10238, 10316, 10394, 10471, 10549, 10625, 10702, 10778, 10853, 10928, 11003,
    11077, 11151, 11224, 11297, 11370, 11442, 11514, 11585, 11656, 11727, 11797,
    11866, 11935, 12004, 12072, 12140, 12207, 12274, 12340, 12406, 12472, 12537,
    12601, 12665, 12729, 12792, 12854, 12916, 12978, 13039, 13100, 13160, 13219,
    13279, 13337, 13395, 13453, 13510, 13567, 13623, 13678, 13733, 13788, 13842,
    13896, 13949, 14001, 14053, 14104, 14155, 14206, 14256, 14305, 14354, 14402,
    14449, 14497, 14543, 14589, 14635, 14680, 14724, 14768, 14811, 14854, 14896,
    14937, 14978, 15019, 15059, 15098, 15137, 15175, 15213, 15250, 15286, 15322,
    15357, 15392, 15426, 15460, 15493, 15525, 15557, 15588, 15619, 15649, 15679,
    15707, 15736, 15763, 15791, 15817, 15843, 15868, 15893, 15917, 15941, 15964,
    15986, 16008, 16029, 16049, 16069, 16088, 16107, 16125, 16143, 16160, 16176,
    16192, 16207, 16221, 16235, 16248, 16261, 16273, 16284, 16295, 16305, 16315,
    16324, 16332, 16340, 16347, 16353, 16359, 16364, 16369, 16373, 16376, 16379,
    16381, 16383, 16384, 16384
};

// Integer sin() scaled by 0x4000 using LUT.
int16_t cdecl int_sin(uint16_t angle)
{
    uint8_t step = SIN_GET_STEP(angle);
    switch (SIN_GET_QUADRANT(angle)) {
        case 0:
            return sine_table[step];
        case 1:
            return sine_table[SIN_STEPS - step];
        case 2:
            return -sine_table[step];
        case 3:
        default:
            return -sine_table[SIN_STEPS - step];
    }
}

// Integer cos() scaled by 0x4000 using LUT.
int16_t cdecl int_cos(uint16_t angle)
{
    return int_sin(angle + SIN_STEPS);
}

// One-octant (45°) lookup table of the arctangent in 0x100 steps + the peak
// endpoint. The polar angle for a given coordinate is found by determining
// the octant, normalise the coordinates to be in the first octant, get the
// angle at index x/y and rotate it back to the actual octant.
static uint8_t atan_table[] = {
    0,   1,   1,   2,   3,   3,   4,   4,   5,   6,   6,   7,   8,   8,   9,
    10,  10,  11,  11,  12,  13,  13,  14,  15,  15,  16,  16,  17,  18,  18,
    19,  20,  20,  21,  22,  22,  23,  23,  24,  25,  25,  26,  27,  27,  28,
    28,  29,  30,  30,  31,  31,  32,  33,  33,  34,  34,  35,  36,  36,  37,
    38,  38,  39,  39,  40,  41,  41,  42,  42,  43,  44,  44,  45,  45,  46,
    46,  47,  48,  48,  49,  49,  50,  51,  51,  52,  52,  53,  53,  54,  55,
    55,  56,  56,  57,  57,  58,  58,  59,  60,  60,  61,  61,  62,  62,  63,
    63,  64,  65,  65,  66,  66,  67,  67,  68,  68,  69,  69,  70,  70,  71,
    71,  72,  72,  73,  74,  74,  75,  75,  76,  76,  77,  77,  78,  78,  79,
    79,  80,  80,  81,  81,  82,  82,  83,  83,  84,  84,  84,  85,  85,  86,
    86,  87,  87,  88,  88,  89,  89,  90,  90,  91,  91,  91,  92,  92,  93,
    93,  94,  94,  95,  95,  96,  96,  96,  97,  97,  98,  98,  99,  99,  99,
    100, 100, 101, 101, 102, 102, 102, 103, 103, 104, 104, 104, 105, 105, 106,
    106, 106, 107, 107, 108, 108, 108, 109, 109, 110, 110, 110, 111, 111, 112,
    112, 112, 113, 113, 113, 114, 114, 115, 115, 115, 116, 116, 116, 117, 117,
    118, 118, 118, 119, 119, 119, 120, 120, 120, 121, 121, 121, 122, 122, 122,
    123, 123, 123, 124, 124, 124, 125, 125, 125, 126, 126, 126, 127, 127, 127,
    128, 128
};

// Integer atan2() for the polar angle in the range -0x1FF to 0x200 using LUT.
int16_t cdecl int_atan2(int16_t x, int16_t y)
{
    // The function can return without setting a return value, so we need to
    // track whether the initial AX value is unexpected in these cases.
#ifdef ASSERT_UNDEFINED
    #pragma warning 200 9
    int16_t ax_value;
    __asm mov ax_value, ax;
#endif

    // The original code is handwritten assembly that picks the octant using a
    // set of flags whose sum corresponds to an index in a jump table of word
    // sized offsets. We just get the numeric octant to handle in a switch
    // statement.
    uint8_t octant = 0;
    int16_t result;

    // Fold coordinates to first quadrant.
    if (x < 0) {
        octant += 4; // Flip Y-axis (4 octants, 180°).
        x = -x;
    }
    if (y < 0) {
        octant += 2; // Flip X-axis (2 octants, 90°).
        y = -y;
    }

    // Skip division and lookup when coordinates are symmetric.
    if (x == y) {
        // atan(0/0) is undefined, and so is the original code's return value,
        // although in practice it is always 0.
        if (x == 0) {
#ifdef ASSERT_UNDEFINED
            if (ax_value != 0) {
                fatal_error("atan(0/0) AX=%d\n", ax_value);
            }
#endif
            return 0;
        }
        // atan(1/1) is π/4.
        else {
            result = 0x80; // 45° diagonal.
        }
    }
    else {
        // Fold coordinates to first octant.
        if (x > y) {
            int16_t tmp = x;
            x = y;
            y = tmp;
            octant += 1; // Flip diagonal (1 octant, 45°).
        }
        uint32_t quotient = ((uint32_t)x << 16) / y;
        // Round up the quotient by adding the max value and shift down to
        // clamp the index to 0..257.
        uint16_t step = (quotient + 0x80) >> 8;
        result = atan_table[step];
    }

    // Retun angle unfolded to its octant.
    switch (octant) {
        case 0:
            return result;
        case 1:
            return -result + 0x100;
        case 2:
            return -result + 0x200;
        case 3:
            return result + 0x100;
        case 4:
            return -result;
        case 5:
            return result - 0x100;
        case 6:
            return result - 0x200;
        case 7:
        default:
            return -(result + 0x100);
    }
}
