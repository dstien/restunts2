#pragma once

#include "restunts.h"
#include <stdbool.h>

#ifdef PLATFORM_DOS16
    #include "externs.h"
#else
    #include <stdio.h>
    #include <strings.h>
    #define stricmp  strcasecmp
    #define testmain main
#endif

#ifndef PLATFORM_STR
    #define PLATFORM_STR "unknown"
#endif

#ifndef DATE
    #define DATE "unknown"
#endif

// dosemu, DOSBox and Windows 10 all support ANSI colours.
#define RED   "\033[31m"
#define GREEN "\033[32m"
#define DIM   "\033[2m"
#define RESET "\033[0m"

#define PRINT_COLOR(color, format, ...) \
    printf(color format RESET, ##__VA_ARGS__)

typedef struct {
    int pass;
    int fail;
} test_result_t;

extern test_result_t g_total_tests;
extern test_result_t g_total_groups;
extern bool g_verbose;

#define TEST(name, expr)                                                            \
    do {                                                                            \
        if (g_verbose) {                                                            \
            printf("    %-20s", #name);                                             \
        }                                                                           \
        if (!(expr)) {                                                              \
            res.fail++;                                                             \
            if (g_verbose) {                                                        \
                PRINT_COLOR(RED, "FAIL");                                           \
                PRINT_COLOR(DIM, "  %s:%d %s" RESET "", __FILE__, __LINE__, #expr); \
                printf("\n");                                                       \
            }                                                                       \
        }                                                                           \
        else {                                                                      \
            res.pass++;                                                             \
            if (g_verbose) {                                                        \
                PRINT_COLOR(GREEN, "PASS");                                         \
                printf("\n");                                                       \
            }                                                                       \
        }                                                                           \
    } while (0)

#define TEST_GROUP(name, BODY)                              \
    do {                                                    \
        test_result_t res = { 0, 0 };                       \
        if (g_verbose) {                                    \
            PRINT_COLOR(DIM, "  %s\n", #name);              \
        }                                                   \
        else {                                              \
            printf("  %-20s", #name);                       \
        }                                                   \
                                                            \
        BODY;                                               \
                                                            \
        if (g_verbose) {                                    \
            print_result(&res, "                        "); \
        }                                                   \
        else {                                              \
            if (res.fail) {                                 \
                PRINT_COLOR(RED, "FAIL");                   \
            }                                               \
            else {                                          \
                PRINT_COLOR(GREEN, "PASS");                 \
            }                                               \
            printf("\n");                                   \
        }                                                   \
        g_total_tests.pass += res.pass;                     \
        g_total_tests.fail += res.fail;                     \
        if (res.fail) {                                     \
            g_total_groups.fail++;                          \
        }                                                   \
        else {                                              \
            g_total_groups.pass++;                          \
        }                                                   \
    } while (0)

void print_result(test_result_t* res, const char* indent);
void test_math();
