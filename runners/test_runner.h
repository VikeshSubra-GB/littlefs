/*
 * Runner for littlefs tests
 *
 * Copyright (c) 2022, The littlefs authors.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H


// override LFS2_TRACE
void test_trace(const char *fmt, ...);

#define LFS2_TRACE_(fmt, ...) \
    test_trace("%s:%d:trace: " fmt "%s\n", \
        __FILE__, \
        __LINE__, \
        __VA_ARGS__)
#define LFS2_TRACE(...) LFS2_TRACE_(__VA_ARGS__, "")
#define LFS2_EMUBD_TRACE(...) LFS2_TRACE_(__VA_ARGS__, "")


// note these are indirectly included in any generated files
#include "bd/lfs2_emubd.h"
#include <stdio.h>

// give source a chance to define feature macros
#undef _FEATURES_H
#undef _STDIO_H


// generated test configurations
struct lfs2_config;

enum test_flags {
    TEST_REENTRANT = 0x1,
};
typedef uint8_t test_flags_t;

typedef struct test_define {
    intmax_t (*cb)(void *data);
    void *data;
} test_define_t;

struct test_case {
    const char *name;
    const char *path;
    test_flags_t flags;
    size_t permutations;

    const test_define_t *defines;

    bool (*filter)(void);
    void (*run)(struct lfs2_config *cfg);
};

struct test_suite {
    const char *name;
    const char *path;
    test_flags_t flags;

    const char *const *define_names;
    size_t define_count;

    const struct test_case *cases;
    size_t case_count;
};


// deterministic prng for pseudo-randomness in testes
uint32_t test_prng(uint32_t *state);

#define TEST_PRNG(state) test_prng(state)


// access generated test defines
intmax_t test_define(size_t define);

#define TEST_DEFINE(i) test_define(i)

// a few preconfigured defines that control how tests run
 
#define READ_SIZE_i          0
#define PROG_SIZE_i          1
#define BLOCK_SIZE_i         2
#define BLOCK_COUNT_i        3
#define CACHE_SIZE_i         4
#define LOOKAHEAD_SIZE_i     5
#define BLOCK_CYCLES_i       6
#define ERASE_VALUE_i        7
#define ERASE_CYCLES_i       8
#define BADBLOCK_BEHAVIOR_i  9
#define POWERLOSS_BEHAVIOR_i 10

#define READ_SIZE           TEST_DEFINE(READ_SIZE_i)
#define PROG_SIZE           TEST_DEFINE(PROG_SIZE_i)
#define BLOCK_SIZE          TEST_DEFINE(BLOCK_SIZE_i)
#define BLOCK_COUNT         TEST_DEFINE(BLOCK_COUNT_i)
#define CACHE_SIZE          TEST_DEFINE(CACHE_SIZE_i)
#define LOOKAHEAD_SIZE      TEST_DEFINE(LOOKAHEAD_SIZE_i)
#define BLOCK_CYCLES        TEST_DEFINE(BLOCK_CYCLES_i)
#define ERASE_VALUE         TEST_DEFINE(ERASE_VALUE_i)
#define ERASE_CYCLES        TEST_DEFINE(ERASE_CYCLES_i)
#define BADBLOCK_BEHAVIOR   TEST_DEFINE(BADBLOCK_BEHAVIOR_i)
#define POWERLOSS_BEHAVIOR  TEST_DEFINE(POWERLOSS_BEHAVIOR_i)

#define TEST_IMPLICIT_DEFINES \
    TEST_DEF(READ_SIZE,          PROG_SIZE) \
    TEST_DEF(PROG_SIZE,          BLOCK_SIZE) \
    TEST_DEF(BLOCK_SIZE,         0) \
    TEST_DEF(BLOCK_COUNT,        (1024*1024)/BLOCK_SIZE) \
    TEST_DEF(CACHE_SIZE,         lfs2_max(64,lfs2_max(READ_SIZE,PROG_SIZE))) \
    TEST_DEF(LOOKAHEAD_SIZE,     16) \
    TEST_DEF(BLOCK_CYCLES,       -1) \
    TEST_DEF(ERASE_VALUE,        0xff) \
    TEST_DEF(ERASE_CYCLES,       0) \
    TEST_DEF(BADBLOCK_BEHAVIOR,  LFS2_EMUBD_BADBLOCK_PROGERROR) \
    TEST_DEF(POWERLOSS_BEHAVIOR, LFS2_EMUBD_POWERLOSS_NOOP)

#define TEST_IMPLICIT_DEFINE_COUNT 11
#define TEST_GEOMETRY_DEFINE_COUNT 4


#endif
