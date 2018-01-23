#ifndef TEST_PTRBLKS0_H_
#define TEST_PTRBLKS0_H_

#include <test/test.h>

typedef struct
{
    int32_t *pushSeq;
    size_t pushSeqLen;
    size_t maxBlkCount;
} nvmos_test_testcase_t(ptrBlks0, test0);

size_t nvmos_test_getTestCases(ptrBlks0, test0)(
    nvmos_test_testcase_t(ptrBlks0, test0) * **res);

nvmos_test_testDriver(ptrBlks0, test0);

nvmos_test_testRunner(ptrBlks0, test0);

typedef struct
{
    int32_t *popSeq;
    size_t popSeqLen;
    size_t maxBlkCount;
} nvmos_test_testcase_t(ptrBlks0, test1);

size_t nvmos_test_getTestCases(ptrBlks0, test1)(
    nvmos_test_testcase_t(ptrBlks0, test1) * **res);

nvmos_test_testDriver(ptrBlks0, test1);

nvmos_test_testRunner(ptrBlks0, test1);

#endif