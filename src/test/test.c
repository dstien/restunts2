#include "test.h"

// Buffer for copying objects into DGROUP.
#ifdef PLATFORM_DOS16
uint8_t far* g_dg_buf;
unsigned g_dg_buf_idx;
#endif

test_result_t g_total_tests = { 0, 0 };
test_result_t g_total_groups = { 0, 0 };
bool g_verbose = false;
bool g_extensive = false;

void print_result(test_result_t* res, const char* indent)
{
    int total = res->pass + res->fail;
    int quot = (uint32_t)res->pass * 1000 / total;
    int pct_int = quot / 10;
    int pct_frac = quot % 10;
    if (res->fail) {
        PRINT_COLOR(
                RED, FSTR() "%d/%d (%d.%d%%)", indent, res->pass, total, pct_int, pct_frac
        );
    }
    else {
        PRINT_COLOR(
                GREEN, FSTR() "%d/%d (%d.%d%%)", indent, res->pass, total, pct_int, pct_frac
        );
    }
    tprintf(" passed\n");
}

int testmain(int argc, char near* near* argv)
{
    dg_reset();
    tprintf("restunts test suite\n");
    tprintf("  Platform  : " PLATFORM_STR "\n");
    tprintf("  Build date: " DATE "\n\n");

    for (int i = 1; i < argc; i++) {
        const char* a = argv[i];
        if (!stricmp(_(a), _("-v"))) {
            g_verbose = true;
        }
        else if (!stricmp(_(a), _("-x"))) {
            g_extensive = true;
            g_verbose = true;
        }
        else {
#ifdef PLATFORM_DOS16
            tprintf("Usage: %Fs [-x] [-v]\n", argv[0]);
            tprintf("  -x   Extensive range tests\n");
#else
            tprintf("Usage: %s [-v]\n", argv[0]);
#endif
            tprintf("  -v   Verbose output\n");
            return 1;
        }
    }

    PRINT_COLOR(DIM, "math.c\n");
    test_math();

    tprintf("\n");
    tprintf("Total functions: ");
    print_result(&g_total_groups, "");
    tprintf("Total tests    : ");
    print_result(&g_total_tests, "");

    return g_total_tests.fail;
}
