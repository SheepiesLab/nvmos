#ifndef TEST_PTRBLKS0_H_
#define TEST_PTRBLKS0_H_

#define ptrBlks0_test0_TESTCASE_COUNT 0

typedef struct
{
    uint8_t *data;
    int32_t *pushSeq;
} nvmos_test_case_ptrBlks0_test0_t;

nvmos_test_testDriver(ptrBlks0, test0);

int nvmos_test_run_ptrBlks0_test0(
    nvmos_ptr_t availMem,
    size_t availMemLen,
    nvmos_test_case_ptrBlks0_test0_t *testCase[PTRBLKS0_TEST0_TESTCASE_COUNT]);

#endif