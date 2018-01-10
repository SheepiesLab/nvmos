#ifndef TEST_HEAP0_H_
#define TEST_HEAP0_H_

#include <kernel/kdef.h>
#include <kernel/mman/heap/Heap.h>
#include <test/test.h>

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
} nvmos_test_testcase_t(heap0, test0);

static nvmos_test_testcase_t(heap0, test0) case0 = {
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

static const unsigned int nvmos_test_testcaseCount(heap0, test0) = 1;

static nvmos_test_testcase_t(heap0, test0) *
    nvmos_test_cases(heap0, test0)
        [nvmos_test_testcaseCount(heap0, test0)] =
    {&case0};

nvmos_test_testDriver(heap0, test0);

nvmos_test_testRunner(heap0, test0);

#endif