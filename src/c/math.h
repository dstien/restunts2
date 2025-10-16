#pragma once

#include "restunts.h"

#pragma pack(push, 1)
typedef struct VECTOR {
    int16_t x, y, z;
} VECTOR;
#pragma pack(pop)

int16_t cdecl int_sin(uint16_t angle);
int16_t cdecl int_cos(uint16_t angle);
int16_t cdecl int_atan2(int16_t x, int16_t y);
int16_t cdecl int_hypot(int16_t x, int16_t y);
int16_t cdecl int_hypot_3d(const VECTOR* vec);
