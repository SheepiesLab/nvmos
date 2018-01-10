#ifndef TEST_HEAP0_H_
#define TEST_HEAP0_H_

#include <kernel/kdef.h>
#include <kernel/mman/heap/Heap.h>
#include <test/test.h>

#define TEST_PART heap0
#define TEST_NAME test0

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
} nvmos_test_testcase_t(TEST_PART, TEST_NAME);

static nvmos_test_testcase_t(TEST_PART, TEST_NAME) case0 = {
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

#define nvmos_test_testcaseCount(TEST_PART, TEST_NAME) 1

static nvmos_test_testcase_t(TEST_PART, TEST_NAME) *
    nvmos_test_cases(TEST_PART, TEST_NAME)
        [nvmos_test_testcaseCount(TEST_PART, TEST_NAME)] =
    {&case0};

nvmos_test_testDriver(TEST_PART, TEST_NAME);

nvmos_test_testRunner(TEST_PART, TEST_NAME);

#undef TEST_PART
#undef TEST_NAME

#endif