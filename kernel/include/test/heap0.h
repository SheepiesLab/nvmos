#ifndef TEST_HEAP0_H_
#define TEST_HEAP0_H_

#include <kernel/kdef.h>
#include <kernel/mman/heap/Heap.h>

typedef enum {
    MALLOC,
    FREE
} heapActions_t;

typedef struct
{
    heapActions_t action;
    uint32_t sizeOrLoc;
    nvmos_ptr_t result;
} testSeqUnit_t;

typedef struct
{
    size_t actionLen;
    testSeqUnit_t testSeq[];
} nvmos_test_case_heap0_test0_t;

#define HEAP0_TEST0_TESTCASE_COUNT 1

static nvmos_test_case_heap0_test0_t case0 = {
    10,
    {{MALLOC, 1, 0x8},
     {MALLOC, 2, 0x18},
     {MALLOC, 3, 0x30},
     {MALLOC, 4, 0x50},
     {FREE, 0x18, 0},
     {FREE, 0x30, 0},
     {MALLOC, 3, 0x18},
     {MALLOC, 2, 0x38},
     {FREE, 0x8, 0},
     {MALLOC, 1, 0x8}}};

static nvmos_test_case_heap0_test0_t *
    heap0_test0_cases[HEAP0_TEST0_TESTCASE_COUNT] =
        {&case0};

int nvmos_test_driver_heap0_test0(
    nvmos_ptr_t availMem,
    size_t availMemLen,
    nvmos_test_case_heap0_test0_t *testCase);

int nvmos_test_run_heap0_test0(
    nvmos_ptr_t availMem,
    size_t availMemLen,
    nvmos_test_case_heap0_test0_t *testCase[HEAP0_TEST0_TESTCASE_COUNT]);

#define RUN_HEAP0_TEST0(availMem, availMemLen) \
    nvmos_test_run_heap0_test0(availMem, availMemLen, heap0_test0_cases);

#endif