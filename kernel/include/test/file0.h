#ifndef TEST_FILE0_H_
#define TEST_FILE0_H_

#include <test/test.h>

typedef struct
{
    void *useless;
} nvmos_test_testcase_t(file0, test0);

size_t nvmos_test_getTestCases(file0, test0)(
    nvmos_test_testcase_t(file0, test0) * *res);

nvmos_test_testDriver(file0, test0);

nvmos_test_testRunner(file0, test0);

#endif