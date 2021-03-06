#ifndef TEST_H_
#define TEST_H_

#include <kernel/kdef.h>

int nvmos_test_runAllTests(
    const nvmos_ptr_t availMem,
    const size_t availMemLen);

void nvmos_test_success(
    char *testName,
    size_t testCaseNum);

#define nvmos_test_fail(                             \
    testName,                                        \
    testCaseNum,                                     \
    errorMsg,                                        \
    ...)                                             \
    printf(                                          \
        "---!!! Test: %s Case: %d Failed! !!!---\n", \
        testName,                                    \
        testCaseNum);                                \
    printf("---!!! ErrorMsg: ");                     \
    printf(errorMsg, __VA_ARGS__);                   \
    printf(" !!!---\n");

#define nvmos_test_testRunner(testPart, testNo) \
    int nvmos_test_run_##testPart##_##testNo(   \
        nvmos_ptr_t availMem,                   \
        size_t availMemLen)

#define nvmos_test_defTestRunner(testPart, testNo)                        \
    nvmos_test_testRunner(testPart, testNo)                               \
    {                                                                     \
        nvmos_test_case_##testPart##_##testNo##_t **testCase;             \
        size_t testPart##_##testNo##_TESTCASE_COUNT =                     \
            nvmos_test_getTestCases(testPart, testNo)(&testCase);         \
        char testName[] = #testPart "_" #testNo;                          \
        for (size_t i = 0; i < testPart##_##testNo##_TESTCASE_COUNT; ++i) \
        {                                                                 \
            int testResult =                                              \
                nvmos_test_driver_##testPart##_##testNo(                  \
                    availMem,                                             \
                    availMemLen,                                          \
                    testCase[i]);                                         \
            if (testResult)                                               \
            {                                                             \
            }                                                             \
            else                                                          \
            {                                                             \
                nvmos_test_success(testName, i);                          \
            }                                                             \
        }                                                                 \
        memset(availMem, 0, availMemLen);                                 \
    }

#define nvmos_test_testcase_t(testPart, testNo) \
    nvmos_test_case_##testPart##_##testNo##_t

#define nvmos_test_testcase(testPart, testNo, testCase) \
    testPart##testNo##testCase

#define nvmos_test_getTestCases(testPart, testNo) \
    nvmos_test_getTestCases_##testPart##_##testNo

#define nvmos_test_cases(testPart, testNo) \
    testPart##_##testNo##_##cases

#define nvmos_test_testcaseCount(testPart, testNo) \
    testPart##_##testNo##_TESTCASE_COUNT

#define nvmos_test_testDriver(testPart, testNo)  \
    int nvmos_test_driver_##testPart##_##testNo( \
        nvmos_ptr_t availMem,                    \
        size_t availMemLen,                      \
        nvmos_test_case_##testPart##_##testNo##_t *testCase)

#define nvmos_test_runTest(testPart, testNo, availMem, availMemLen) \
    nvmos_test_run_##testPart##_##testNo(availMem, availMemLen);

#endif