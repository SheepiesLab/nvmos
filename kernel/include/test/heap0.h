#ifndef TEST_HEAP0_H_
#define TEST_HEAP0_H_

#include <kdef.h>
#include <kernel/noarch/heap.h>

typedef enum heapActions {
    MALLOC,
    FREE
} heapActions_t;

typedef struct
{
    heapActions_t *actions;
    uint32_t *sizeOrLoc;
    nvmos_ptr_t *results;
    size_t actionLen;
} nvmos_test_case_heap0_test0_t;



int nvmos_test_driver_heap0_test0(
    nvmos_ptr_t availMem,
    size_t availMemLen,
    nvmos_test_case_heap0_test0_t *testCase);

#endif