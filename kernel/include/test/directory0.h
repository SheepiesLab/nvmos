#ifndef TEST_DIRECTORY0_H_
#define TEST_DIRECTORY0_H_

#include <test/test.h>

typedef struct
{

} nvmos_test_testcase_t(directory0, test0);

size_t nvmos_test_getTestCases(directory0, test0)(
    nvmos_test_testcase_t(directory0, test0) * **res);

nvmos_test_testDriver(directory0, test0);

nvmos_test_testRunner(directory0, test0);

#endif