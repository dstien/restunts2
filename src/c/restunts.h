#ifndef RESTUNTS_RESTUNTS_H
#define RESTUNTS_RESTUNTS_H

#include <stdint.h>

#if defined(__WATCOMC__) && defined(__I86__)
    // Like Watcom's default __cdecl, but don't mangle the symbol name.
    #pragma aux __cdecl "*" parm \
        caller[] value struct float struct routine[ax] modify[ax bx cx dx es]
#else
    #define __far
    #define __cdecl
#endif

#endif
