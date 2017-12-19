#ifndef TEST_H_
#define TEST_H_

#include <kernel/kdef.h>

int nvmos_test_runAllTests(
    nvmos_ptr_t availMem,
    size_t availMemLen);

void nvmos_test_success(
    char *testName,
    size_t testCaseNum);

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

#endif