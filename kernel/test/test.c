#include <test/test.h>
#include <stdio.h>

#include <test/heap0.h>
#include <test/ptrBlks0.h>

void nvmos_test_success(
    char *testName,
    size_t testCaseNum)
{
    printf("Test: %s Case: %d Success!\n", testName, (int)testCaseNum);
}

int nvmos_test_runAllTests(
    const nvmos_ptr_t availMem,
    const size_t availMemLen)
{
    const nvmos_ptr_t _availMem = availMem;
    const size_t _availMemLen = availMemLen;

    printf("Test Memory Address: %p\n", _availMem);
    printf("Test Memory Size: %p\n", _availMemLen);

    nvmos_test_runTest(heap0, test0, _availMem, _availMemLen);
    nvmos_test_runTest(ptrBlks0, test0, _availMem, _availMemLen);
}