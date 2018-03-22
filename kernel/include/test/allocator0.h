#ifndef TEST_ALLOCATOR0_H_
#define TEST_ALLOCATOR0_H_

#include <test/test.h>

typedef struct
{
    size_t id;
} nvmos_test_testcase_t(allocator0, test0);

size_t nvmos_test_getTestCases(allocator0, test0)(
    nvmos_test_testcase_t(allocator0, test0) * *res);

nvmos_test_testDriver(allocator0, test0);

nvmos_test_testRunner(allocator0, test0);

#endif