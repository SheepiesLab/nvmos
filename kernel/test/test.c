#include <test/test.h>
#include <stdio.h>

#include <test/heap0.h>

void nvmos_test_success(
    char *testName,
    uint32_t testCaseNum)
{
    printf("Test: %s Case: %d Success!\n", testName, testCaseNum);
}

#define nvmos_test_fail(                             \
    testName,                                        \
    testCaseNum,                                     \
    errorMsg,                                        \
    errorMsgPara)                                    \
    printf(                                          \
        "---!!! Test: %s Case: %d Failed! !!!---\n", \
        testName,                                    \
        testCaseNum);                                \
    printf("---!!! ErrorMsg: ");                     \
    printf(errorMsg, errorMsgPara);                  \
    printf(" !!!---\n");

int nvmos_test_runAllTests(
    nvmos_ptr_t availMem,
    size_t availMemLen)
{
    RUN_HEAP0_TEST0(availMem, availMemLen);
}