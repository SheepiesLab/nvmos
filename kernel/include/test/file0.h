#ifndef TEST_FILE0_H_
#define TEST_FILE0_H_

#include <test/test.h>

#define TEST_PART file0
#define TEST_NAME test0



typedef struct {

} nvmos_test_testcase_t(TEST_PART, TEST_NAME);

#define nvmos_test_testcaseCount(TEST_PART, TEST_NAME) 1

nvmos_test_testDriver(TEST_PART, TEST_NAME);

nvmos_test_testRunner(TEST_PART, TEST_NAME);

#undef TEST_PART
#undef TEST_NAME 

#endif