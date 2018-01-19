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
    testSeqUnit_t *testSeq;
} nvmos_test_testcase_t(heap0, test0);

size_t nvmos_test_getTestCases(heap0, test0)(
    nvmos_test_testcase_t(heap0, test0) * *res);

nvmos_test_testDriver(heap0, test0);

nvmos_test_testRunner(heap0, test0);

#endif