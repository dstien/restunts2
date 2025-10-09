#include "util.h"
#include <stdint.h>

void memcpy(void* dst, const void* src, size_t n)
{
    uint8_t* d = dst;
    const uint8_t* s = src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
}

FILE* fopen(const char* path, const char* mode)
{
    struct SREGS s;
    union REGS r;

    s.ds = FP_SEG(path);
    r.x.dx = FP_OFF(path);

    // Create new file for writing.
    if (mode && mode[0] == 'w') {
        r.h.ah = 0x3C; // Create file.
        r.x.cx = 0;    // No attributes.
        intdosx(&r, &r, &s);
    }
    // Open existing file for reading.
    else {
        r.h.ah = 0x3D; // Open file.
        r.h.al = 0;    // Read only.
        intdosx(&r, &r, &s);
    }

    if (r.x.cflag) {
        return 0;
    }

    return (FILE*)r.x.ax;
}

int fclose(FILE* file)
{
    union REGS r;

    r.h.ah = 0x3E; // Close file.
    r.x.bx = (unsigned)file;
    intdos(&r, &r);

    if (r.x.cflag) {
        return -1;
    }

    return 0;
}

size_t fwrite(const void far* src, size_t size, size_t nmemb, FILE* file)
{
    struct SREGS s;
    union REGS r;

    s.ds = FP_SEG(src);
    r.x.dx = FP_OFF(src);
    r.h.ah = 0x40; // Write to file.
    r.x.bx = (unsigned)file;
    r.x.cx = size * nmemb;
    intdosx(&r, &r, &s);

    return r.x.ax;
}
