#include "test.h"

test_result_t g_total_tests = { 0, 0 };
test_result_t g_total_groups = { 0, 0 };
bool g_verbose = false;

void print_result(test_result_t* res, const char* indent)
{
    int total = res->pass + res->fail;
    int quot = (uint32_t)res->pass * 1000 / total;
    int pct_int = quot / 10;
    int pct_frac = quot % 10;
    if (res->fail) {
        PRINT_COLOR(RED, "%s%d/%d (%d.%d%%)", indent, res->pass, total, pct_int, pct_frac);
    }
    else {
        PRINT_COLOR(GREEN, "%s%d/%d (%d.%d%%)", indent, res->pass, total, pct_int, pct_frac);
    }
    printf(" passed\n");
}

int testmain(int argc, char** argv)
{
    printf("restunts test suite\n");
    printf("  Platform  : " PLATFORM_STR "\n");
    printf("  Build date: " DATE "\n\n");

    if (argc > 1) {
        if (argc == 2 && !stricmp(argv[1], "-v")) {
            g_verbose = true;
        }
        else {
            printf("Usage: %s [-v]\n\n", argv[0]);
            return 1;
        }
    }

    PRINT_COLOR(DIM, "math.c\n");
    test_math();

    printf("\n");
    printf("Total functions: ");
    print_result(&g_total_groups, "");
    printf("Total tests    : ");
    print_result(&g_total_tests, "");

    return g_total_tests.fail;
}
