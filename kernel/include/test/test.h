#ifndef TEST_H_
#define TEST_H_

#include <kernel/kdef.h>

int nvmos_test_runAllTests(
    nvmos_ptr_t availMem,
    size_t availMemLen);

void nvmos_test_success(
    char *testName,
    uint32_t testCaseNum);

void nvmos_test_fail(
    char *testName,
    uint32_t testCaseNum);

#endif