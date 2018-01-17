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
    nvmos_ptr_t availMem,
    size_t availMemLen)
{
    printf("Test Memory Address: %p\n", availMem);
    printf("Test Memory Size: %p\n", availMemLen);
    
    nvmos_test_runTest(heap0, test0, availMem, availMemLen);
    nvmos_test_runTest(ptrBlks0, test0, availMem, availMemLen);
}