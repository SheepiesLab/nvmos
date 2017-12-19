#include <test/test.h>
#include <stdio.h>

#include <test/heap0.h>

void nvmos_test_success(
    char *testName,
    size_t testCaseNum)
{
    printf("Test: %s Case: %d Success!\n", testName, (int)testCaseNum);
}

int nvmos_test_runAllTests(
    nvmos_ptr_t availMem,
    size_t availMemLen)
{
    printf("Avail Memory Address: %p\n", availMem);
    printf("Avail Memory Size: %p\n", availMemLen);
    RUN_HEAP0_TEST0(availMem, availMemLen);
}