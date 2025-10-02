#pragma once

#include <stdint.h>

#if defined(__WATCOMC__) && defined(__I86__)
// clang-format off
    // Like Watcom's default __cdecl, but don't mangle the symbol name.
    #pragma aux __cdecl "*"                           \
                parm caller[]                         \
                value struct float struct routine[ax] \
                modify[ax bx cx dx es]
// clang-format on
#else
    #define __far
    #define __cdecl
#endif
