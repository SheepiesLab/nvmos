#include <test/test.h>
#include <test/allocator0.h>

#include <kernel/datalayer/allocator/allocator.h>

#include <stdio.h>

size_t nvmos_test_getTestCases(allocator0, test0)(
    nvmos_test_testcase_t(allocator0, test0) * *res)
{
    static nvmos_test_testcase_t(allocator0, test0) cases = {.id = 1};
    *res = cases;
    return 1;
}

nvmos_test_testDriver(allocator0, test0)
{

    return 0;
}

nvmos_test_defTestRunner(allocator0, test0)