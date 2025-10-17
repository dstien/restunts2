#pragma once

#include "restunts.h"

#pragma pack(push, 1)
typedef struct VECTOR {
    int16_t x, y, z;
} VECTOR;

typedef struct MATRIX {
    union {
        int16_t vals[9];
        struct {
            int16_t m11, m21, m31;
            int16_t m12, m22, m32;
            int16_t m13, m23, m33;
        };
    };
} MATRIX;
#pragma pack(pop)

int16_t cdecl int_sin(uint16_t angle);
int16_t cdecl int_cos(uint16_t angle);
int16_t cdecl int_atan2(int16_t x, int16_t y);
int16_t cdecl int_hypot(int16_t x, int16_t y);
int16_t cdecl int_hypot_3d(const VECTOR near* vec);

void cdecl vec_transform(const VECTOR near* src, const MATRIX near* mat, VECTOR near* dst);
